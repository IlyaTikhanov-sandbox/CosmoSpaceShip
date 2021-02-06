#pragma once

#include "SDL.h"
#include "ECS/Constants.h"
#undef main
#include "SDL_image.h"
#include <iostream>
#include <vector>
#include "Sounds.h"
//#include "ECS/ECS.h"
#include <map>
#include <random>

#define FILE_PATH "assets/theme.wav"
#define LVL_CHANGE 120
//#define FRIENDLY


class ColliderComponent;
class AssetManager;
class Entity;
class Weapon;

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

enum LevelStage : int
{
	StageRegular,
	StageBoss
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

	void ChangeLevel()
	{
		int num = currentStage;
		++num;
		currentStage = static_cast<LevelStage>(num);
	}

	bool AllEnemiesAreDead();

	bool isEntityOnDisplay(Entity * e);

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
	LevelStage currentStage;

	WeaponParams PlayerWeaponParams;
	WeaponParams EnemyWeaponParams;
	WeaponParams BossWeaponParams;

	//Game Objects
	Weapon* player_weapon;
	Weapon* enemy_weapon;
	Weapon* boss_weapon;

	//Temp solution
	std::vector<std::vector <char>> walkinMap;

	void LoadAssets();
	void InitWeapon(WeaponParams * weapon, SoundHandler * sounder);
	void WeaponsInit();
	void playerInit();
	void enemyInit(float xpos, float ypos, std::string texture);
	void BossInit(float xpos, float ypos, const char * texture);
	void InitLvl1(int Num_Enemies = 3);

	enum groupLabels : std::size_t
	{
		groupMap,
		groupPlayers,
		gpoupEnemies,
		groupBoss,
		gpoupWeapons,
		groupColliders,
		groupPlayerProjectiles,
		groupEnemyProjectiles,
		groupWeapons
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
};

