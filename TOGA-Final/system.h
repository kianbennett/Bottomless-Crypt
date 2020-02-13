#pragma once

#include <set>
#include "entity.h"

class System {
public:
	// set is a list of unique entries so no need to check if already exists when adding/removing
	std::set<Entity> entities;
};