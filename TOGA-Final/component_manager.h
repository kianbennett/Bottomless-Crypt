#pragma once

#include <vector>
#include <unordered_map>
#include "transform_component.h"

class ComponentManager {

	template<typename T>
	struct ComponentList {
		std::unordered_map<Entity::Id, T&> map;

		void add(Entity::Id id, T& component) {
			map[id] = component;
		}

		T& get(Entity::Id id) {
			return &map[id];
		}

		void remove(Entity::Id id) {
			map.erase(id)
		}
	};

public:
	ComponentManager() {}
	ComponentList<TransformComponent> transform;
};