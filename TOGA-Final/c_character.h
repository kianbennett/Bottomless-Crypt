#pragma once

#include <vector>
#include <string>
#include "c_item.h"

struct CharacterComponent {

	std::string name;
	std::vector<Entity> inventory;
	Entity* equippedWeapon; // use pointer so it can be set to null
	int inventoryLimit;
	int healthMax, healthCurrent;
	int gold;

	CharacterComponent() {}

	CharacterComponent(std::string name, int health, int gold) : name(name), 
		healthMax(health), healthCurrent(health), gold(gold), inventoryLimit(8), equippedWeapon(nullptr) {}
};