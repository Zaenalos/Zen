#include "Gui.h"
#include "Hook.h"
#include "Logger.h"
#include "Scanner.h"
#include "GTsymbols.h"
#include <cstdarg>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstdint>

static bool isInitialized = false;
static float screenWidth, screenHeight;

NetAvatar* Hook::HookGetLocalPlayer(int64_t gameLogic) {
	return oGetLocalPlayer(gameLogic);
}

int64_t Hook::HookGetGameLogic() {
	return oGetGameLogic();
}

int64_t HookSendPacket(int64_t type, uint8_t* packet, int64_t peer) {
    return oSendPacket(type, packet, peer);
}

void Hook::HookSetFPSLimit(float fps) {
    oSetFPSLimit(fps);
}

void Hook::HookLogToConsole(const char* msg, ...) {
	const char *prefix = "`2[ZEN]`` ";
    size_t prefix_len = strlen(prefix);

    va_list args;
    va_start(args, msg);
    va_list args_copy;
    va_copy(args_copy, args);
    
    int msg_len = vsnprintf(nullptr, 0, msg, args_copy);
    va_end(args_copy);

    size_t total_len = prefix_len + msg_len;
    char *buffer = (char *)malloc(total_len + 1);
    if (!buffer) {
        va_end(args);
        return;
    }

    strcpy(buffer, prefix);
    vsnprintf(buffer + prefix_len, msg_len + 1, msg, args);

    va_end(args);

    oLogToConsole("%s", buffer);

    free(buffer);
}

int64_t HookBaseAppDraw(int64_t idk) {
    if (!isInitialized) {
    	Gui::Init(screenWidth, screenHeight);
        isInitialized = true;
    }
    
    Gui::Draw(screenWidth, screenHeight);
    Gui::Render();
    
    return oBaseApp(idk);
}

int64_t HookOnTouch(float x, float y, int64_t arg1, int64_t arg2, unsigned int holding, int type) {
    bool notCaptureMouse = Gui::OnTouch(x, y, holding, type);

    if (notCaptureMouse) {
        return onativeOnTouch(x, y, arg1, arg2, holding, type);
    }
    return 0;
}

// Hook screen width function
float HookGetScreenWidth() {
    float width = oGetScreenWidth();
    screenWidth = width;
    LOG_INFO("Screen width: %f", width);
    return screenWidth;
}

// Hook screen height function
float HookGetScreenHeight() {
    float height = oGetScreenHeight();
    screenHeight = height;
    LOG_INFO("Screen height: %f", height);
    return screenHeight;
}

