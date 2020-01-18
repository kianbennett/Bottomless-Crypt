#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ResourceManager.h"

class Renderer {
public:
	const float* vertexData;
	int vertexDataLength;
	vec4 colour;

	Renderer(const float vertexData[], int length, vec4 colour);
	Renderer(const float vertexData[], int length, vec4 colour, std::string shader);
	~Renderer();

	void render(mat4 model);

private:
	Shader* shader;
	unsigned int VAO;
	void initVertexData();
};

class QuadRenderer : public Renderer {
public:
	QuadRenderer(vec4 colour) : Renderer(quad, 18, colour) {}
	QuadRenderer(vec4 colour, std::string shader) : Renderer(quad, 18, colour, shader) {}

private:
	// Static so new memory isn't allocated for every instance of the class
	constexpr static float quad[] = {
		-0.5f,  0.5f, 0.0f, // top left
		 0.5f, -0.5f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, // bottom left

		-0.5f, 0.5f, 0.0f, // top left
		 0.5f, 0.5f, 0.0f, // top right
		 0.5f, -0.5f, 0.0f, // bottom right
	};
};