#pragma once

#include <memory>
#include "component_array.h"

// Function definitions are in the header file since you can't easily give definitions for 
// generic type functions externally
// See https://stackoverflow.com/questions/495021/why-can-templates-only-be-implemented-in-the-header-file

class ComponentManager {
public:
	ComponentManager() : nextComponentId(0) {}

	// Creates a unique for the component type adds it to the maps
	template<typename Component>
	void registerComponent() {
		// Get string from Component type name to use as key in maps
		const char* typeName = typeid(Component).name();

		// Make sure component hasn't already been registered
		if (componentIds.find(typeName) != componentIds.end()) {
			printf("Cannot register component type more than once\n");
			return;
		}

		// Update maps
		componentIds[typeName] = nextComponentId;
		componentArrays[typeName] = new ComponentArray<Component>();

		// Increment component id
		nextComponentId++;
	}

	// Retrieves component id from map
	template<typename Component>
	int getComponentId() {
		const char* typeName = typeid(Component).name();
		if (componentIds.find(typeName) == componentIds.end()) {
			printf("Component must be registered before use\n");
			return -1;
		}
		return componentIds[typeName];
	}

	// Adds a component to the relevant ComponentArray
	template<typename Component>
	void addComponent(Entity entity, Component component) {
		getComponentArray<Component>()->insertComponent(entity, component);
	}

	// Removes a component from the relevant ComponentArray
	template<typename Component>
	void removeComponent(Entity entity) {
		getComponentArray<Component>()->removeComponent(entity);
	}

	// Retrieves component from relevant ComponentArray
	template<typename Component>
	Component& getComponent(Entity entity) {
		return getComponentArray<Component>()->getComponent(entity);
	}

	// Looks through each ComponentArray and removes components associated with that entity
	void onEntityDestroyed(Entity entity) {
		for (auto pair : componentArrays) {
			IComponentArray* component = pair.second;
			component->onEntityDestroyed(entity);
		}
	}

	template<typename Component>
	bool hasComponent(Entity entity) {
		return getComponentArray<Component>()->hasComponent(entity);
	}

private:
	// Gives each component a unique id (mapped to the string of component type name)
	std::unordered_map<const char*, int> componentIds;
	// Map component arrays to the type name
	std::unordered_map<const char*, IComponentArray*> componentArrays;
	// Incremented every time a component is registered
	int nextComponentId;

	template<typename Component>
	ComponentArray<Component>* getComponentArray() {
		const char* typeName = typeid(Component).name();
		if (componentIds.find(typeName) == componentIds.end()) {
			printf("Component must be registered before use\n");
			return NULL;
		}
		// Cast IComponentArray to ComponentArray<Component>
		return (ComponentArray<Component>*) componentArrays[typeName];
	}
};