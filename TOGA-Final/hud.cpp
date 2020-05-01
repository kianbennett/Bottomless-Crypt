#include "hud.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

HUD::HUD() {
	timeText = ECS::createEntity();
	ECS::addComponent(timeText, TransformComponent(5, 5));
	ECS::addComponent(timeText, UITextComponent("", { 0xFF, 0xFF, 0xFF }, font));

	infoText = ECS::createEntity();
	ECS::addComponent(infoText, TransformComponent(5, 25));
	ECS::addComponent(infoText, UITextComponent("", { 0xFF, 0xFF, 0xFF }, font));

	levelText = ECS::createEntity();
	ECS::addComponent(levelText, TransformComponent(5, 45));
	ECS::addComponent(levelText, UITextComponent("", { 0xFF, 0xFF, 0xFF }, font));

	goldText = ECS::createEntity();
	ECS::addComponent(goldText, TransformComponent(5, 85));
	ECS::addComponent(goldText, UITextComponent("", { 0xFF, 0xEE, 0xAA }, font));

	healthText = ECS::createEntity();
	ECS::addComponent(healthText, TransformComponent(5, 105));
	ECS::addComponent(healthText, UITextComponent("", { 0xFF, 0xDD, 0xDD }, font));

	strengthText = ECS::createEntity();
	ECS::addComponent(strengthText, TransformComponent(5, 125));
	ECS::addComponent(strengthText, UITextComponent("", { 0xFF, 0xFF, 0xFF }, font));

	defenceText = ECS::createEntity();
	ECS::addComponent(defenceText, TransformComponent(5, 145));
	ECS::addComponent(defenceText, UITextComponent("", { 0xFF, 0xFF, 0xFF }, font));

	inventoryText = ECS::createEntity();
	ECS::addComponent(inventoryText, TransformComponent(SCREEN_WIDTH - 5, 5));
	ECS::addComponent(inventoryText, UITextComponent("", { 0xFF, 0xFF, 0xFF }, font, Vec2Int(1, 0)));

	tooltipText = ECS::createEntity();
	ECS::addComponent(tooltipText, TransformComponent(SCREEN_WIDTH - 5, SCREEN_HEIGHT - 25));
	ECS::addComponent(tooltipText, UITextComponent("", { 0xFF, 0xFF, 0xFF }, font, Vec2Int(1, 0)));

	for (int i = 0; i < 8; i++) {
		inventoryItems[i] = ECS::createEntity();
		ECS::addComponent(inventoryItems[i], TransformComponent(SCREEN_WIDTH - 5, 25 + 20 * i));
		ECS::addComponent(inventoryItems[i], UITextComponent("", { 0xFF, 0xFF, 0xFF }, font, Vec2Int(1, 0)));
	}

	for (int i = 0; i < 3; i++) {
		SDL_Color colour = { 0xFF, 0xFF, 0xFF, 0xFF };
		if(i == 2) colour.a = 0XAA;
		notificationLines[i] = ECS::createEntity();
		ECS::addComponent(notificationLines[i], TransformComponent(5, SCREEN_HEIGHT - 25 - i * 25));
		ECS::addComponent(notificationLines[i], UITextComponent("", colour, font));
	}

	fpsCheckTime = 0;
	framesCounted = 0;
	notificationTimer = 0;
}

void HUD::update(float deltaTime) {
	if (showDebug) {
		// Update FPS check
		fpsCheckTime += deltaTime;
		framesCounted++;
		float fpsCheckInterval = 1.0f / 20.0f;
		if (fpsCheckTime > fpsCheckInterval) {
			int fps = (int) (framesCounted / (fpsCheckTime));
			std::string fpsString = "fps: " + std::to_string(fps);
			ECS::getComponent<UITextComponent>(timeText).text = fpsString;
			ECS::getComponent<UITextComponent>(timeText).textChanged = true;
			fpsCheckTime = 0;
			framesCounted = 0;
		}

		// Update entity info text
		std::string entityInfoString = "entities: " + std::to_string(ECS::entityManager->getSize());
		ECS::getComponent<UITextComponent>(infoText).text = entityInfoString;
		ECS::getComponent<UITextComponent>(infoText).textChanged = true;

		std::string levelString = "dungeon level: " + std::to_string(level->depth + 1);
		ECS::getComponent<UITextComponent>(levelText).text = levelString;
		ECS::getComponent<UITextComponent>(levelText).textChanged = true;
	}

	if (notificationTimer > 0) {
		notificationTimer -= deltaTime;
		if (notificationTimer <= 0) {
			notifications.clear();
		}
	}

	for (int i = 0; i < 3; i++) {
		if (i < notifications.size()) {
			ECS::getComponent<UITextComponent>(notificationLines[i]).text = notifications[i];
		}
		else {
			ECS::getComponent<UITextComponent>(notificationLines[i]).text = "";
		}
		ECS::getComponent<UITextComponent>(notificationLines[i]).textChanged = true;
	}
}

void HUD::updateDebugInfo(bool showDebug) {
	//showDebugInfo = !showDebugInfo;
	ECS::entityManager->setActive(timeText, showDebug);
	ECS::entityManager->setActive(infoText, showDebug);
}

void HUD::updatePlayerStats(CharacterComponent character) {
	ECS::getComponent<UITextComponent>(goldText).text = "gold: " + std::to_string(character.gold);
	ECS::getComponent<UITextComponent>(goldText).textChanged = true;

	std::string healthString = "health: " + std::to_string(character.healthCurrent) + " / " + std::to_string(character.healthMax);
	ECS::getComponent<UITextComponent>(healthText).text = healthString;
	ECS::getComponent<UITextComponent>(healthText).textChanged = true;

	ECS::getComponent<UITextComponent>(strengthText).text = "strength: " + std::to_string(character.strength);
	ECS::getComponent<UITextComponent>(strengthText).textChanged = true;

	ECS::getComponent<UITextComponent>(defenceText).text = "defence: " + std::to_string(character.defence);
	ECS::getComponent<UITextComponent>(defenceText).textChanged = true;
}

void HUD::setTooltip(std::string text) {
	ECS::getComponent<UITextComponent>(tooltipText).text = text;
	ECS::getComponent<UITextComponent>(tooltipText).textChanged = true;
}

void HUD::addNotification(std::string text) {
	notificationTimer = 3;
	notifications.insert(notifications.begin(), text);
	if (notifications.size() > 3) {
		notifications.pop_back();
	}
}

void HUD::updateInventory(CharacterComponent character) {
	ECS::getComponent<UITextComponent>(inventoryText).text = "Inventory (I) " +
		std::to_string(character.inventory.size()) + " / " + std::to_string(character.inventoryLimit);
	ECS::getComponent<UITextComponent>(inventoryText).textChanged = true;

	for (int i = 0; i < 8; i++) {
		bool show = showInventory && i < character.inventory.size();
		ECS::entityManager->setActive(inventoryItems[i], show);

		if (i < character.inventory.size()) {
			ItemComponent item = ECS::getComponent<ItemComponent>(character.inventory[i]);
			ECS::getComponent<UITextComponent>(inventoryItems[i]).text = item.name;
			ECS::getComponent<UITextComponent>(inventoryItems[i]).textChanged = true;
		}
	}
}