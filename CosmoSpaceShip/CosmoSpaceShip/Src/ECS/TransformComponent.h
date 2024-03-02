#pragma once

#include "Components.h"
#include "../Game.h"
#include "../Vector2D.h"

class TransformComponent : public Component
{
public:

/// --- Make it private
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
	float screenCoeff = 0;

	int speed = 3;
/// --- 

// --- Getters
	int getWidth() { return width; }
	int getHeight() { return height; }

	Vector2D getPosition() { return position; }

	int getScaledWidth() { return scaledWidth; }
	int getScaledHeight() { return scaledHeight; }


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

// --- Constructors
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
	TransformComponent(float x, float y, int scale_,int speed_) // Usually use this one
	{
		startPosX = x;
		startPosY = y;
		position.x = x;
		position.y = y;
		scale = scale_;
		speed = speed_;
	}

	//<summary> Constructor considering Screen Resolution </summary>
	TransformComponent(float x, float y, float screenResolutionFactor, int speed_)
	{
		startPosX = x;
		startPosY = y;
		position.x = x;
		position.y = y;
		screenCoeff = screenResolutionFactor;
		scale = 0;
		speed = speed_;

		int checkScaledHeight = 0;
		int checkScaledWidth = 0;
		if (screenCoeff != 0)
		{
			checkScaledHeight = Game::m_resolutionSettings->getWindowWidth() * screenCoeff;
			checkScaledWidth = Game::m_resolutionSettings->getWindowWidth() * screenCoeff;
		}
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

	TransformComponent(float x, float y, int h, int w, float screenCoef_, int speed_)
	{
		height = h;
		width = w;
		screenCoeff = screenCoef_;
		startPosX = x;
		startPosY = y;
		position.x = x;
		position.y = y;
		speed = speed_;
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
		if (screenCoeff != 0)
		{
			scaledHeight = Game::m_resolutionSettings->getWindowWidth() * screenCoeff;
			scaledWidth = Game::m_resolutionSettings->getWindowWidth() * screenCoeff;
		}
		else
		{
			scaledHeight = height * scale;
			scaledWidth = width * scale;
		}
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

	void Leap(int length)
	{
		//if (abs((position.x + direction.x * length) - WINDOW_WIDTH / 2) < WINDOW_WIDTH / 2 - PERSON_SIZE)
		//{
		//	std::cout << "LEAP!" << std::endl;
		//	position.x += direction.x * length;
		//}
		int XNextPosition = position.x + direction.x * length;
		if (XNextPosition < 0)
		{
			XNextPosition = 1;
		}
		else if (XNextPosition + 32 * scale * 2 > Game::camera.w)
		{
			std::cout << "RIGHT LEAP PREVENT OUT OF BORDER!!" << std::endl;
			XNextPosition = Game::camera.w - 32 * scale - 10;
		}
		position.x = XNextPosition;
	}
};