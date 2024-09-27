#include <sstream>
#include <iomanip>
#include "Structures.h"
#include "Logger.h"
#include "Hook.h"
#include "Gui.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"

static bool showHexViewer = false;
static bool showModMenu = true;
static bool showMenuButton = true;
bool debugSendPacket = false;
bool showAvatarInfo = false;
ImFont* default_font;

void ShowNetAvatarInfo(NetAvatar* avatar) {
    if (avatar) {
        ImGui::Text("NetAvatar Information:");
        ImGui::Text("Position: (X: %.2f, Y: %.2f)", avatar->pos.x, avatar->pos.y);
        ImGui::Text("Size: (X: %.2f, Y: %.2f)", avatar->size.x, avatar->size.y);
        ImGui::Text("Pos enc: (X: %.2f, Y: %.2f)", avatar->pos_enc.x, avatar->pos_enc.y);
        ImGui::Text("Size enc: (X: %.2f, Y: %.2f)", avatar->size_enc.x, avatar->size_enc.y);
        ImGui::Text("Net ID: %d", avatar->netID);
        ImGui::Text("Destroy: %d", avatar->destroy);
        ImGui::Text("Grow ID: %s", avatar->growID.c_str());
        ImGui::Text("Facing Left: %s", (avatar->facing_left) ? "Yes" : "No");
    } else {
        ImGui::Text("No current avatar, please enter in a world.");
    }
}

bool Gui::OnTouch(float x, float y, unsigned int holding, int type) {
    ImGuiIO& io = ImGui::GetIO();

    io.MousePos = ImVec2(x, y);

    switch (holding) {
    case 0: // Touch action
        io.MouseDown[0] = true;
        break;

    case 1: // finger release action
        io.MouseDown[0] = false;
        break;

    case 2: // Finger hovering or wiping the screen
        io.MouseDown[0] = true;
        break;

    default:
        io.MouseDown[0] = false;
        break;
    }
    
    if (!io.WantCaptureMouse) {
    	return true;
    }
    
    return false;
}

// Initialize ImGui and OpenGL
void Gui::Init(float screenWidth, float screenHeight) {
    LOG_INFO("GUI: Screen initialized with width: %f and height: %f", screenWidth, screenHeight);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = NULL;
    io.DisplaySize = ImVec2(screenWidth, screenHeight);
    
    ImFontConfig font_cfg;
    font_cfg.SizePixels = 30.0f;
    default_font = io.Fonts->AddFontDefault(&font_cfg);

    float scaleFactor = 3.0f;
    ImGui::GetStyle().ScaleAllSizes(scaleFactor);
    ImGui::StyleColorsDark();
    
    ImGui_ImplOpenGL3_Init("#version 300 es");
}

// Draw the mod menu UI
void Gui::Draw(float screenWidth, float screenHeight) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui::NewFrame();

    static float time = 0.0f;  // Time variable to animate the colors
    time += ImGui::GetIO().DeltaTime;  // Increase over time

    // Calculate RGB values
    float r = (sin(time * 2.0f) * 0.5f) + 0.5f;  // Cycles between 0 and 1
    float g = (sin(time * 2.0f + 2.0f) * 0.5f) + 0.5f;
    float b = (sin(time * 2.0f + 4.0f) * 0.5f) + 0.5f;

    // Create color for the text
    ImVec4 color = ImVec4(r, g, b, 1.0f);  // RGB + Alpha = 1.0 for full opacity

    ImVec2 textPos = ImVec2(10, 10);  // Position on the screen (change as needed)
    ImGui::GetForegroundDrawList()->AddText(default_font, 100.0f, textPos, ImColor(color), "ZEN");

    // Render the ImGui window with the mod menu
    if (showModMenu) {
        ImGui::SetNextWindowSize(ImVec2(screenWidth / 2, screenHeight / 3), ImGuiCond_FirstUseEver);
        ImGui::Begin("Zen Menu", &showModMenu, ImGuiWindowFlags_NoCollapse);

        if (ImGui::Checkbox("Debug SendPacket", &debugSendPacket)) {
            LOG_INFO("Debug SendPacket: %s", debugSendPacket ? "Enabled" : "Disabled");
        }

        ImGui::Checkbox("Show Avatar Info", &showAvatarInfo);  // Add toggle for avatar info
        ImGui::Separator();

        static float fpsLimit = 60.0f;  // Default value
        if (ImGui::SliderFloat("FPS Limit", &fpsLimit, 30.0f, 120.0f, "%.0f FPS")) {
            Hook::HookSetFPSLimit(fpsLimit);
        }

        ImGui::End();
    }

    // Show avatar info if the checkbox is checked
    if (showAvatarInfo) {
        ImGui::SetNextWindowSize(ImVec2(screenWidth / 2, screenHeight / 4), ImGuiCond_FirstUseEver);
        ImGui::Begin("Avatar Info", &showAvatarInfo, ImGuiWindowFlags_NoCollapse);

        // Retrieve the NetAvatar struct using your hook
        NetAvatar* avatar = Hook::HookGetLocalPlayer(Hook::HookGetGameLogic());
        ShowNetAvatarInfo(avatar);  // Display avatar info

        ImGui::End();
    }

    // Floating button to toggle mod menu
    if (showMenuButton) {
        ImGui::SetNextWindowPos(ImVec2(10, 10));  // Position the floating button at the top-left corner
        ImGui::Begin("Mod Menu Button", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);

        if (ImGui::Button("Open Mod Menu")) {
            showModMenu = !showModMenu;
        }

        ImGui::End();
    }
}

// Render the UI
void Gui::Render() {
	ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}