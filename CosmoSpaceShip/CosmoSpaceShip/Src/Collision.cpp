#include "Collision.h"
#include "ECS/ColliderComponent.h"

bool Collision::AABB(const SDL_Rect & recA, const SDL_Rect & recB)
{
	if (recA.x + recA.w >= recB.x &&
		recB.x + recB.w >= recA.x &&
		recA.y + recA.h >= recB.y &&
		recB.y + recB.h >= recA.y)
	{
		return true;
	}

	return false;
}

bool Collision::AABB(const ColliderComponent & colA, const ColliderComponent & colB)
{
	if (AABB(colA.collider, colB.collider))
	{
		//std::cout << colA.tag << " hit: " << colB.tag << std::endl;
		return true;
	}
	else
		return false;

	return false;
}

bool Collision::CenterAABB(const SDL_Rect & recA, const SDL_Rect & recB)
{
	if (recA.x + recA.w/2 >= recB.x &&
		recB.x + recB.w/2 >= recA.x &&
		recA.y + recA.h/2 >= recB.y &&
		recB.y + recB.h/2 >= recA.y)
	{
		return true;
	}

	return false;
}

bool Collision::CenterAABB(const ColliderComponent & colA, const ColliderComponent & colB)
{
	if (CenterAABB(colA.collider, colB.collider))
	{
		//std::cout << colA.tag << " hit: " << colB.tag << std::endl;
		return true;
	}
	else
		return false;

	return false;
}

