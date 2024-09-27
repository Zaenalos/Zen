#pragma once

extern bool debugSendPacket;

namespace Gui {
	void Init(float screenHeight, float screenWidth);
    void Draw(float screenHeight, float screenWidth);
    void Render();
    bool OnTouch(float x, float y, unsigned int holding, int type);
}