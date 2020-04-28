#pragma once

#include "types.h"

struct Entity {
	// This : syntax is used to specify the bit size of the variable
	unsigned index : 20;
	unsigned generation : 12;

	Entity() : index(0), generation(0) {}
	Entity(unsigned index, unsigned generation) : index(index), generation(generation) {}

	// When casting to an integer return value() so it can be used for set/map key comparisons
	operator unsigned() const {
		return value();
	}

	// Returns the value of the entity as a 32 bit integer
	unsigned value() const {
		return (generation << 12) | index;
	}
};

// Define hash function so Entity can be used as a key in unordered_map
namespace std {
	template <>
	struct hash<Entity> {
		std::size_t operator()(const Entity& entity) const {
			return entity.value();
		}
	};
}