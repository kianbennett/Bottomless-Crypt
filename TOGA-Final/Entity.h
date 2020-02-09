#pragma once

class Entity {
public:
	// Entity id consists of an index (used to look up entities in an array) and a generation
	// Entity list is stored in EntityManager as an array of generations
	// When an entity is destroyed that generation is incremented
	// To tell if an entity is alive match it's generation to the generation in EntityManager array
	struct Id {
		unsigned index, generation;

		Id() : index(0), generation(0) {}
		Id(unsigned index, unsigned generation) : index(index), generation(generation) {}
	};

	Entity() {}
	Entity(Id id) : id(id) {}
	
	const Id& getId() {
		return id;
	}

	bool isAlive() {
		// EntityManager isAlive
		return true;
	}
	void destroy() {
		// EntityManager destroy
	}

private:
	Id id;
};