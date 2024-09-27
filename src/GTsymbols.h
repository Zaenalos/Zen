#pragma once
#include <cstdint>
#include "Structures.h"

char growtopiaLib[] = "libgrowtopia.so";

typedef NetAvatar* (*GetLocalPlayer_t)(int64_t);
typedef int64_t (*GetGameLogic_t)();
typedef int64_t (*OnTouch_t)(float, float, int64_t, int64_t, uint, int);
typedef int64_t (*SendPacket_t)(int64_t, uint8_t*, int64_t);
typedef int64_t (*BaseAppDraw_t)(int64_t);
typedef float (*GetScreenWidth_t)();
typedef float (*GetScreenHeight_t)();
typedef void (*SetFPSLimit_t)(float);
typedef void (*LogToConsole_t)(const char*, ...);

GetLocalPlayer_t oGetLocalPlayer;
GetGameLogic_t oGetGameLogic;
OnTouch_t onativeOnTouch;
SendPacket_t oSendPacket;
BaseAppDraw_t oBaseApp;
GetScreenWidth_t oGetScreenWidth;
GetScreenHeight_t oGetScreenHeight;
SetFPSLimit_t oSetFPSLimit;
LogToConsole_t oLogToConsole;