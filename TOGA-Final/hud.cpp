#include "hud.h"

HUD::HUD() {
	timeText = createTextEntity(5, 5);
	infoText = createTextEntity(5, 25);
	levelText = createTextEntity(5, 45);
	goldText = createTextEntity(5, 85, { 0xFF, 0xEE, 0xAA });
	healthText = createTextEntity(5, 105, { 0xFF, 0xDD, 0xDD });
	tooltipText = createTextEntity(SCREEN_WIDTH - 5, SCREEN_HEIGHT - 25, { 0xFF, 0xFF, 0xFF }, Vec2Int(1, 0));

	inventoryText = createTextEntity(SCREEN_WIDTH - 5, 5, { 0xFF, 0xFF, 0xFF }, Vec2Int(1, 0));
	inventoryDivider1 = createTextEntity(SCREEN_WIDTH - 5, 15, { 0x88, 0x88, 0x88 }, Vec2Int(1, 0), "__________________________________");
	inventoryHeader =	createTextEntity(SCREEN_WIDTH - 5, 34, { 0xFF, 0xFF, 0xFF }, Vec2Int(1, 0), " Item                 |  E  |  G  ");
	inventoryDivider2 = createTextEntity(SCREEN_WIDTH - 5, 37, { 0x88, 0x88, 0x88 }, Vec2Int(1, 0), "__________________________________");

	inventoryMarker = createTextEntity(SCREEN_WIDTH - 386, 62, { 0xFF, 0xFF, 0xFF }, Vec2(), ">");

	for (int i = 0; i < 8; i++) {
		inventoryLines[i].name = createTextEntity(SCREEN_WIDTH - 368, 62 + 22 * i, { 0xFF, 0xFF, 0xFF });
		inventoryLines[i].strength = createTextEntity(SCREEN_WIDTH - 98, 62 + 22 * i, { 0xFF, 0xFF, 0xFF }, Vec2(0.5f, 0));
		inventoryLines[i].gold = createTextEntity(SCREEN_WIDTH - 32, 62 + 22 * i, { 0xFF, 0xFF, 0xFF }, Vec2(0.5f, 0));
	}

	for (int i = 0; i < 3; i++) {
		SDL_Color colour = { 0xFF, 0xFF, 0xFF, 0xFF };
		if(i == 2) colour.a = 0XAA;
		notificationLines[i] = createTextEntity(5, SCREEN_HEIGHT - 25 - i * 25);
	}

	fpsCheckTime = 0;
	framesCounted = 0;
	notificationTimer = 0;
}

void HUD::update(float deltaTime) {
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

void HUD::setActive(bool active) {
	ECS::entityManager->setActive(levelText, active);
	ECS::entityManager->setActive(goldText, active);
	ECS::entityManager->setActive(healthText, active);
	ECS::entityManager->setActive(inventoryText, active);
	ECS::entityManager->setActive(tooltipText, active);
	ECS::entityManager->setActive(notificationLines[0], active);
	ECS::entityManager->setActive(notificationLines[1], active);
	ECS::entityManager->setActive(notificationLines[2], active);
	ECS::entityManager->setActive(inventoryHeader, active && showInventory);
	ECS::entityManager->setActive(inventoryDivider1, active && showInventory);
	ECS::entityManager->setActive(inventoryDivider2, active && showInventory);
	if (!active) {
		ECS::entityManager->setActive(inventoryMarker, false);
		for (int i = 0; i < 8; i++) {
			inventoryLines[i].setActive(false);
		}
	}
}

void HUD::updatePlayerStats(CharacterComponent character) {
	ECS::getComponent<UITextComponent>(goldText).text = "gold: " + std::to_string(character.gold);
	ECS::getComponent<UITextComponent>(goldText).textChanged = true;

	std::string healthString = "health: " + std::to_string(character.healthCurrent) + " / " + std::to_string(character.healthMax);
	ECS::getComponent<UITextComponent>(healthText).text = healthString;
	ECS::getComponent<UITextComponent>(healthText).textChanged = true;
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

void HUD::clearNotifications() {
	notifications.clear();
}

void HUD::updateInventory() {
	CharacterComponent character = ECS::getComponent<CharacterComponent>(level->player);
	ECS::getComponent<UITextComponent>(inventoryText).text = "Inventory (I) " +
		std::to_string(character.inventory.size()) + " / " + std::to_string(character.inventoryLimit);
	ECS::getComponent<UITextComponent>(inventoryText).textChanged = true;

	ECS::entityManager->setActive(inventoryHeader, showInventory);
	ECS::entityManager->setActive(inventoryDivider1, showInventory);
	ECS::entityManager->setActive(inventoryDivider2, showInventory);
	ECS::entityManager->setActive(inventoryMarker, showInventory && character.inventory.size() > 0);

	for (int i = 0; i < 8; i++) {
		bool show = showInventory && i < character.inventory.size();
		inventoryLines[i].setActive(show);

		if (i < character.inventory.size()) {
			ItemComponent item = ECS::getComponent<ItemComponent>(character.inventory[i]);
			inventoryLines[i].setValues(item, character.equippedWeapon != nullptr && character.inventory[i] == *character.equippedWeapon);
		}
	}

	updateInventoryIndex(inventoryIndex);
}

void HUD::nextInventoryItem() {
	CharacterComponent character = ECS::getComponent<CharacterComponent>(level->player);
	inventoryIndex++;
	if (inventoryIndex >= character.inventory.size()) {
		inventoryIndex = 0;
	}
	updateInventoryIndex(inventoryIndex);
}

void HUD::prevInventoryItem() {
	CharacterComponent character = ECS::getComponent<CharacterComponent>(level->player);
	inventoryIndex--;
	if (inventoryIndex < 0) {
		inventoryIndex = character.inventory.size() - 1;
	}
	updateInventoryIndex(inventoryIndex);
}

void HUD::updateInventoryIndex(int index) {
	CharacterComponent character = ECS::getComponent<CharacterComponent>(level->player);
	if (index >= character.inventory.size()) index = character.inventory.size() - 1;
	inventoryIndex = index;
	TransformComponent& transform = ECS::getComponent<TransformComponent>(inventoryMarker);
	transform.setPos(Vec2(transform.x, 62 + 22 * index));
}

Entity HUD::createTextEntity(int x, int y, SDL_Color color, Vec2 pivot, std::string text) {
	Entity entity = ECS::createEntity();
	ECS::addComponent(entity, TransformComponent(x, y));
	ECS::addComponent(entity, UITextComponent(text, color, font, pivot));
	return entity;
}