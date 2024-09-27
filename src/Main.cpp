#include <iostream>
#include <thread>
#include <chrono>
#include <dlfcn.h>
#include <link.h> // For dl_iterate_phdr
#include <cstring> // For memcmp
#include "Hook.h"
#include "Logger.h"
#include "Structures.h"

// Callback function for dl_iterate_phdr to get base address and size
int callback(struct dl_phdr_info *info, size_t size, void *data) {
    if (std::string(info->dlpi_name).find("libgrowtopia.so") != std::string::npos) {
        uintptr_t baseAddr = info->dlpi_addr;
        size_t libSize = 0;

        // Iterate over the program headers to calculate the total size
        for (int i = 0; i < info->dlpi_phnum; i++) {
            if (info->dlpi_phdr[i].p_type == PT_LOAD) {
                // Calculate the end address of this segment
                uintptr_t segmentStart = baseAddr + info->dlpi_phdr[i].p_vaddr;
                uintptr_t segmentEnd = segmentStart + info->dlpi_phdr[i].p_memsz;
                libSize = std::max(libSize, segmentEnd - baseAddr);
            }
        }

        LOG_INFO("libgrowtopia.so base address: %p, size: %zu bytes", (void*)baseAddr, libSize);

        // Store the base address and size in the LibraryInfo structure
        LibraryInfo* libInfo = reinterpret_cast<LibraryInfo*>(data);
        libInfo->baseAddr = baseAddr;
        libInfo->libSize = libSize;

        return 1; // Stop iterating once we found the library
    }
    return 0; // Continue iterating if not found
}

void InitializeLibrary() {
    LibraryInfo libInfo = {0, 0};

    while (true) {
        // Use dl_iterate_phdr to find the base address and size of libgrowtopia.so
        if (dl_iterate_phdr(callback, &libInfo) > 0) {
            LOG_INFO("libgrowtopia.so found with base address %p and size %zu bytes", (void*)libInfo.baseAddr, libInfo.libSize);

            break; // Exit the loop once the library is found
        } else {
            LOG_INFO("libgrowtopia.so not found, retrying in 1 second...");
            std::this_thread::sleep_for(std::chrono::seconds(1)); // Delay before retrying
        }
    }

    // Initialize hooks if pattern is found
    if (Hook::Init(libInfo) == 0) {
        LOG_INFO("Hook success!");
    } else {
        LOG_ERROR("Failed to initialize hooks");
    }
}

// Constructor function that initializes the library in a detached thread
__attribute__((constructor))
void library_init() {
    LOG_INFO("Shared library libZen loaded and initializing...");

    std::thread([]() {
        InitializeLibrary();
    }).detach();
}