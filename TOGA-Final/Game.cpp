#include "Game.h"

Game::Game(int width, int height) : width(width), height(height), player() {
}

Game::~Game() {
	for (Entity* entity : entities) {
		delete entity;
	}
}

void Game::init() {
	//Entity* entity = new Entity(300.0f, 300.0f, 100.0f, 50.0f, vec4(1.0f));
	player = new EntityPlayer(300.0f, 300.0f, 50.0f, 50.0f, vec4(1.0f));
	entities.push_back(player);
}

void Game::update(float dt) {
	for (Entity* entity : entities) {
		entity->update(dt);
	}
}

void Game::render() {
	for (Entity* entity : entities) {
		entity->render();
	}
}

void Game::onKeyDown(int key) {
	if (key == GLFW_KEY_W) player->moveUp = true;
	if (key == GLFW_KEY_S) player->moveDown = true;
	if (key == GLFW_KEY_A) player->moveLeft = true;
	if (key == GLFW_KEY_D) player->moveRight = true;
}

void Game::onKeyUp(int key) {
	if (key == GLFW_KEY_W) player->moveUp = false;
	if (key == GLFW_KEY_S) player->moveDown = false;
	if (key == GLFW_KEY_A) player->moveLeft = false;
	if (key == GLFW_KEY_D) player->moveRight = false;
}