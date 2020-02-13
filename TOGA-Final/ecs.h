#pragma once

#include "entity_manager.h"
#include "component_manager.h"
#include "system_manager.h"

// Static class that acts as a central location to access ECS related functions
class ECS {
public:
	static Entity createEntity() {
		return entityManager->create();
	}

	static void destroyEntity(Entity entity) {
		entityManager->destroy(entity);
		componentManager->onEntityDestroyed(entity);
		systemManager->onEntityDestroyed(entity);
	}

	template<typename Component>
	static void registerComponent() {
		componentManager->registerComponent<Component>();
	}

	// Maybe move these into ComponentManager
	template<typename Component>
	static void addComponent(Entity entity, Component component) {
		// Adds the component to the vector of components
		componentManager->addComponent<Component>(entity, component);
		// Sets the bit at the index of the component's unique id to true
		entity.signature.set(componentManager->getComponentId<Component>(), true);
		// Update systems to add entities if needed
		systemManager->onEntitySignatureChanged(entity);
	}

	template<typename Component>
	static void removeComponent(Entity entity) {
		componentManager->removeComponent<Component>(entity);
		entity.signature.set(componentManager->getComponentId<Component>(), false);
		systemManager->onEntitySignatureChanged(entity);
	}

	template<typename Component>
	static Component& getComponent(Entity entity) {
		return componentManager->getComponent<Component>(entity);
	}

	template<typename Component>
	static uint8_t getComponentId() {
		return componentManager->getComponentId<Component>();
	}

	template<typename System>
	static std::shared_ptr<System> registerSystem() {
		return systemManager->registerSystem<System>();
	}

	template<typename System>
	static void setSystemSignature(Signature signature) {
		systemManager->setSignature<System>(signature);
	}

private:
	static std::unique_ptr<EntityManager> entityManager;
	static std::unique_ptr<ComponentManager> componentManager;
	static std::unique_ptr<SystemManager> systemManager;
};
