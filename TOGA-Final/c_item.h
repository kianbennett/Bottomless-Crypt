#pragma once

#include <string>

enum class ItemType {
	Weapon, Potion, Misc
};

struct ItemComponent {

	std::string name;
	ItemType type;
	int strength, value;

	ItemComponent(std::string name, ItemType type, int strength, int value) : name(name), type(type), strength(strength), value(value) {}
};