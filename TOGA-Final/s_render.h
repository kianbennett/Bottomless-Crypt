#pragma once

#include "ecs.h"
#include "camera.h"

extern Camera camera;
extern SDL_Renderer* renderer;

// Compare function to sort collection by RendererComponent's zIndex (this is really slow)
auto compareZ = [](const Entity& a, const Entity& b) {
	if (ECS::entityManager->isAlive(a) && ECS::componentManager->hasComponent<RendererComponent>(a) &&
		ECS::entityManager->isAlive(b) && ECS::componentManager->hasComponent<RendererComponent>(b)) {
		int zA = ECS::getComponent<RendererComponent>(a).zIndex;
		int zB = ECS::getComponent<RendererComponent>(b).zIndex;
		return zA < zB;
	}
	return a < b;
};

class RenderSystem : public System {
public:

	RenderSystem() {
		signature.set(ECS::getComponentId<TransformComponent>());
		signature.set(ECS::getComponentId<RendererComponent>());
	}

	void init() override {
	}

	void update(float dt) override {
		for (Entity entity : sortedEntities) {
			if (!ECS::entityManager->isActive(entity)) continue;
			TransformComponent transform = ECS::getComponent<TransformComponent>(entity);
			RendererComponent rendererComponent = ECS::getComponent<RendererComponent>(entity);

			// Define rect to render
			SDL_Rect rect = {
				(int)transform.x, (int)transform.y,
				rendererComponent.size.x * transform.scaleX, rendererComponent.size.y * transform.scaleY
			};
			Vec2 pos = camera.transform(Vec2(rect.x, rect.y));
			rect.x = pos.x;
			rect.y = pos.y;
			rect.w *= camera.getScale();
			rect.h *= camera.getScale();

			// Offset by pivot
			rect.x -= rect.w * rendererComponent.pivot.x;
			rect.y -= rect.h * rendererComponent.pivot.y;

			if (rendererComponent.texture != nullptr) {
				// Render texture
				SDL_RenderCopy(renderer, rendererComponent.texture, &rendererComponent.textureClip, &rect);
			}
			else {
				// Set render colour
				SDL_SetRenderDrawColor(renderer, rendererComponent.colour.r, rendererComponent.colour.g, rendererComponent.colour.b, rendererComponent.colour.a);
				// Render rect
				SDL_RenderFillRect(renderer, &rect);
			}
		}
	}

	void dealloc() override {
	}

	// When an entity is added or removed create a new vector of those entities sorted by their zIndex
	// TODO: Do this only once after adding all entities instead of every entity?
	void onEntitiesChanged() override {
		sortedEntities.clear();
		sortedEntities.insert(sortedEntities.end(), entities.begin(), entities.end());
	}

	void sortEntities() {
		std::sort(sortedEntities.begin(), sortedEntities.end(), compareZ);
	}

private:
	std::vector<Entity> sortedEntities;
};