#pragma once

#include "ECS.h"
#include "Components.h"
#include "../Vector2D.h"

class ProjectileComponent : public Component
{
public:
	ProjectileComponent(int rng, int sp,int dam, Vector2D vel,ProjectileType prType = ProjectileType::StraightForward):range(rng),speed(sp),damage(dam),velocity(vel),projType(prType)
	{}
	~ProjectileComponent()
	{}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = velocity * speed;
	}

	void update() override
	{
		distance += speed;

		if (distance > range)
		{
			//std::cout << "Out of Range" << std::endl;
			entity->destroy();
		}
		else if (transform->position.x > Game::camera.x + Game::camera.w ||
			     transform->position.x < Game::camera.x - Game::camera.w ||
			     transform->position.y > Game::camera.y + Game::camera.h ||
			     transform->position.y < Game::camera.y - Game::camera.h)
		{
			//std::cout << "Out of bounds" << std::endl;
			entity->destroy();
		}
	}

	void changeRicochetDirX()
	{
		transform->velocity.x *= -1;
		transform->position.x += transform->velocity.x ;
	}
	void changeRicochetDirY()
	{
		transform->velocity.y *= -1;
		transform->position.y += transform->velocity.y ;
	}

	int getDamage() { return damage; }

	ProjectileType projType;

private:
	TransformComponent * transform;

	int damage = 25;
	int range = 0;
	int speed = 0;
	int distance = 0;
	Vector2D velocity;

};