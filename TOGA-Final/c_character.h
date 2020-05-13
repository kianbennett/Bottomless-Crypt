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

	CharacterComponent() : equippedWeapon(nullptr), inventoryLimit(0), healthMax(0), healthCurrent(0), gold(0) {}

	CharacterComponent(std::string name, int health, int gold) : name(name), 
		healthMax(health), healthCurrent(health), gold(gold), inventoryLimit(8), equippedWeapon(nullptr) {}
};