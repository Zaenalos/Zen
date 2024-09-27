#include "Scanner.h"
#include "Logger.h"
#include <unordered_map>
#include <cstring>
#include <thread>
#include <vector>
#include <atomic>

std::unordered_map<std::string, Pattern> patterns;

void initializePatterns() {
	patterns["getLocalPlayer"] = { {}, "" };
	patterns["getGameLogic"] = { {}, "" };
	patterns["sendPacket"] = { {}, "" };
    patterns["baseAppDraw"] = { {}, "" };
    patterns["fpsLimit"] = { {}, "" };
    patterns["logConsole"] = { {}, "" };

    // Parse patterns
    Scanner::parsePattern("? ? ? F9 C0 03 5F D6 ? ? ? FC", 
			patterns["getLocalPlayer"].bytes, patterns["getLocalPlayer"].mask);
    Scanner::parsePattern("? ? ? A9 FD 03 00 91 ? ? ? 97 ? ? ? F9 ? ? ? A8 C0 03 5F D6 ? ? ? 91", 
			patterns["getGameLogic"].bytes, patterns["getGameLogic"].mask);
    Scanner::parsePattern("? ? ? D1 ? ? ? A9 ? ? ? 91 ? ? ? F9 ? ? ? A9 ? ? ? A9 ? ? ? D5 F3 03 02 AA ? ? ? F9 F4 03 01 AA ? ? ? F8", 
            patterns["sendPacket"].bytes, patterns["sendPacket"].mask);
    Scanner::parsePattern("? ? ? 6D ? ? ? A9 ? ? ? 91 ? ? ? A9 ? ? ? A9 ? ? ? A9 ? ? ? D1 ? ? ? D5 F3 03 00 AA ? ? ? 6F", 
            patterns["baseAppDraw"].bytes, patterns["baseAppDraw"].mask);
    Scanner::parsePattern("? ? ? D1 ? ? ? A9 ? ? ? 91 ? ? ? F9 ? ? ? D5 ? ? ? F9 ? ? ? 1E", 
            patterns["fpsLimit"].bytes, patterns["fpsLimit"].mask);
    Scanner::parsePattern("? ? ? A9 ? ? ? F9 FD 03 00 91 ? ? ? A9 ? ? ? A9 ? ? ? D1 ? ? ? D1 ? ? ? A9 F3 03 00 AA ? ? ? A9 ? ? ? 91", 
            patterns["logConsole"].bytes, patterns["logConsole"].mask);
}

void Scanner::parsePattern(const std::string& patternStr, std::vector<uint8_t>& pattern, std::string& mask) {
    std::istringstream iss(patternStr);
    std::string byteStr;

    while (iss >> byteStr) {
        if (byteStr == "?") {
            pattern.push_back(0);
            mask += "?";
        } else {
            uint8_t byte = static_cast<uint8_t>(std::stoi(byteStr, nullptr, 16));
            pattern.push_back(byte);
            mask += "x";
        }
    }
}

inline bool Scanner::comparePattern(const uint8_t* data, const Pattern& pattern) {
    const size_t patternSize = pattern.bytes.size();
    for (size_t i = 0; i < patternSize; i++) {
        if (pattern.mask[i] == 'x' && data[i] != pattern.bytes[i]) {
            return false;
        }
    }
    return true;
}

uintptr_t Scanner::scanForPattern(uintptr_t baseAddr, size_t size, const Pattern& pattern) {
    size_t patternSize = pattern.bytes.size();
    size_t endAddr = baseAddr + size - patternSize;

    for (uintptr_t addr = baseAddr; addr <= endAddr; addr++) {
        if (comparePattern(reinterpret_cast<const uint8_t*>(addr), pattern)) {
            LOG_INFO("Pattern found at address: 0x%lx", addr);
            return addr;
        }
    }
    LOG_INFO("Pattern not found");
    return 0;
}

void Scanner::scanChunk(uintptr_t baseAddr, size_t size, const Pattern& pattern, std::atomic<uintptr_t>& result) {
    size_t patternSize = pattern.bytes.size();
    size_t endAddr = baseAddr + size - patternSize;

    for (uintptr_t addr = baseAddr; addr <= endAddr; addr++) {
        if (comparePattern(reinterpret_cast<const uint8_t*>(addr), pattern)) {
            result = addr;
            return;
        }
    }
}

void Scanner::scanAll(Addresses* addresses, LibraryInfo libInfo) {
    uintptr_t baseAddr = libInfo.baseAddr;
    size_t libSize = libInfo.libSize;

    // Initialize patterns once
    initializePatterns();
	
	std::atomic<uintptr_t> getLocalPlayerAddr{0};
	std::atomic<uintptr_t> getGameLogicAddr{0};
	std::atomic<uintptr_t> sendPacketAddr{0};
    std::atomic<uintptr_t> fpsAddr{0};
    std::atomic<uintptr_t> logConsoleAddr{0};
    std::atomic<uintptr_t> baseAppAddr{0};

    size_t numThreads = std::thread::hardware_concurrency();
    size_t chunkSize = libSize / numThreads;

    std::vector<std::thread> threads;

    // Scan using multiple threads
    for (size_t i = 0; i < numThreads; ++i) {
        uintptr_t chunkBase = baseAddr + i * chunkSize;
        size_t size = (i == numThreads - 1) ? (libSize - (chunkSize * (numThreads - 1))) : chunkSize;
		
		threads.emplace_back(scanChunk, chunkBase, size, patterns["getLocalPlayer"], std::ref(getLocalPlayerAddr));
		threads.emplace_back(scanChunk, chunkBase, size, patterns["getGameLogic"], std::ref(getGameLogicAddr));
		threads.emplace_back(scanChunk, chunkBase, size, patterns["sendPacket"], std::ref(sendPacketAddr));
        threads.emplace_back(scanChunk, chunkBase, size, patterns["fpsLimit"], std::ref(fpsAddr));
        threads.emplace_back(scanChunk, chunkBase, size, patterns["logConsole"], std::ref(logConsoleAddr));
        threads.emplace_back(scanChunk, chunkBase, size, patterns["baseAppDraw"], std::ref(baseAppAddr));
    }

    for (auto& thread : threads) {
        thread.join();
    }
	
	addresses->getLocalPlayerAddr = getLocalPlayerAddr.load();
	addresses->getGameLogicAddr = getGameLogicAddr.load();
	addresses->sendPacketAddr = sendPacketAddr.load();
    addresses->setFpsAddr = fpsAddr.load();
    addresses->logConsoleAddr = logConsoleAddr.load();
    addresses->baseAppAddr = baseAppAddr.load();
}