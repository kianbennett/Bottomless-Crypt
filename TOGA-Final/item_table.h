#pragma once

#include "level.h"

class ItemTable {
public:
	static Entity getItem(Level* level) {
		const std::vector<ItemComponent> itemList = {
			ItemComponent("sharp stick", ItemType::Weapon, 1, 0),
			ItemComponent("wooden club", ItemType::Weapon, 2, 4),
			ItemComponent("wooden sword", ItemType::Weapon, 3, 12),
			ItemComponent("iron sword", ItemType::Weapon, 6, 30),
			ItemComponent("iron mace", ItemType::Weapon, 6, 30),
			ItemComponent("iron axe", ItemType::Weapon, 7, 35),
			ItemComponent("bronze sword", ItemType::Weapon, 9, 60),
			ItemComponent("steel sword", ItemType::Weapon, 11, 100), // 7
			ItemComponent("steel mace", ItemType::Weapon, 11, 100),
			ItemComponent("steel axe", ItemType::Weapon, 12, 112),
			ItemComponent("sharp steel sword", ItemType::Weapon, 13, 160),
			ItemComponent("adamantium sword", ItemType::Weapon, 18, 350),
			ItemComponent("adamantium axe", ItemType::Weapon, 20, 700),

			ItemComponent("small health potion", ItemType::Potion, 10, 40), // 13
			ItemComponent("medium health potion", ItemType::Potion, 20, 80),
			ItemComponent("large health potion", ItemType::Potion, 40, 120),
			ItemComponent("beef jerky", ItemType::Potion, 5, 5),
			ItemComponent("mouldy bread loaf", ItemType::Potion, 1, 2),
			ItemComponent("roast chicken", ItemType::Potion, 8, 5),
			ItemComponent("dead rat", ItemType::Potion, 2, 0),

			ItemComponent("old boot", ItemType::Misc, 0, 0), // 20
			ItemComponent("empty cup", ItemType::Misc, 0, 0),
			ItemComponent("goblin skull", ItemType::Misc, 0, 0),
			ItemComponent("gold pendant", ItemType::Misc, 0, 400),
			ItemComponent("ruby gem", ItemType::Misc, 0, 820),
			ItemComponent("emerald gem", ItemType::Misc, 0, 860),
			ItemComponent("topaz gem", ItemType::Misc, 0, 900),
			ItemComponent("diamond gem", ItemType::Misc, 0, 1200),
		};

		std::vector<int> itemIds;

		itemIds.insert(itemIds.end(), 5, 0); // Adds 0 to the end of the vector 5 times
		itemIds.insert(itemIds.end(), 6, 1);
		itemIds.insert(itemIds.end(), 6, 2);
		itemIds.insert(itemIds.end(), 2, 3);
		itemIds.insert(itemIds.end(), 2, 4);
		itemIds.insert(itemIds.end(), 2, 5);
		itemIds.insert(itemIds.end(), 2, 13);
		itemIds.insert(itemIds.end(), 2, 16);
		itemIds.insert(itemIds.end(), 2, 17);
		itemIds.insert(itemIds.end(), 2, 18);
		itemIds.insert(itemIds.end(), 2, 19);
		itemIds.insert(itemIds.end(), 1, 20);
		itemIds.insert(itemIds.end(), 1, 21);
		itemIds.insert(itemIds.end(), 1, 22);

		if (level->depth > 2) {
			itemIds.insert(itemIds.end(), 4, 6);
			itemIds.insert(itemIds.end(), 4, 7);
			itemIds.insert(itemIds.end(), 4, 8);
			itemIds.insert(itemIds.end(), 4, 9);
			itemIds.insert(itemIds.end(), 8, 14);
			itemIds.insert(itemIds.end(), 3, 23);
		}

		if (level->depth > 5) {
			itemIds.insert(itemIds.end(), 4, 10);
			itemIds.insert(itemIds.end(), 4, 11);
			itemIds.insert(itemIds.end(), 4, 12);
			itemIds.insert(itemIds.end(), 8, 15);
			itemIds.insert(itemIds.end(), 2, 24);
			itemIds.insert(itemIds.end(), 2, 25);
			itemIds.insert(itemIds.end(), 2, 26);
			itemIds.insert(itemIds.end(), 2, 27);
		}

		int n = itemIds[rand() % itemIds.size()];

		Entity entity = ECS::createEntity();
		ECS::addComponent(entity, itemList[n]);

		return entity;
	}
};