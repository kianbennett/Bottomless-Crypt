#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

class Transform {
public:
	vec2 pos;
	float rot;
	vec2 scale;

	Transform() : pos(), rot(0), scale(1.0f) {}
	Transform(float x, float y, float w, float h, float r) : pos(vec2(x, y)), rot(r), scale(w, h) {}

	void reset() {
		pos = vec2();
		rot = 0;
		scale = vec2();
	}

	glm::mat4 getTransformation() {
		// Identity matrix
		mat4 model = glm::mat4(1.0f);

		model = translate(model, vec3(pos, 0.0f));
		model = rotate(model, radians(rot), vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, vec3(scale, 1.0f));

		return model;
	}
};