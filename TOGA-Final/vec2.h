#pragma once
#include <cmath>
#include <SDL_stdinc.h>
#include <iostream>

struct Vec2Int;

// This class started off as just a container for x and y but grew as I needed to add functionality
struct Vec2 {
	float x, y;

	Vec2() : x(0), y(0) {}
	Vec2(float x, float y) : x(x), y(y) {}
	//Vec2(Vec2Int v) : Vec2(v.x, v.y) {}

	float magnitude() const {
		return sqrt(pow(x, 2) + pow(y, 2));
	}

	static float distance(Vec2 a, Vec2 b) {
		return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
	}

	static float angle(Vec2 a, Vec2 b) {
		float dotProduct = a.x * b.x + a.y * a.y;
		float rad = acos(dotProduct / (a.magnitude() * b.magnitude()));
		float deg = rad * 180 / (float) M_PI;
		return deg;
	}

	bool operator ==(const Vec2& v) const {
		return (x == v.x) && (y == v.y);
	}

	bool operator !=(const Vec2& v) const {
		return (x != v.x) || (y != v.y);
	}

	// These needed to be added to use Vec2 as a key in a map
	// It is important that the values are unique rather acually making sense with the comparison
	bool operator <(const Vec2& v) const {
		float m1 = magnitude();
		float m2 = v.magnitude();
		if (m1 != m2) return m1 < m2;
		else return x < v.x;
	}

	bool operator >(const Vec2& v) const {
		float m1 = magnitude();
		float m2 = v.magnitude();
		if (m1 != m2) return m1 < m2;
		else return x < v.x;
	}

	Vec2 operator +(const Vec2& v) {
		Vec2 o = *this;
		o.x += v.x;
		o.y += v.y;
		return o;
	}

	Vec2 operator -(const Vec2& v) {
		Vec2 o = *this;
		o.x -= v.x;
		o.y -= v.y;
		return o;
	}

	Vec2 operator *(float f) {
		Vec2 o = *this;
		o.x *= f;
		o.y *= f;
		return o;
	}

	Vec2 operator /(float f) {
		Vec2 o = *this;
		o.x /= f;
		o.y /= f;
		return o;
	}

	Vec2& operator *=(const float f) {
		//return Vec2(x * f, y * f);
		x = x * f;
		y = y * f;
		return *this;
	}

	void print() const {
		printf("(%f, %f)\n", x, y);
	}

	// Taken from the Unity engine https://nathanfmp.files.wordpress.com/2012/01/horror_video_games-_essays_on_the_fusion_of_fear_and_play_by_bernard_perron-_clive_barker.pdf
	static Vec2 moveTowards(Vec2 current, Vec2 target, float maxDistanceDelta) {
		Vec2 a = target - current;
		float magnitude = a.magnitude();
		if (magnitude <= maxDistanceDelta || magnitude == 0.0f) {
			return target;
		}
		return current + a / magnitude * maxDistanceDelta;
	}

	operator Vec2Int() const;
};

struct Vec2Int {
	int x, y;

	Vec2Int() : x(0), y(0) {}
	Vec2Int(int x, int y) : x(x), y(y) {}
	Vec2Int(Vec2 v) : Vec2Int((int) v.x, (int) v.y) {}
	
	float magnitude() const {
		return (float) sqrt(pow(x, 2) + pow(y, 2));
	}

	static float distance(Vec2Int a, Vec2Int b) {
		return (float) sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
	}

	static float angle(Vec2Int a, Vec2Int b) {
		float dotProduct = (float) (a.x * b.x + a.y * a.y);
		float rad = acos(dotProduct / (a.magnitude() * b.magnitude()));
		float deg = rad * 180.0f / (float) M_PI;
		return deg;
	}

	bool operator ==(const Vec2Int& v) const {
		return (x == v.x) && (y == v.y);
	}

	bool operator !=(const Vec2Int& v) const {
		return (x != v.x) || (y != v.y);
	}

	bool operator <(const Vec2Int& v) const {
		float m1 = magnitude();
		float m2 = v.magnitude();
		if (m1 != m2) return m1 < m2;
		else return x < v.x;
	}

	bool operator >(const Vec2Int& v) const {
		float m1 = magnitude();
		float m2 = v.magnitude();
		if (m1 != m2) return m1 > m2;
		else return x > v.x;
	}

	Vec2Int operator *=(float f) const {
		return Vec2Int(x * f, y * f);
	}

	Vec2Int operator +(const Vec2Int& v) {
		Vec2Int o = *this;
		o.x += v.x;
		o.y += v.y;
		return o;
	}

	Vec2Int operator -(const Vec2Int& v) {
		Vec2Int o = *this;
		o.x -= v.x;
		o.y -= v.y;
		return o;
	}

	Vec2Int operator *(float f) {
		Vec2Int o = *this;
		o.x *= f;
		o.y *= f;
		return o;
	}

	Vec2Int operator /(float f) {
		Vec2Int o = *this;
		o.x /= f;
		o.y /= f;
		return o;
	}

	void print() const {
		printf("(%d, %d)\n", x, y);
	}

	operator Vec2() const {
		return Vec2(x, y);
	}
};