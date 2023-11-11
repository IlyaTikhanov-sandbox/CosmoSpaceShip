#pragma once

#include "SDL.h"
#include "Game.h"

class ColliderComponent;

class Collision
{
public:
	static bool AABBx(const SDL_Rect& recA, const SDL_Rect& recB);
	static bool AABBy(const SDL_Rect& recA, const SDL_Rect& recB);
	static bool AABB(const SDL_Rect & recA, const SDL_Rect & recB);
	static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB);
	static bool CenterAABB(const SDL_Rect & recA, const SDL_Rect & recB);
	static bool CenterAABB(const ColliderComponent& colA, const ColliderComponent& colB);
};