#include "LogicComponent.h"

void LogicComponent::GoToThePoint(Vector2D destPoint)
{
	float Xdiff = destPoint.x - OwnTransform->position.x;
	float Ydiff = destPoint.y - OwnTransform->position.y;
	OwnTransform->velocity.x = abs(Xdiff) < 3 ? 0 : Xdiff / abs(Xdiff);
	OwnTransform->velocity.y = abs(Ydiff) < 3 ? 0 : Ydiff / abs(Ydiff);
	//if(OwnTransform->velocity.x)
	//	sprite->spriteFlip = OwnTransform->velocity.x < 0 ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
	sprite->Play(Anims::Idle);
}


void LogicComponent::GOthePath()
{
	if (!pointStack->empty())
	{
		busy = true;
		idle = false;
		if (NOTAlmostEquals(OwnTransform->position, pointStack->checkTop()))// (!~=)
		{
			GoToThePoint(pointStack->checkTop());
		}
		else
		{
			OwnTransform->position = pointStack->pop();
		}
	}
}

void LogicComponent::GoLeftRight()
{
	if (goLeft)
	{
		busy = true;
		idle = false;
		Vector2D destPoint = Vector2D(70.0f, 70.0f);
		if (NOTAlmostEquals(OwnTransform->position, destPoint))// (!~=)
		{
			GoToThePoint(destPoint);
		}
		else
		{
			//OwnTransform->position = destPoint;
			goLeft = !goLeft;
		}
	}
	else
	{
		busy = true;
		idle = false;
		Vector2D destPoint = Vector2D(1115.0f - OwnTransform->scaledWidth, 70.0f);
		if (NOTAlmostEquals(OwnTransform->position, destPoint))// (!~=)
		{
			GoToThePoint(destPoint);
		}
		else
		{
			//OwnTransform->position = destPoint;
			goLeft = !goLeft;
		}
	}
}

void LogicComponent::ChaseDownPlayer(Vector2D playerPos)
{
	ClearPoints();
	if (NOTAlmostEquals(OwnTransform->position, playerPos))
		AddPoint(playerPos);
	//fighting->TargetAttack(OwnTransform->position, playerPos, fighting->getWeapon()->getSound());
}

void LogicComponent::stay()
{
	idle = true;
	busy = false;
	OwnTransform->velocity.x = 0;
	OwnTransform->velocity.y = 0;
	sprite->Play(Anims::Idle);
}
void LogicComponent::Bossstay()
{
	idle = true;
	busy = false;
	OwnTransform->velocity.x = 0;
	OwnTransform->velocity.y = 0;
	sprite->Play(Anims::BossIdle);
}

void LogicComponent::update()
{
	//std::cout << "Current pos: X:" << OwnTransform->position.x << " Y: " << OwnTransform->position.y << std::endl;
	releaseHit();
	if (hp->WasHit() != 0)
	{
		idle = false;
	}
	else if(busy)
	{
		switch (currentLogic)
		{
		case LogicType::LogicNone:

			break;
		case LogicType::FollowPath:
			GOthePath();
			break;
		case LogicType::EasyLogic:
			GoLeftRight();
			break;
		case LogicType::BossLogic:
			Bossstay();
			break;
		default:
			break;
		}
	}
	else 
	{
	if (!idle)
		stay();
	}
}

