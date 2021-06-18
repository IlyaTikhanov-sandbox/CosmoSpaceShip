#pragma once

#include "../Game.h"
#include "ECS.h"
#include "Components.h"
//#include "TimingComponent.h"
#include "FigthComponent.h"

class KeyboardController : public Component
{
public:
	TransformComponent *transform;
	TimingComponent * timing;
	SpriteComponent * sprite;
	FigthComponent * fighting;
	SoundHandler* sounding;

	KeyboardController(SoundHandler* s):sounding(s) {}
	~KeyboardController() {}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		timing = &entity->getComponent<TimingComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
		fighting = &entity->getComponent<FigthComponent>();
	}

	void update() override;
private:
	const Uint8 *keystate;
	//AssetManager* m_assets;
};