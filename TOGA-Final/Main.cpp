#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Game.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "Entity.h"

using namespace std;
using namespace glm;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
GLFWwindow* initGLFW();
void initGLAD();
void initOpenGL(GLFWwindow* window);
void loadShaders();

float deltaTime = 0.0f, lastFrame = 0.0f;

Game game = Game(WINDOW_WIDTH, WINDOW_HEIGHT);
Camera camera = Camera();

int main() {
	GLFWwindow* window = initGLFW();
	initGLAD();
	loadShaders();
	initOpenGL(window);

	game.init();

	// While the window hasn't been closed
	while (!glfwWindowShouldClose(window)) {
		float time = glfwGetTime();
		deltaTime = time - lastFrame;
		lastFrame = time;

		game.update(deltaTime);

		// Clear the colour buffer
		glClearColor(0.5f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		game.render();

		// Swap the colour buffer that we have drawn into so it can be used to show output on the screen
		glfwSwapBuffers(window);
		// Check if any events have been triggered, update the window state and calls corresponding callbacks
		glfwPollEvents();
	}

	// Deletes all loaded resources (shaders etc)
	ResourceManager::clearAll();

	// Cleans up GLFW resources
	glfwTerminate();
	return 0;
}

void initOpenGL(GLFWwindow* window) {
	// Set size of rendering window
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	// Don't render reverse of faces
	glEnable(GL_CULL_FACE);
	// Enable blending between colours (transparency)
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	// Create an orthographic projection matrix and apply it to the shaders
	mat4 projection = ortho(0.0f, (float) WINDOW_WIDTH, (float) WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f);
	Shader* shaderDefault = ResourceManager::getShader("default");
	shaderDefault->use();
	shaderDefault->setMat4("projection", projection);
}

void loadShaders() {
	ResourceManager::loadShader("default", "shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");
}

// Initialises GLFW and returns the created window
GLFWwindow* initGLFW() {
	glfwInit();
	// Specify OpenGL version (3.3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Use core-profile (instead of immediate)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create the window object
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "TOGA Final", NULL, NULL);
	// Check window was created successfully
	if (window == NULL) {
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		exit(-1);
	}
	// Make the window's context the current OpenGL context
	glfwMakeContextCurrent(window);
	// Set window resize callback
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	// Set key callback
	glfwSetKeyCallback(window, keyCallback);

	// Centres the window (half the current monitor resolution)
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwSetWindowPos(window, mode->width / 2 - WINDOW_WIDTH / 2, mode->height / 2 - WINDOW_HEIGHT / 2);

	return window;
}

// Initialise GLAD (library for loading OpenGL function pointers)
void initGLAD() {
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		cout << "Failed to initialise GLAD" << endl;
		exit(-1);
	}
}

// Called when the window is resized
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	// Adjust the viewport to the new window size
	glViewport(0, 0, width, height);
}

// Called when an input key is pressed or released
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (key == GLFW_KEY_F2 && action == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (key == GLFW_KEY_F3 && action == GLFW_PRESS) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}

	if (action == GLFW_PRESS) game.onKeyDown(key);
	if (action == GLFW_RELEASE) game.onKeyUp(key);
}