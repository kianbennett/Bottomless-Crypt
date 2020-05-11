#pragma once

#include "level.h"

class ItemTable {
public:
	static Entity getItem(Level* level) {
		const std::vector<ItemComponent> itemList = {
			ItemComponent("Sharp Stick", ItemType::Weapon, 1, 0),
			ItemComponent("Wooden Club", ItemType::Weapon, 2, 4),
			ItemComponent("Wooden Sword", ItemType::Weapon, 3, 12),
			ItemComponent("Iron Sword", ItemType::Weapon, 6, 30),
			ItemComponent("Iron Mace", ItemType::Weapon, 18, 800),
			ItemComponent("Iron Axe", ItemType::Weapon, 18, 800),
			ItemComponent("Bronze Sword", ItemType::Weapon, 8, 60),
			ItemComponent("Steel Sword", ItemType::Weapon, 11, 100), // 7
			ItemComponent("Steel Mace", ItemType::Weapon, 18, 800),
			ItemComponent("Steel Axe", ItemType::Weapon, 18, 800),
			ItemComponent("Sharp Steel Sword", ItemType::Weapon, 12, 120),
			ItemComponent("Adamantium Axe", ItemType::Weapon, 18, 800),
			ItemComponent("Adamantium Sword", ItemType::Weapon, 18, 800),

			ItemComponent("Small Health Potion", ItemType::Potion, 10, 40), // 13
			ItemComponent("Medium Health Potion", ItemType::Potion, 20, 80),
			ItemComponent("Large Health Potion", ItemType::Potion, 40, 120),
			ItemComponent("Beef Jerky", ItemType::Potion, 5, 5),
			ItemComponent("Mouldy Bread Loaf", ItemType::Potion, 1, 2),
			ItemComponent("Roast Chicken", ItemType::Potion, 8, 5),
			ItemComponent("Dead Rat", ItemType::Potion, 2, 0),

			ItemComponent("Old Boot", ItemType::Misc, 0, 0), // 20
			ItemComponent("Empty Cup", ItemType::Misc, 0, 0),
			ItemComponent("Goblin Skull", ItemType::Misc, 0, 0),
			ItemComponent("Gold Pendant", ItemType::Misc, 0, 400),
			ItemComponent("Ruby Gem", ItemType::Misc, 0, 820),
			ItemComponent("Emerald Gem", ItemType::Misc, 0, 860),
			ItemComponent("Topaz Gem", ItemType::Misc, 0, 900),
			ItemComponent("Diamond Gem", ItemType::Misc, 0, 1200),
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

		int n = itemIds[rand() % itemIds.size()];

		Entity entity = ECS::createEntity();
		ECS::addComponent(entity, itemList[n]);

		return entity;
	}
};