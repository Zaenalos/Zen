cmake_minimum_required(VERSION 3.30)

set(CMAKE_TOOLCHAIN_FILE "$ENV{HOME}/NDK/android-ndk-r27b/build/cmake/android.toolchain.cmake")
set(ANDROID_PLATFORM 33)
set(CMAKE_ANDROID_ARCH_ABI arm64-v8a)
set(ANDROID_ABI arm64-v8a)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_BUILD_TYPE Release)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_VERBOSE_MAKEFILE ON)
endif()

include(ProcessorCount)
ProcessorCount(N)
if(N GREATER 1)
    set(CMAKE_BUILD_PARALLEL_LEVEL ${N})
    message(STATUS "Using ${N} processors for building.")
else()
    message(STATUS "Processor count not detected, using default.")
endif()

project(ProjectHook)

add_subdirectory(essentials)
add_subdirectory(src)

set_target_properties(${PROJECT_NAME} PROPERTIES 
    OUTPUT_NAME "Zen"
    LIBRARY_OUTPUT_DIRECTORY "/data/data/com.termux/files/home/storage/shared/"
)

# Apply compile flags to the target
target_compile_options(${PROJECT_NAME} PRIVATE
    -O3
    -march=armv8-a
    -mcpu=generic
    -flto
    -fopenmp
    -ffast-math
    -funroll-loops
    -fomit-frame-pointer
    -fmerge-all-constants
    -fno-exceptions
    -fvisibility=hidden
    -fdata-sections
    -ffunction-sections
    -fstrict-aliasing     # Optional, if code is alias-safe
    -finline-functions     # Further inlining with LTO
    -fstack-protector-strong # Optional for added security
)

# Apply linker flags to the target
target_link_options(${PROJECT_NAME} PRIVATE
    -flto
    -Wl,--gc-sections
    -Wl,--strip-all
    -Wl,-O3               # Switch to O3 for more aggressive optimizations
    -Wl,--as-needed
    -Wl,--relax           # ARM-specific optimization
    -Wl,--sort-common     # Optimize memory usage by sorting common symbols
    -Wl,--no-undefined    # Ensure all symbols are defined
)

# Display project configuration
message(STATUS "Project name: ${PROJECT_NAME}")
message(STATUS "Toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
message(STATUS "ABI version: ${ANDROID_ABI}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")