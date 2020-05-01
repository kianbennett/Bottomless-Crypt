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
extern bool showInventory;

class HUD {
public:
	HUD();
	void update(float deltaTime);
	void updateDebugInfo(bool showDebug);
	void updatePlayerStats(CharacterComponent character);
	void setTooltip(std::string text);
	void addNotification(std::string text);
	void updateInventory(CharacterComponent character);
	
private:
	Entity timeText, infoText, levelText;
	Entity goldText, healthText, strengthText, defenceText;
	Entity inventoryText, inventoryItems[8];
	Entity tooltipText;
	Entity notificationLines[3];
	float fpsCheckTime;
	Uint32 framesCounted;
	std::vector<std::string> notifications;
	float notificationTimer;
};