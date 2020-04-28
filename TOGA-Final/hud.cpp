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

	tooltipText = ECS::createEntity();
	ECS::addComponent(tooltipText, TransformComponent(SCREEN_WIDTH - 5, SCREEN_HEIGHT - 25));
	ECS::addComponent(tooltipText, UITextComponent("", { 0xFF, 0xFF, 0xFF }, font, Vec2Int(1, 0)));

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

		std::string levelString = "level: " + std::to_string(level->depth + 1);
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

void HUD::setTooltip(std::string text) {
	ECS::getComponent<UITextComponent>(tooltipText).text = text;
	ECS::getComponent<UITextComponent>(tooltipText).textChanged = true;
}

void HUD::addNotification(std::string text) {
	notificationTimer = 3;
	notifications.push_back(text);
	if (notifications.size() > 3) {
		notifications.erase(notifications.begin());
	}
}