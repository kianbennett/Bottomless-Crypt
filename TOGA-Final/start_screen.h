#pragma once

#include "ecs.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

extern TTF_Font* font;
extern TTF_Font* titleFont;

class StartScreen {
public:
	StartScreen() {}

	void init() {
		titleText = ECS::createEntity();
		ECS::addComponent(titleText, TransformComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 40));
		ECS::addComponent(titleText, UITextComponent("BOTTOMLESS CRYPT", { 171, 161, 199 }, titleFont, Vec2(0.5f, 0.5f)));

		startText = ECS::createEntity();
		ECS::addComponent(startText, TransformComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 20));
		ECS::addComponent(startText, UITextComponent("[Press space to begin]", { 0xFF, 0xFF, 0xFF }, font, Vec2(0.5f, 0.5f)));
	}

	void setActive(bool active) {
		ECS::entityManager->setActive(titleText, active);
		ECS::entityManager->setActive(startText, active);
	}

private:
	Entity titleText;
	Entity startText;
};