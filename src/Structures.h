#pragma once
#include <cstdint>
#include <string>
#include <vector>

struct LibraryInfo {
    uintptr_t baseAddr;
    size_t libSize;
};

struct Addresses {
	uintptr_t getLocalPlayerAddr;
	uintptr_t getGameLogicAddr;
	uintptr_t sendPacketAddr;
    uintptr_t logConsoleAddr;
    uintptr_t baseAppAddr;
	uintptr_t setFpsAddr;
};

struct Pattern {
    std::vector<uint8_t> bytes;
    std::string mask;
};

struct CL_Vec2f {
    float x = 0.f, y = 0.f;
    CL_Vec2f(float x = 0.f, float y = 0.f) { this->x = x; this->y = y; }
};

struct NetAvatar {
	void* vtable;
    CL_Vec2f pos;
    CL_Vec2f size;
    CL_Vec2f pos_enc;
    CL_Vec2f size_enc;
    std::string growID;
    uint32_t netID;
    uint32_t destroy;
    char pad1[1];
    void* unknown0;
    uint8_t unknown1;
    uint8_t facing_left;
};