#pragma once

#include "SDL.h"
#include "ECS/Constants.h"
#include "CS_types.h"
#undef main
#include "SDL_image.h"
#include <iostream>
#include <vector>
#include "Sounds.h"
#include <string>
//#include "ECS/ECS.h"
#include <map>
#include <random>
#include "ResolutionSettings.h"

#define FILE_PATH "assets/theme.wav"
#define LVL_CHANGE 120
//#define DEBUG

class ColliderComponent;
class AssetManager;
class Entity;
class Weapon;
class Menu;

enum AttackType : int
{
	UsualAttack,
	SpecialAttack,
	RicochetAttack
};
enum ProjectileType :int
{
	StraightForward,
	Ricochet
};

struct Attack
{
	int range;
	int speed;
	int damage;
	int scale;
	float screenFactor;
	int delay;
	std::string textureID;
	bool isAnimated = true;
	bool rotate = false;
	AttackType attackType;
	SoundHandler::soundType attackSound;
	ProjectileType projType;
};


struct WeaponParams
{
	WeaponParams() {}
	int Attack_num;
	std::map<AttackType, Attack> attacks;
	SoundHandler *pSounder;
};

struct Score
{
public:
	int calculateBonus(std::vector<const Entity*> deadEntities);

	void increaseScore(int add) { m_score += add; }
	void growScore(LevelStage level) 
	{ 
		//m_score += static_cast<int>(level) * 0.3;
		if(bonusTimer > 0)
			bonusTimer--;
	}
	void printScore();
	void resetScore() { m_score = 0; bonusTimer = 550; }
	
	std::string getScoreStr();

	void resetTimer() { bonusTimer = 550; }
	int getTimer() { return bonusTimer; }
	void printTimer();


private:
	long double m_score{ 0 };
	int bonusTimer{ 550 };
};


class Game
{
public:
	Game();
	~Game();

	void init(const char* title, int width, int height, bool fullscreen);

	void handleEvents();
	void update();
	bool running() { return isRunning; }
	void render();
	void clean();

	void ChangeLevel();

	bool AllEnemiesAreDead();

	bool isEntityOnDisplay(Entity * e);

	//MenuFunctions
	void CreateMenu();
	void prepareGameMenu();
	void handleGameMenu();
	void activateMenuButton(int currentButton);
	void updateGameMenu();
	void renderGameMenu();
	bool isGameMenuActive();
	bool isGameMenuGameClosed();
	void switchToGameMenu();
	bool isNewGame()  { return new_game; }
	void newGameOff() { new_game = false; }
	

	void drawBackground();

	static Menu* m_gameMenu;
	static SDL_Renderer * renderer;
	static SDL_Event event;
	static bool isRunning;
	static int commonPlayerSpeed;
	static SDL_Rect camera;
	static AssetManager * assets;
	SoundHandler* sounder;
	bool rock = false;
	static bool pause;
	static bool switch_on;
	static LevelStage currentStage;

	//Resolution Parameters
	static ResolutionSettings* m_resolutionSettings;
	static int actualPlayWidth;
	static int actualPlayHeight;


	WeaponParams PlayerWeaponParams;
	WeaponParams EnemyWeaponParams;
	WeaponParams BossWeaponParams;

	//Game Objects
	Weapon* player_weapon;
	Weapon* enemy_weapon;
	Weapon* boss_weapon;

	//Temp solution
	std::vector<std::vector <char>> walkinMap;

	//Game functions
	void LoadAssets();
	void InitWeapon(WeaponParams * weapon, SoundHandler * sounder);
	void WeaponsInit();
	void playerInit();
	void enemyInit(float xpos, float ypos, std::string texture);
	void BossInit(float xpos, float ypos, const char * texture);
	void InitLvl1(int Num_Enemies = 3);
	void killEnemies();
	void resetGame();
	void reInit();
	bool isPlayerActive();

	enum groupLabels : std::size_t
	{
		groupMap,
		groupPlayers,
		groupEnemies,
		groupBoss,
		gpoupWeapons,
		groupColliders,
		groupPlayerProjectiles,
		groupEnemyProjectiles,
		groupWeapons,
		groupTexts
	};

	enum CollisionType : int
	{
		None,
		X,
		Y
	};

	void TimingHandle();

	// // guaranteed unbiased

private:
	int cnt = 0;
	SDL_Window *window;
	int lvl_change_timer = 0;
	bool changeLevelRequested = false;

	bool new_game = false;
	int gameMenuDelay = 10;
	int gameMenuCooldown;
	int gameMenuCooldown2;

	int m_playWidth;
	int m_playHeight;

	Score m_scoreCounter;
};


