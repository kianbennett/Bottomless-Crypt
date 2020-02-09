#include <iostream>
#include "world.h"

using namespace std;

int main() {
	World::componentManager = new ComponentManager;
	World::entityManager = new EntityManager;

	World world;
	world.init();

	while (true) {
		world.update(0);
	}

	world.cleanup();
}