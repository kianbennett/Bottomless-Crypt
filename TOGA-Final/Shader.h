#pragma once

#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

class Shader {
public:
	// Program ID
	unsigned int ID;

	Shader() : ID(0) {};

	// Reads the shader from a file and builds it
	Shader(const string vertexPath, const string fragmentPath) {
		const char* vertexSource = read(vertexPath);
		const char* fragmentSource = read(fragmentPath);

		unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);

		// Compile vertex shader
		glShaderSource(vertex, 1, &vertexSource, NULL);
		glCompileShader(vertex);
		checkCompilerErrors(vertex, false);

		// Compile fragment shader
		glShaderSource(fragment, 1, &fragmentSource, NULL);
		glCompileShader(fragment);
		checkCompilerErrors(fragment, false);

		// Create and link the shader program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompilerErrors(ID, true);

		// Delete the shader objects as we no longed need them after linking into the program
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	// Activates the shader
	void use() {
		glUseProgram(ID);
	}
	// Utility functions to set uniforms
	void setBool(const string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setInt(const string& name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const string& name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setVec2(const string& name, const vec2& value) const {
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec2(const string& name, float x, float y) const {
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}
	void setVec3(const string& name, const vec3& value) const {
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec3(const string& name, float x, float y, float z) const {
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}
	void setVec4(const string& name, const vec4& value) const {
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setVec4(const string& name, float x, float y, float z, float w) const {
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}
	void setMat2(const string& name, const mat2& mat) const {
		glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	void setMat3(const string& name, const mat3& mat) const {
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	void setMat4(const string& name, const mat4& mat) const {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

private:
	// Reads the string from a text file
	char* read(const string filePath) const {
		// Create an ifstream file with the filename passed to the function
		ifstream file(filePath.c_str(), ios::in | ios::binary);

		// Check if a file was found
		if (!file) {
			cout << "Shader text file not found!" << endl;
			return 0;
		}

		// Get size of the file by moving to the end of it and seeing how far we've gone
		file.seekg(0, ios::end);
		int size = (int)file.tellg();

		// Check if file is empty
		if (!size) {
			cout << "Shader file is empty!" << endl;
		}

		// Container to hold the data, via dynamically-allocated memory
		char* data = 0;

		// Allocate memory for data
		data = new char[size + 1];
		// Move back to the start of the file
		file.seekg(0, ios::beg);
		// Read file
		file.read(data, size);

		// Null terminate char array so it knows it is the end
		data[size] = '\0';

		return data;
	}

	void checkCompilerErrors(unsigned int shader, bool isProgram) {
		int success;
		char infoLog[1024];

		if (isProgram) {
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				cout << "Shader program linking failed!\n" << infoLog << endl;
			}
		}
		else {
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				cout << "Vertex shader compilation failed!\n" << infoLog << endl;
			}
		}
	}
};