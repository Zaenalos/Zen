#pragma once

#include "shadowhook.h"
#include "Structures.h"
#include <cstdint>

namespace Hook {
    int installHooks(LibraryInfo libInfo);
    int Init(LibraryInfo libInfo);
    
    // References for useful hooks
    NetAvatar* HookGetLocalPlayer(int64_t gameLogic);
    int64_t HookGetGameLogic();
    void HookSetFPSLimit(float fps);
    void HookLogToConsole(const char *msg, ...);
}