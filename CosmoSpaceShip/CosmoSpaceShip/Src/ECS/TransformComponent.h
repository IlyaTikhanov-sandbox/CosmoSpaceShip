#pragma once

#include "Components.h"
#include "Constants.h"
#include "../Vector2D.h"

class TransformComponent : public Component
{
public:

	Vector2D position;
	Vector2D velocity;
	Vector2D direction;
	Vector2D prevPosition;

	int startPosX = 0;
	int startPosY = 0;

	int height = 32;
	int width = 32;

	int scaledHeight = 0;
	int scaledWidth = 0;

	int scale = 1;

	int speed = 3;

	Vector2D getCentralPosTOP(int width = 0)
	{
		return Vector2D(position.x + (scaledWidth / 2) - (width / 2), position.y + scaledHeight);
	}

	Vector2D getLeftAttackPos(int width = 0)  { return Vector2D(position.x + scaledWidth * 0.125 - (width / 2), position.y + scaledHeight * 0.65); }
	Vector2D getRightAttackPos(int width = 0) { return Vector2D(position.x + scaledWidth * (1-0.125) - (width / 2), position.y + scaledHeight * 0.65); }
	TransformComponent(const TransformComponent & obj):position(obj.position), velocity(obj.velocity), height(obj.height),width(obj.width),scale(obj.scale),speed(obj.speed)
	{}

	TransformComponent& operator=(const TransformComponent & obj)
	{
		position = obj.position;
		velocity = obj.velocity;
		height = obj.height;
		width = obj.width; 
		scale = obj.scale;
		speed = obj.speed;

		return *this;
	}
	TransformComponent()
	{
		position.Zero();
	}

	TransformComponent(int scale_)
	{
		position.Zero();
		//position.x = 400;
		//position.y = 320;
		scale = scale_;
	}

	TransformComponent(float x, float y)
	{
		startPosX = x;
		startPosY = y;
		position.x = x;
		position.y = y;
	}

	TransformComponent(float x, float y, int scale_)
	{
		startPosX = x;
		startPosY = y;
		position.x = x;
		position.y = y;
		scale = scale_;
	}
	TransformComponent(float x, float y, int scale_,int speed_)
	{
		startPosX = x;
		startPosY = y;
		position.x = x;
		position.y = y;
		scale = scale_;
		speed = speed_;
	}

	TransformComponent(float x, float y, int h, int w, int scale_)
	{
		height = h;
		width = w;
		scale = scale_;
		startPosX = x;
		startPosY = y;
		position.x = x;
		position.y = y;
	}
	TransformComponent(float x, float y, int h, int w, int scale_,int speed_)
	{
		height = h;
		width = w;
		scale = scale_;
		startPosX = x;
		startPosY = y;
		position.x = x;
		position.y = y;
		speed = speed_;
	}

	void init() override
	{
		velocity.Zero();
		scaledHeight = height * scale;
		scaledWidth = width * scale;
	}
	void update() override
	{
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}
	void updPrevPostition()
	{
		prevPosition = position;
	}
};
