#pragma once

#include <SDL.h>
#include <string>
#include <algorithm>
#include "ecs.h"
#include "c_ui_text.h"
#include "c_transform.h"
#include "level.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

extern Level* level;
extern TTF_Font* font;
extern bool showDebug;
extern bool showInventory;

struct InventoryLine {
	Entity name;
	Entity strength, gold;

	void setActive(bool active) {
		ECS::entityManager->setActive(name, active);
		ECS::entityManager->setActive(strength, active);
		ECS::entityManager->setActive(gold, active);
	}

	void setValues(ItemComponent item, bool equipped) {
		ECS::getComponent<UITextComponent>(name).text = item.name + (equipped ? " (E)" : "");
		ECS::getComponent<UITextComponent>(name).textChanged = true;
		ECS::getComponent<UITextComponent>(strength).text = item.strength ? std::to_string(item.strength) : "-";
		ECS::getComponent<UITextComponent>(strength).textChanged = true;
		ECS::getComponent<UITextComponent>(gold).text = item.value ? std::to_string(item.value) : "-";
		ECS::getComponent<UITextComponent>(gold).textChanged = true;
	}
};

class HUD {
public:
	int inventoryIndex;

	HUD();
	void update(float deltaTime);
	void updateDebugInfo(bool showDebug);
	void updatePlayerStats(CharacterComponent character);
	void setTooltip(std::string text);
	void addNotification(std::string text);
	void updateInventory();
	void nextInventoryItem();
	void prevInventoryItem();
	
private:
	Entity timeText, infoText, levelText;
	Entity goldText, healthText;

	Entity inventoryText;
	Entity inventoryDivider1, inventoryDivider2;
	Entity inventoryHeader;
	InventoryLine inventoryLines[8];

	Entity inventoryMarker;

	Entity tooltipText;
	Entity notificationLines[3];

	float fpsCheckTime;
	Uint32 framesCounted;
	std::vector<std::string> notifications;
	float notificationTimer;

	Entity createTextEntity(int x, int y, SDL_Color color = { 0xFF, 0xFF, 0xFF }, Vec2 pivot = Vec2(0, 0), std::string text = "");
	void updateInventoryIndex(int index);
};