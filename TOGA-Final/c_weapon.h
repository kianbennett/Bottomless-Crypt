#pragma once

#include <vector>

enum class WeaponProperty {
	Heavy, Light, TwoHanded
};

enum class DamageType {
	None, Silver, Fire, Ice, Poison
};

struct WeaponComponent {

	std::vector<WeaponProperty> properties;
	int damageCount; // number of "dice" to "roll"
	int damageSize; // number of "sides" to each "dice"
	DamageType damageType;
};