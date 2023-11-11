#pragma once

#include "Components.h"
#include "SDL.h"
#include "Constants.h"

class ScoreComponent : public Component
{
	ScoreComponent(): score(0), bonusTimer(500)
	{
		
	}
	ScoreComponent(int timerValue) : score(0), bonusTimer(timerValue)
	{

	}

	void init()
	{}

	void update()
	{
		//entity->g
	}

	void addScore(double bonus) { score += bonus; }
	void setScore(double scoreValue) { score = scoreValue; }
	int getScore() { return score; }
private:
	long double score;
	int bonusTimer;
};