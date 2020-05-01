#pragma once

#include "level.h"

class ItemTable {
public:

	static Entity getItem(Level* level) {
		std::vector<int> itemIds;

		itemIds.insert(itemIds.end(), 5, 0); // Adds 0 to the end of the vector 5 times
		itemIds.insert(itemIds.end(), 4, 1);
		itemIds.insert(itemIds.end(), 2, 2);

		int n = itemIds[rand() % itemIds.size()];

		switch (n) {
		case 0:
			return sword(level);
		}

		return sword(level);
	}

	static Entity sword(Level* level) {
		Entity entity = ECS::createEntity();
		ECS::addComponent(entity, ItemComponent("Sword", 30, 10));
		ECS::addComponent(entity, WeaponComponent(1, 6));
		return entity;
	}
};