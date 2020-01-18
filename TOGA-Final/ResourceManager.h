#pragma once

#include <map>
#include <string>

#include "Shader.h"

using namespace std;

class ResourceManager {
public:
	static Shader* loadShader(string name, const string vertexPath, const string fragmentPath);
	static Shader* getShader(string name);
	static void clearAll();

private:
	// Make the constructor private to prevent any objects of the class being created
	ResourceManager() {} 

	static map<string, Shader> shaders;
};