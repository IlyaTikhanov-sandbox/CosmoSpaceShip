#pragma once

#include <string>
#include "SDL.h"
#include "Components.h"
#include "../TextureManager.h"

class ColliderComponent : public Component
{
public:
	SDL_Rect collider;
	std::string tag;

	SDL_Texture* tex;
	SDL_Rect srcR, destR;

	ColliderComponent(std::string t) :tag(t) {}

	ColliderComponent(std::string t, int xpos, int ypos, int size)
	{
		tag = t;
		collider.x = xpos;
		collider.y = ypos;
		collider.h = collider.w = size;
	}

	TransformComponent* transform;

	void init() override
	{
		if (!entity->hasComponent<TransformComponent>())
		{ 
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();

		tex = TextureManager::LoadTexture("assets/ColTex.png");
		srcR = { 0,0,32,32 };
		destR = { collider.x,collider.y,collider.w,collider.h };

		//Game::colliders.push_back(this);
	}

	void update() override
	{
		if (tag != "terrain")
		{
			collider.x = static_cast<int>(transform->position.x);
			collider.y = static_cast<int>(transform->position.y);
			collider.w = static_cast<int>(transform->width * transform->scale);
			collider.h = static_cast<int>(transform->height * transform->scale);
		}

		destR.x = collider.x - Game::camera.x;
		destR.y = collider.y - Game::camera.y;
	}

	void draw() override
	{
		TextureManager::Draw(tex,srcR,destR,SDL_FLIP_NONE);
	}



};