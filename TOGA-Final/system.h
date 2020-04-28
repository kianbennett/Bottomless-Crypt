#pragma once

#include <set>
#include <type_traits>
#include <functional>
#include "entity.h"

//struct comp {
//	bool operator() (const Entity& a, const Entity& b) const {
//		return a < b;
//	}
//};

class System {
public:
	// Set is a list of unique entries so no need to check if already exists when adding/removing
	std::set<Entity> entities;
	// Store which components are needed for each system in a bitset
	// If bit at index of component id is 1 then the system requires that component
	Signature signature;

	virtual void init() = 0;
	virtual void update(float dt) = 0;
	virtual void dealloc() = 0;
	virtual void onEntitiesChanged() {};
};