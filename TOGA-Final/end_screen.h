#pragma once

#include "ecs.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

extern TTF_Font* font;

class EndScreen {
public:
	EndScreen() {}

	void init() {
		gameOverText = ECS::createEntity();
		ECS::addComponent(gameOverText, TransformComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100));
		ECS::addComponent(gameOverText, UITextComponent("GAME OVER", { 0XFF, 0XAA, 0XAA }, font, Vec2(0.5f, 0.5f)));

		goldText = ECS::createEntity();
		ECS::addComponent(goldText, TransformComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 60));
		ECS::addComponent(goldText, UITextComponent("Final score:", { 0XFF, 0XFF, 0XFF }, font, Vec2(0.5f, 0.5f)));

		goldValueText = ECS::createEntity();
		ECS::addComponent(goldValueText, TransformComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 35));
		ECS::addComponent(goldValueText, UITextComponent("3205", { 252, 192, 71 }, font, Vec2(0.5f, 0.5f)));

		restartText = ECS::createEntity();
		ECS::addComponent(restartText, TransformComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 20));
		ECS::addComponent(restartText, UITextComponent("[Press space to restart]", { 0xFF, 0xFF, 0xFF }, font, Vec2(0.5f, 0.5f)));
	}

	void setActive(bool active) {
		ECS::entityManager->setActive(gameOverText, active);
		ECS::entityManager->setActive(goldText, active);
		ECS::entityManager->setActive(goldValueText, active);
		ECS::entityManager->setActive(restartText, active);
	}

private:
	Entity gameOverText;
	Entity goldText, goldValueText;
	Entity restartText;
};