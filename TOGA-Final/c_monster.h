#pragma once

#include "ecs.h"

struct MonsterComponent {

	int strength;

	MonsterComponent(int strength) : strength(strength) {}
};