int Hook::installHooks(LibraryInfo libInfo) {
	void* stub;
    int success = 0;
    int error_num;
    
    Addresses addresses;
    
    Scanner::scanAll(&addresses, libInfo);
    
    //Hook GetLocalPlayer
    stub = shadowhook_hook_func_addr((void*)addresses.getLocalPlayerAddr, (void*)HookGetLocalPlayer, (void**)&oGetLocalPlayer);

	if (stub == nullptr) {
    	LOG_ERROR("Failed hooking GetLocalPlayer");
    	error_num = shadowhook_get_errno();
    	const char* error_msg = shadowhook_to_errmsg(error_num);
    	LOG_ERROR("Error cause: %d - %s", error_num, error_msg);
    	success = 1;
	} else {
    	LOG_INFO("Hook GetLocalPlayer Success! Address: %p", stub);
	}
    
    //Hook GetGameLogic
    stub = shadowhook_hook_func_addr((void*)addresses.getGameLogicAddr, (void*)HookGetGameLogic, (void**)&oGetGameLogic);

	if (stub == nullptr) {
    	LOG_ERROR("Failed hooking GetGameLogic");
    	error_num = shadowhook_get_errno();
    	const char* error_msg = shadowhook_to_errmsg(error_num);
    	LOG_ERROR("Error cause: %d - %s", error_num, error_msg);
    	success = 1;
	} else {
    	LOG_INFO("Hook GetGameLogic Success! Address: %p", stub);
	}
	
	// Hook SetFpsLimit
	stub = shadowhook_hook_func_addr((void*)addresses.setFpsAddr, (void*)HookSetFPSLimit, (void**)&oSetFPSLimit);

	if (stub == nullptr) {
    	LOG_ERROR("Failed hooking SetFPSLimit");
    	error_num = shadowhook_get_errno();
    	const char* error_msg = shadowhook_to_errmsg(error_num);
    	LOG_ERROR("Error cause: %d - %s", error_num, error_msg);
    	success = 1;
	} else {
    	LOG_INFO("Hook SetFPSLimit Success! Address: %p", stub);
	}
	
	// Hook LogToConsole
	stub = shadowhook_hook_func_addr((void*)addresses.logConsoleAddr, (void*)HookLogToConsole, (void**)&oLogToConsole);

	if (stub == nullptr) {
    	LOG_ERROR("Failed hooking LogToConsole");
    	error_num = shadowhook_get_errno();
    	const char* error_msg = shadowhook_to_errmsg(error_num);
    	LOG_ERROR("Error cause: %d - %s", error_num, error_msg);
    	success = 1;
	} else {
    	LOG_INFO("Hook LogToConsole Success! Address: %p", stub);
	}
	
	// Hook SendPacket
	stub = shadowhook_hook_func_addr((void*)addresses.sendPacketAddr, (void*)HookSendPacket, (void**)&oSendPacket);

	if (stub == nullptr) {
    	LOG_ERROR("Failed hooking SendPacket");
    	error_num = shadowhook_get_errno();
    	const char* error_msg = shadowhook_to_errmsg(error_num);
    	LOG_ERROR("Error cause: %d - %s", error_num, error_msg);
    	success = 1;
	} else {
    	LOG_INFO("Hook SendPacket Success! Address: %p", stub);
	}
	
    // Hook OnTouch
    stub = shadowhook_hook_sym_name(growtopiaLib, "Java_com_rtsoft_growtopia_AppGLSurfaceView_nativeOnTouch", (void*)HookOnTouch, (void**)&onativeOnTouch);

    if (stub == nullptr) {
        LOG_ERROR("Failed hooking OnTouch");
        error_num = shadowhook_get_errno();
        const char* error_msg = shadowhook_to_errmsg(error_num);
        LOG_ERROR("Error cause: %d - %s", error_num, error_msg);
        success = 1;
    } else {
        LOG_INFO("Hook OnTouch Success! Address: %p", stub);
    }

    // Hook nativeGetScreenWidth
    stub = shadowhook_hook_sym_name(growtopiaLib, "Java_com_rtsoft_growtopia_SharedActivity_nativeGetScreenWidth", (void*)HookGetScreenWidth, (void**)&oGetScreenWidth);

    if (stub == nullptr) {
        LOG_ERROR("Failed hooking nativeGetScreenWidth");
        error_num = shadowhook_get_errno();
        const char* error_msg = shadowhook_to_errmsg(error_num);
        LOG_ERROR("Error cause: %d - %s", error_num, error_msg);
        success = 1;
    } else {
        LOG_INFO("Hook nativeGetScreenWidth Success! Address: %p", stub);
        HookGetScreenWidth();
    }

    // Hook nativeGetScreenHeight
    stub = shadowhook_hook_sym_name(growtopiaLib, "Java_com_rtsoft_growtopia_SharedActivity_nativeGetScreenHeight", (void*)HookGetScreenHeight, (void**)&oGetScreenHeight);

    if (stub == nullptr) {
        LOG_ERROR("Failed hooking nativeGetScreenHeight");
        error_num = shadowhook_get_errno();
        const char* error_msg = shadowhook_to_errmsg(error_num);
        LOG_ERROR("Error cause: %d - %s", error_num, error_msg);
        success = 1;
    } else {
        LOG_INFO("Hook nativeGetScreenHeight Success! Address: %p", stub);
        HookGetScreenHeight();
    }
    
    // Hook BaseApp::Draw
    stub = shadowhook_hook_func_addr((void*)addresses.baseAppAddr, (void*)HookBaseAppDraw, (void**)&oBaseApp);

    if (stub == nullptr) {
        LOG_ERROR("Failed hooking BaseApp::Draw");
        error_num = shadowhook_get_errno();
        const char* error_msg = shadowhook_to_errmsg(error_num);
        LOG_ERROR("Error cause: %d - %s", error_num, error_msg);
        success = 1;
    } else {
        LOG_INFO("Hook BaseApp::Draw Success! Address: %p", stub);
    }
    
    return success;
}

int Hook::Init(LibraryInfo libInfo) {
    LOG_INFO("Hook initiating ...");

    if (shadowhook_init(SHADOWHOOK_MODE_UNIQUE, false) != 0) {
        LOG_ERROR("Hook init failed!");
        return 1;
    }
    
    LOG_INFO("Hook init success!"); 

    if (installHooks(libInfo) != 0) {
        LOG_ERROR("Failed to install hooks");
        return 1;
    }

    LOG_INFO("Hook installation successful!");
    return 0;
}