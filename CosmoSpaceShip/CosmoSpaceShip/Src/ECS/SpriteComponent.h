#pragma once

#include "Components.h"
#include "SDL.h"
#include "Constants.h"
#include "../TextureManager.h"
#include "Animation.h"
#include <map>
#include "../AssetManager.h"


constexpr std::size_t maxAnims = 8;

enum Anims : int
{
	Idle,
	WasHit,
	Dead,
	BossIdle,
	BossWasHit
};

//TODO:
//Make std::bitset<maxAnims> animsEnabled to work
//every Play(Anim)should set animsEnabled bit of Anim to be played
//Sprite component should decide which one to play depending on priority
//Ex. WasHit > Idle to
//Challenge: When to disable Anim's bit from animsEnabled??


class SpriteComponent : public Component
{
private:

	int size = 0;
	int frames = 0;
	int speed = 100;
	bool animated = false;

	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
	
	int animIndex = 0;
	std::bitset<maxAnims> animsEnabled;
	std::map<int, Animation> animations;
	
	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

public:

/// --- Base methods
	SpriteComponent() = default;
	SpriteComponent(std::string id)
	{
		setTex(id);
	}

	SpriteComponent(std::string id, bool isAnimated, int width = 32)
	{
		animated = isAnimated;

		Animation idle = Animation(0, 4, 150);
		Animation wasHit = Animation(1, 4, 150);
		Animation dead = Animation(2, 4, 150);
		Animation BossIdle = Animation(0, 8, 300);
		Animation BossWasHit = Animation(1, 8, 300);

		animations.emplace(Anims::Idle, idle);
		animations.emplace(Anims::WasHit, wasHit);
		animations.emplace(Anims::Dead, dead);
		animations.emplace(Anims::BossIdle, BossIdle);
		animations.emplace(Anims::BossWasHit, BossWasHit);

		Play(Anims::Idle);
		setTex(id);
		size = width;
	}

	~SpriteComponent() {}

/// --- Getters

	SDL_RendererFlip getSpriteFlip() { return spriteFlip; }

/// --- Setters

	void setSpriteFlip(SDL_RendererFlip _spriteFlip) { spriteFlip = _spriteFlip; }	
	void setTex(std::string id) { texture = Game::assets->GetTexture(id); }

/// --- Core Component functions

	void init() override
	{
		srcRect.x = srcRect.y = 0; //WA to draw Source Rect > 32x32
		
		srcRect.w = entity->getComponent<TransformComponent>().getWidth();
		srcRect.h = entity->getComponent<TransformComponent>().getHeight();
	}

	void update() override
	{
		if (animated)
		{
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}

		srcRect.y = animIndex * srcRect.h; // animIndex * transform->height

		Vector2D position = entity->getComponent<TransformComponent>().getPosition();

		destRect.x = static_cast<int>(position.x) - Game::camera.x;
		destRect.y = static_cast<int>(position.y) - Game::camera.y;
		destRect.w = entity->getComponent<TransformComponent>().getScaledWidth();
		destRect.h = entity->getComponent<TransformComponent>().getScaledHeight();
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect,spriteFlip);
	}

	void Play(int animName)
	{
		frames = animations[animName].frames;
		animIndex = animations[animName].index;
		speed = animations[animName].speed;
	}

/// --- 
};