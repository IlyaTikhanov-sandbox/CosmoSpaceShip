#pragma once

#include "Components.h"
#include "../Stack.h"

enum LogicType
{
	LogicNone,
	FollowPath,  //follow the path of points in the pointStack
	EasyLogic,   //Go from the left to the right
	BossLogic
};

class LogicComponent : public Component
{
public:
	LogicComponent(LogicType logic = LogicType::LogicNone)
	{
		currentLogic = logic;
	}

	void init() override
	{
		OwnTransform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
		fighting = &entity->getComponent<FigthComponent>();
		pointStack = new Stack <Vector2D>(100);
		hp = &entity->getComponent<HealthComponent>();
	
		busy = true;

		if (OwnTransform->position.x > 610)
			goLeft = true;
		else
			goLeft = false;
	}

	void AddPoint(Vector2D Point)
	{
		pointStack->push(Point);
	}
	void ClearPoints()
	{
		pointStack->clearStack();
	}


	void GOthePath();
	void GoLeftRight();
	void GoToThePoint(Vector2D destPoint);
	void ChaseDownPlayer(Vector2D playerPos);
	void stay();

	void Bossstay();

	void update() override;
	
	inline void gotHit() { wasHit = 30; }
	void releaseHit()
	{
		if (wasHit > 0)
			wasHit--;
	}
	

private:
	TransformComponent* OwnTransform;
	SpriteComponent* sprite;
	Stack <Vector2D> * pointStack;
	FigthComponent* fighting;
	HealthComponent* hp;
	
	int wasHit = 0;

	bool idle = false;
	bool busy = false;
	bool goLeft = false;

	LogicType currentLogic = LogicType::LogicNone;
};
