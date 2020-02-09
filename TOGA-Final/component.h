#pragma once

#include "entity.h"

struct Component {
	Entity::Id& id;
	Component(Entity::Id& id) : id(id) {}
};

//class Component {
//public:
//	Component(Entity::Id& id) : id(id) {}
//
//	Entity::Id& ownerId() {
//		return id;
//	}
//
//private:
//	Entity::Id& id;
//};