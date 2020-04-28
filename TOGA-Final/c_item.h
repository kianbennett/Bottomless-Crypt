#pragma once

#include <string>

struct ItemComponent {

	std::string name;
	int value;
	int weight;

	ItemComponent(std::string name, int value, int weight) : name(name), value(value), weight(weight) {}
};