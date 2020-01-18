#pragma once

#include <vector>
#include "Entity.h"
#include "EntityPlayer.h"

class Game {
public:
	int width, height;
	vector<Entity*> entities;

	Game(int width, int height);
	~Game();

	void init();
	void update(float dt);
	void render();

	void onKeyDown(int key);
	void onKeyUp(int key);

private:
	EntityPlayer* player;
};