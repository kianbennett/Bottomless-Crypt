#pragma once

#include <iostream>
#include <glm/glm.hpp>

using namespace glm;

class Camera {
public:
	vec2 pos;

	Camera() {
		pos = vec3(0.0f, 0.0f, 3.0f);
	}
};