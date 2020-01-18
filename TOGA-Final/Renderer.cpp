#include "Renderer.h"

Renderer::Renderer(const float vertexData[], int length, vec4 colour) :
	Renderer(vertexData, length, colour, "default") {}

Renderer::Renderer(const float vertexData[], int length, vec4 colour, string shader) : 
		vertexData(vertexData), vertexDataLength(length), colour(colour), VAO(0) {
	this->shader = ResourceManager::getShader(shader);
	initVertexData();
}

Renderer::~Renderer() {
	glDeleteVertexArrays(1, &VAO);
}

void Renderer::initVertexData() {
	// Generate and bind a vertex array object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate a vertex buffer object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	// Bind this buffer to the GL_ARRAY_BUFFER target
	// Any buffer calls we make on the GL_ARRAY_BUFFER target will be used to configure the bound buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Copies vertex data into the buffer
	glBufferData(GL_ARRAY_BUFFER, vertexDataLength * sizeof(float), vertexData, GL_STATIC_DRAW);

	// Specify how OpenGL should interpret the vertex data (for each vertex attribute)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);

	// Unbind the buffers once we've stored the data in them
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteBuffers(1, &VBO);
}

void Renderer::render(mat4 model) {
	shader->use();
	shader->setMat4("model", model);
	shader->setVec4("colour", colour);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertexDataLength / 3);
	glBindVertexArray(0);
}