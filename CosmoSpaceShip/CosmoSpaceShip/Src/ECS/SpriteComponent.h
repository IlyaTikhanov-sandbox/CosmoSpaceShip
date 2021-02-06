#pragma once

#include "Components.h"
#include "SDL.h"
#include "Constants.h"
#include "../TextureManager.h"
#include "Animation.h"
#include <map>
#include "../AssetManager.h"


constexpr std::size_t maxAnims = 4;

enum Anims : int
{
	Idle,
	WasHit,
	Dead,
	BossIdle
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
	TransformComponent * transform;
	SDL_Texture * texture;
	SDL_Rect srcRect, destRect;

	bool animated = false;
	int frames = 0;
	int speed = 100;
	int size;

public:

	int animIndex = 0;
	std::map<int, Animation> animations;
	std::bitset<maxAnims> animsEnabled;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(std::string id)
	{
		setTex(id);
	}
	SpriteComponent(std::string id,bool isAnimated,int width = 32)
	{
		animated = isAnimated;

		Animation idle   = Animation(0, 4, 150);
		Animation wasHit = Animation(1, 4, 150);
		Animation dead   = Animation(2, 4, 150);
		Animation BossIdle = Animation(0, 8, 150);

		animations.emplace(Anims::Idle  , idle);
		animations.emplace(Anims::WasHit, wasHit);
		animations.emplace(Anims::Dead , dead);
		animations.emplace(Anims::BossIdle, BossIdle);

		

		Play(Anims::Idle);
		setTex(id);
		size = width;
		
	}

	~SpriteComponent()
	{
	}
	
	void setTex(std::string id)
	{
		texture = Game::assets->GetTexture(id);
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();


		srcRect.x = srcRect.y = 0;
		//WA to draw Source Rect > 32x32
		
		srcRect.w = transform->width;
		srcRect.h = transform->height;
	}

	void update() override
	{
		if (animated)
		{
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}

		srcRect.y = animIndex * transform->height;

		destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
		destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
		destRect.w = transform->width * transform->scale;
		destRect.h = transform->height * transform->scale;
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

};