#pragma once

#include <vector>
#include <string>
#include "c_item.h"

struct CharacterComponent {

	std::string name;
	std::vector<Entity> inventory;
	int inventoryLimit;
	int healthMax, healthCurrent;
	int gold;
	int strength, defence;

	CharacterComponent(std::string name, int health, int gold, int strength, int defence) : name(name), 
		healthMax(health), healthCurrent(health), gold(gold), strength(strength), defence(defence), inventoryLimit(8) {}
};