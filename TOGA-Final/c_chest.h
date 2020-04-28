#pragma once

#include "components.h"

struct ChestComponent {

	std::vector<ItemComponent> inventory;
	int gold;
	bool opened;

	ChestComponent(int gold) : gold(gold), opened(false) {}
};