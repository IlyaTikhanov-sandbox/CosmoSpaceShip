#pragma once

#include "Components.h"

class LogicComponent;

class HealthComponent : public Component
{
public:
	HealthComponent(int health = 100):hp(health),maxHP(health) {}
	~HealthComponent() {}

	void init() override
	{
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override
	{
		//std::cout << "hp = " << hp << std::endl;
		if (dead)
		{
			releaseHit();
			if (wasHit != 0)
			{
				sprite->Play(Anims::Dead);
			}
			else
			{
				entity->destroy();
			}
		}
		else
		{
			if (hp <= 0)
			{
				entity->destroy();
				std::cout << "Someone is dead" << std::endl;
			}
			else
			{
				releaseHit();
				if (wasHit != 0)
				{
					sprite->Play(Anims::WasHit);
				}
			}
		}
	}

	void decreaseHP(int damage)
	{
		hp -= damage;
	}

	inline void gotHit() { wasHit = 30; }
	void releaseHit()
	{
		if (wasHit > 0)
			wasHit--;
	}
	int WasHit() { return wasHit; }
	int getMaxHP()  { return maxHP; }

private:
	int hp = 100;
	int maxHP = 0;
	int wasHit = 0;
	bool dead = false;
	SpriteComponent* sprite;
};
