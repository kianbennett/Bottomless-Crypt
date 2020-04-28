#pragma once

// Using a 1d variable sized array to store a 2d matrix guarantees the data will be contiguous in memory
template <typename T>
class Matrix {
public:
	Matrix() : array(nullptr), width(0), height(0) {}
	Matrix(int width, int height) : width(width), height(height) {
		array = new T[width * height];
	}

	// Return reference so it can be mutated
	T& get(int x, int y) {
		return array[index(x, y)];
	}

	T& get(Vec2Int pos) {
		return get(pos.x, pos.y);
	}

	void set(int x, int y, T element) {
		array[index(x, y)] = element;
	}

	void set(Vec2Int pos, T element) {
		array[index(pos.x, pos.y)] = element;
	}

	bool isInsideMatrix(int x, int y) {
		return x >= 0 && x < width && y >= 0 && y < height;
	}

	int getWidth() {
		return width;
	}

	int getHeight() {
		return height;
	}

private:
	T* array;
	int width, height;

	size_t index(int x, int y) {
		return y * width + x;
	}
};