#include "ResourceManager.h"

// Initialise static variable
map<string, Shader> ResourceManager::shaders;

Shader* ResourceManager::loadShader(string name, const string vertexPath, const string fragmentPath) {
	Shader shader = Shader(vertexPath, fragmentPath);
	shaders.insert(pair<string, Shader>(name, shader));
	return &shader;
}

Shader* ResourceManager::getShader(string name) {
	return &(shaders[name]);
}

void ResourceManager::clearAll() {
	for (auto i : shaders) {
		glDeleteProgram(i.second.ID);
	}
}