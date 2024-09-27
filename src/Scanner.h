#pragma once
#include "Structures.h"
#include <cstdint>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdint>
#include <iomanip>
#include <string>

namespace Scanner {
	void parsePattern(const std::string& patternStr, std::vector<uint8_t>& pattern, std::string& mask);
	inline bool comparePattern(const uint8_t* data, const Pattern& pattern);
	uintptr_t scanForPattern(uintptr_t baseAddr, size_t size, const Pattern& pattern);
	void scanChunk(uintptr_t baseAddr, size_t size, const Pattern& pattern, std::atomic<uintptr_t>& result);
	void scanAll(Addresses* addresses, LibraryInfo libInfo);
}