#pragma once

#include "ecs.h"

extern SDL_Renderer* renderer;

class RenderTextSystem : public System {
public:
	RenderTextSystem() {
		signature.set(ECS::getComponentId<TransformComponent>());
		signature.set(ECS::getComponentId<UITextComponent>());
	}

	void init() override {
		for (Entity entity : entities) {
			UITextComponent& textComponent = ECS::getComponent<UITextComponent>(entity);
			loadTexture(textComponent);
		}
	}

	void update(float dt) override {
		for (Entity entity : entities) {
			if (!ECS::entityManager->isActive(entity)) continue;
			TransformComponent& transform = ECS::getComponent<TransformComponent>(entity);
			UITextComponent& text = ECS::getComponent<UITextComponent>(entity);

			if (text.textChanged) {
				loadTexture(text);
				text.textChanged = false;
			}

			if (text.texture != NULL) {
				// Size of image to render
				SDL_Rect renderQuad = { transform.x, transform.y, text.width * transform.scaleX, text.height * transform.scaleY };
				renderQuad.x -= text.width * text.pivot.x;
				renderQuad.y -= text.height * text.pivot.y;
				//Render to screen
				SDL_RenderCopyEx(renderer, text.texture, NULL, &renderQuad, transform.rot, NULL, SDL_FLIP_NONE);
			}
		}
	}

	void dealloc() override {
		for (Entity entity : entities) {
			UITextComponent text = ECS::getComponent<UITextComponent>(entity);
			SDL_DestroyTexture(text.texture);
			text.texture = NULL;
		}
	}

	void loadTexture(UITextComponent& textComponent) {
		if (textComponent.text == "") {
			textComponent.texture = NULL;
			return;
		}

		SDL_DestroyTexture(textComponent.texture);
		SDL_Surface* surface = TTF_RenderText_Blended(textComponent.font, textComponent.text.c_str(), textComponent.colour);

		if (surface != NULL) {
			//Create texture from surface pixels
			textComponent.texture = SDL_CreateTextureFromSurface(renderer, surface);
			if (textComponent.texture != NULL) {
				// Get image dimensions
				SDL_QueryTexture(textComponent.texture, NULL, NULL, &textComponent.width, &textComponent.height);
			}
			else {
				printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
			}

			// Free surface from memory since it's no longer needed
			SDL_FreeSurface(surface);
		}
		else {
			printf("Unable to render text surface - Error: %s\n", TTF_GetError());
		}
	}
};
