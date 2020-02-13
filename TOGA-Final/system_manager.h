#pragma once

#include <unordered_map>
#include <bitset>
#include <memory>
#include "system.h"
#include "types.h"
#include "ecs.h"

class SystemManager {
public:
	// Registers a system in the map
	template<typename System>
	std::shared_ptr<System> registerSystem() {
		const char* typeName = typeid(System).name();

		if (hasSystemBeenRegistered(typeName)) {
			printf("System has already been registered\n");
			return NULL;
		}

		std::shared_ptr<System> system = std::make_shared<System>();
		systems[typeName] = system;
		return system;
	}

	// Sets the signature for a system in the map
	template<typename System>
	void setSignature(Signature signature) {
		const char* typeName = typeid(System).name();

		if (!hasSystemBeenRegistered(typeName)) {
			printf("System needs to be registered before use\n");
		}

		signatures[typeName] = signature;
	}

	// Called when an entity is destroyed, goes through each system and removes that entity
	void onEntityDestroyed(Entity entity) {
		for (auto pair : systems) {
			std::shared_ptr<System> system = pair.second;
			// Entities is a set (unique values) so no exists check is needed
			system->entities.erase(entity);
		}
	}

	// Goes through each system and checks if the new signature matches and add/remove entities accordingly
	void onEntitySignatureChanged(Entity entity) {
		// Go through each system and check the signature matches the new signature
		for (auto pair : systems) {
			const char* typeName = pair.first;
			std::shared_ptr<System> system = pair.second;
			Signature systemSignature = signatures[typeName];

			// Before AND operator in bitsets, if they are equal the result will be the same as the original
			// If the signatures match add the entity to the system, otherwise remove the entity
			if ((entity.signature & systemSignature) == systemSignature) {
				// Again no check is needed as entities is a set
				system->entities.insert(entity);
			}
			else {
				system->entities.erase(entity);
			}
		}
	}

private:
	// Store systems in a map with the key as the string representation of the type name
	std::unordered_map<const char*, std::shared_ptr<System>> systems;
	// Store which components are needed for each system in a bitset
	// If bit at index of component id is 1 then the system requires that component
	std::unordered_map<const char*, Signature> signatures;

	bool hasSystemBeenRegistered(const char* typeName) {
		return systems.find(typeName) != systems.end();
	}
};