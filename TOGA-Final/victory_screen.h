#pragma once

#include "ecs.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

extern TTF_Font* font;

class VictoryScreen {
public:
	VictoryScreen() {}

	void init() {
		victoryText = ECS::createEntity();
		ECS::addComponent(victoryText, TransformComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 100));
		ECS::addComponent(victoryText, UITextComponent("CONGRATULATIONS! You have ascended!", { 0XFF, 0XAA, 0XAA }, font, Vec2(0.5f, 0.5f)));

		goldText = ECS::createEntity();
		ECS::addComponent(goldText, TransformComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 60));
		ECS::addComponent(goldText, UITextComponent("Final score:", { 0XFF, 0XFF, 0XFF }, font, Vec2(0.5f, 0.5f)));

		goldValueText = ECS::createEntity();
		ECS::addComponent(goldValueText, TransformComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 35));
		ECS::addComponent(goldValueText, UITextComponent("3205", { 252, 192, 71 }, font, Vec2(0.5f, 0.5f)));

		restartText = ECS::createEntity();
		ECS::addComponent(restartText, TransformComponent(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 20));
		ECS::addComponent(restartText, UITextComponent("[Press space to replay]", { 0xFF, 0xFF, 0xFF }, font, Vec2(0.5f, 0.5f)));
	}

	void setActive(bool active) {
		ECS::entityManager->setActive(victoryText, active);
		ECS::entityManager->setActive(goldText, active);
		ECS::entityManager->setActive(goldValueText, active);
		ECS::entityManager->setActive(restartText, active);
	}

	void setScore(int score) {
		ECS::getComponent<UITextComponent>(goldValueText).text = std::to_string(score);
		ECS::getComponent<UITextComponent>(goldValueText).textChanged = true;
	}

private:
	Entity victoryText;
	Entity goldText, goldValueText;
	Entity restartText;
};