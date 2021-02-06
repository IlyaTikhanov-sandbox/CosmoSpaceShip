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

	void update() override
	{
		/*
		SDL_PumpEvents();
		keystate = SDL_GetKeyboardState(NULL);
		if (keystate[SDLK_RETURN]) printf("Return Key Pressed.\n");
		if (keystate[SDLK_w] && keystate[SDLK_UP]) std::cout << "Right and Up Keys Pressed.\n" << std::endl;
		
		//with this system it is possible to make multiple key handling
		//need to use else if for each event

		//KEY is down
		if (keystate[SDL_SCANCODE_W])
		{
			transform->velocity.y = -1.65f;
			sprite->Play("Walk");
		}
		if (keystate[SDL_SCANCODE_A])
		{
			transform->velocity.x = -1.65f;
			transform->direction.x = transform->velocity.x;
			sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
			sprite->Play("Walk");
		}
		if (keystate[SDL_SCANCODE_S])
		{
			transform->velocity.y = 1.65f;
			transform->direction.y = transform->velocity.y;
			sprite->Play("Walk");
		}
		if (keystate[SDL_SCANCODE_D])
		{
			transform->velocity.x = 1.65f;
			transform->direction.x = transform->velocity.x;
			sprite->spriteFlip = SDL_FLIP_NONE;
			sprite->Play("Walk");
		}

		//KEY is up
		if (keystate[SDL_SCANCODE_W] == 0)
		{
			transform->velocity.y = 0;
			sprite->Play("Idle");
		}
		if (keystate[SDL_SCANCODE_A] == 0)
		{
			transform->velocity.x = 0;
			sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
			sprite->Play("Idle");
		}
		if (keystate[SDL_SCANCODE_S] == 0)
		{
			transform->velocity.y = 0;
			sprite->Play("Idle");
		}
		if (keystate[SDL_SCANCODE_D] == 0)
		{
			transform->velocity.x = 0;
			sprite->spriteFlip = SDL_FLIP_NONE;
			sprite->Play("Idle");
		}
		*/

		if (Game::event.type == SDL_KEYDOWN)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_UP:
				fighting->Attack(transform->position, Vector2D(0, -1), SoundHandler::soundUpAttack);
				break;
			case SDLK_w:
				transform->velocity.y = -1.65f;
				break;
			case SDLK_a:
				transform->velocity.x = -1.65f;
				break;
			case SDLK_d:
				transform->velocity.x = 1.65f;
				break;
			case SDLK_s:
				transform->velocity.y = 1.65f;
				break;
			case SDLK_p:
				Game::pause = true;
				break;
			//case SDLK_o:
			//	Game::switch_on = false;
			//	sounding->changeBackgroundSound(SoundHandler::MusicType::Boring);
			//	break;
			default:
				break;
			}
		}

		if (Game::event.type == SDL_KEYUP)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_LEFT:
				break;
			case SDLK_RIGHT:
				break;
			case SDLK_UP:
				break;
			case SDLK_DOWN:
				break;
			case SDLK_q:
				break;
			case SDLK_w:
				transform->velocity.y = 0;
				sprite->Play(Anims::Idle);
				break;
			case SDLK_a:
				transform->velocity.x = 0;
				sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
				sprite->Play(Anims::Idle);
				break;
			case SDLK_d:
				transform->velocity.x = 0;
				sprite->spriteFlip = SDL_FLIP_NONE;
				sprite->Play(Anims::Idle);
				break;
			case SDLK_s:
				transform->velocity.y = 0;
				sprite->Play(Anims::Idle);
				break;
			case SDLK_ESCAPE:
				Game::isRunning = false;
				break;
			default:
				break;
			}
		}
	}
private:
	const Uint8 *keystate;
	//AssetManager* m_assets;
};