#pragma once

#include "EntityPlayer.h"

class PlayerInput {
public:
	PlayerInput(EntityPlayer* player) : player(player) {}

	void onKeyPress(int key);
	void onKeyRelease(int key);
private:
	EntityPlayer* player;
};