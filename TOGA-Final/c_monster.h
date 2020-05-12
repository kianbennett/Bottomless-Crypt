#pragma once

#include "ecs.h"

struct MonsterComponent {

	int strength;
	int turnsFollowed;

	MonsterComponent(int strength) : strength(strength), turnsFollowed(0) {}
};