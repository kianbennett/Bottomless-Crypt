#pragma once

#include <SDL.h>
#include <string>
#include "ecs.h"
#include "c_ui_text.h"
#include "c_transform.h"
#include "level.h"

extern Level* level;
extern TTF_Font* font;
extern bool showDebug;

class HUD {
public:
	HUD();
	void update(float deltaTime);
	void updateDebugInfo(bool showDebug);
	void setTooltip(std::string text);
	void addNotification(std::string text);
	
private:
	Entity timeText, infoText, levelText;
	Entity tooltipText;
	Entity notificationLines[3];
	float fpsCheckTime;
	Uint32 framesCounted;
	std::vector<std::string> notifications;
	float notificationTimer;
};