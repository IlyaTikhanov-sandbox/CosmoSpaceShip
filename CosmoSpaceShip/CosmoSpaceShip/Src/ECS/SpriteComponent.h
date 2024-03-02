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
//Make std::bitset<maxAnims> m_animsEnabled to work
//every Play(Anim)should set m_animsEnabled bit of Anim to be played
//Sprite component should decide which one to play depending on priority
//Ex. WasHit > Idle to
//Challenge: When to disable Anim's bit from m_animsEnabled??


class SpriteComponent : public Component
{
private:

	int m_size = 0;
	int m_frames = 0;
	int m_speed = 100;
	bool m_animated = false;

	SDL_Texture* m_texture;
	SDL_Rect m_srcRect, m_destRect;
	
	int m_animIndex = 0;
	std::bitset<maxAnims> m_animsEnabled;
	std::map<int, Animation> m_animations;
	
	SDL_RendererFlip m_spriteFlip = SDL_FLIP_NONE;

public:

/// --- Base methods
	SpriteComponent() = default;
	SpriteComponent(std::string id)
	{
		setTex(id);
	}

	SpriteComponent(std::string id, bool isAnimated, int width = 32)
	{
		m_animated = isAnimated;

		Animation idle = Animation(0, 4, 150);
		Animation wasHit = Animation(1, 4, 150);
		Animation dead = Animation(2, 4, 150);
		Animation BossIdle = Animation(0, 8, 300);
		Animation BossWasHit = Animation(1, 8, 300);

		m_animations.emplace(Anims::Idle, idle);
		m_animations.emplace(Anims::WasHit, wasHit);
		m_animations.emplace(Anims::Dead, dead);
		m_animations.emplace(Anims::BossIdle, BossIdle);
		m_animations.emplace(Anims::BossWasHit, BossWasHit);

		Play(Anims::Idle);
		setTex(id);
		m_size = width;
	}

	~SpriteComponent() {}

/// --- Getters

	SDL_RendererFlip getSpriteFlip() { return m_spriteFlip; }

/// --- Setters

	void setSpriteFlip(SDL_RendererFlip _spriteFlip) { m_spriteFlip = _spriteFlip; }	
	void setTex(std::string id) { m_texture = Game::assets->GetTexture(id); }

/// --- Core Component functions

	void init() override
	{
		m_srcRect.x = m_srcRect.y = 0; //WA to draw Source Rect > 32x32
		
		m_srcRect.w = entity->getComponent<TransformComponent>().getWidth();
		m_srcRect.h = entity->getComponent<TransformComponent>().getHeight();
	}

	void update() override
	{
		if (m_animated)
		{
			m_srcRect.x = m_srcRect.w * static_cast<int>((SDL_GetTicks() / m_speed) % m_frames);
		}

		m_srcRect.y = m_animIndex * m_srcRect.h; // m_animIndex * transform->height

		Vector2D position = entity->getComponent<TransformComponent>().getPosition();

		m_destRect.x = static_cast<int>(position.x) - Game::camera.x;
		m_destRect.y = static_cast<int>(position.y) - Game::camera.y;
		m_destRect.w = entity->getComponent<TransformComponent>().getScaledWidth();
		m_destRect.h = entity->getComponent<TransformComponent>().getScaledHeight();
	}

	void draw() override
	{
		TextureManager::Draw(m_texture, m_srcRect, m_destRect,m_spriteFlip);
	}

	void Play(int animName)
	{
		m_frames = m_animations[animName].frames;
		m_animIndex = m_animations[animName].index;
		m_speed = m_animations[animName].speed;
	}

/// --- 
};