#pragma once

#include "component.h"

struct TransformComponent : public Component {
	float x, y;
	float rot;
	float scaleX, scaleY;
};