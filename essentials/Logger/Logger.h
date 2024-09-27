#pragma once

#include <android/log.h>

// Log tag
#define LOG_TAG "Zen"

// Define log macros
#define LOG(level, ...) do { \
    __android_log_print(level, LOG_TAG, __VA_ARGS__); \
} while (0)

#define LOG_INFO(...) LOG(ANDROID_LOG_INFO, __VA_ARGS__)
#define LOG_WARN(...) LOG(ANDROID_LOG_WARN, __VA_ARGS__)
#define LOG_ERROR(...) LOG(ANDROID_LOG_ERROR, __VA_ARGS__)
#define LOG_DEBUG(...) LOG(ANDROID_LOG_DEBUG, __VA_ARGS__)