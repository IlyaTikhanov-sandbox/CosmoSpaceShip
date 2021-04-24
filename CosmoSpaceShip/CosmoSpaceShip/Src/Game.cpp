#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS/Menu.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include <vector>
#include <set>
#include <sstream>



static int count = 0;

std::random_device rd;     // only used once to initialise (seed) engine
std::mt19937 rng = std::mt19937(rd());

Map * map;
Manager manager;

SDL_Renderer * Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = {32 * MAP_TILE_SCALE,32 * MAP_TILE_SCALE, WINDOW_WIDTH - (32 * MAP_TILE_SCALE) * 2, WINDOW_HEIGHT - (32 * MAP_TILE_SCALE) * 2};

AssetManager* Game::assets = new AssetManager(&manager);
Menu* Game::m_gameMenu = nullptr;



bool Game::switch_on = true;
LevelStage Game::currentStage = LevelStage::StageMenu;



bool Game::isRunning = false;
bool Game::pause = false;

int Game::commonPlayerSpeed = 3;

//auto& player(manager.addEntity());
auto& label(manager.addEntity());
//MenuLabels
auto& playLabel(manager.addEntity());
auto& exitLabel(manager.addEntity());


Game::Game()
{
	currentStage      = StageMenu;
	gameMenuCooldown  = gameMenuDelay;
	gameMenuCooldown2 = gameMenuDelay;
}

Game::~Game()
{}

void Game::init(const char* title, int width, int height, bool fullscreen)
{
	//switch_on = true;
	int flags = 0;

	m_playHeight = height;
	m_playWidth  = width;
	
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystem Initialized..." << std::endl;
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		if (window)
		{
			std::cout << "Window Created..." << std::endl;
		}
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			std::cout << "Renderer Created..."<< std::endl;
		}
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}

		isRunning = true;
	}

	if (TTF_Init() == -1)
	{
		std::cout << "Error Initializing SDL TTF" << std::endl;
	}

	//currentStage = LevelStage::StageRegular; Uncomment to skip MENU

	sounder = new SoundHandler();
	sounder->init();

	LoadAssets(); //also map gets created


	//Label Init
	///SDL_Color white = { 255,255,255,255 };
	///label.addComponent<UILabel>(10, 10, "Hello", "courier", white);
	//
	
	WeaponsInit();

	playerInit();

	InitLvl1(3);

	BossInit(500.0f, 70.0f, "BossShip");

	CreateMenu();
}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& enemies(manager.getGroup(Game::gpoupEnemies));
//auto& bosses(manager.getGroup(Game::groupBoss));
auto& PlayerProjectiles(manager.getGroup(Game::groupPlayerProjectiles));
auto& EnemyProjectiles(manager.getGroup(Game::groupEnemyProjectiles));
auto& Weapons(manager.getGroup(Game::gpoupWeapons));

void Game::handleEvents()
{
	for (auto& player : manager.getGroup(Game::groupPlayers))
	{
		if (!player->isActive())
		{
			resetGame();
			switchToGameMenu();
			new_game = true;
		}
	}
	

	
	if (AllEnemiesAreDead())
	{
		++lvl_change_timer;
	}

	if (lvl_change_timer == LVL_CHANGE)
	{
		lvl_change_timer = 0;
		ChangeLevel();
	}

	if (currentStage == LevelStage::StageBoss)
	{
		enemies.clear();
		enemies = manager.getGroup(Game::groupBoss);
		for (auto& e : enemies)
		{
			e->addGroup(Game::gpoupEnemies);
		}
		std::cout << "Added Boss to enemies" << std::endl;
	}

	sounder->handleEvents();

	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT :
	//	isRunning = false;
		break;
	default:
		break;
	}
}

/*
void Game::TimingHandle()
{
	player.getComponent<TransformComponent>().speed = commonPlayerSpeed;
	count++;
	if (count == 200)
	{
		count = 0;
		if (player.getComponent<TimingComponent>().size() < 5)
		{
			player.getComponent<TimingComponent>().AddTimingState(player.getComponent<TransformComponent>().position);
			std::cout << "Timing State was added" << "Current Player TimingState Storage size = " << player.getComponent<TimingComponent>().size() << std::endl;
		}
	}
}
*/


void Game::update()
{
	//TimingHandle();

	//Player position from previous frame
	Vector2D prevPlayerPos;
	for (auto& player : manager.getGroup(Game::groupPlayers))
	{
		prevPlayerPos = player->getComponent<TransformComponent>().position;
	}
	//Vector2D 
	//std::cout << "Player width: " << player.getComponent<TransformComponent>().width << std::endl;
	for (auto& pr : EnemyProjectiles)
	{
		pr->getComponent<TransformComponent>().updPrevPostition();
	}

	// Mechanizm to stream some info to the screen
	///std::stringstream ss;
	///ss << "Player" << "(" << prevPlayerPos.x << " , " << prevPlayerPos.y << ")";
	///label.getComponent<UILabel>().SetLabelText(ss.str(), "courier");
	//


	//update all components to the current frame state
	manager.refresh();
	manager.update();

#ifndef FRIENDLY
	//Hitting enemy handling
	for (auto& pr : PlayerProjectiles)
	{
		SDL_Rect prCol = pr->getComponent<ColliderComponent>().collider;
		for (auto& e : enemies)
		{
			SDL_Rect enCol = e->getComponent<ColliderComponent>().collider;
			if (Collision::AABB(prCol, enCol))
			{
				std::cout << "player hit some enemy" << std::endl;
				e->getComponent<HealthComponent>().gotHit();
				e->getComponent<HealthComponent>().update();
				e->getComponent<HealthComponent>().decreaseHP(pr->getComponent<ProjectileComponent>().getDamage());
				pr->destroy();
				sounder->playSound(SoundHandler::soundType::soundExplosive);
			}
			else
			{
				e->getComponent<HealthComponent>().releaseHit();
			}
		}
	}

	for (auto& pr : EnemyProjectiles)
	{
		SDL_Rect prCol = pr->getComponent<ColliderComponent>().collider;
		SDL_Rect plCol;
		for (auto& player : manager.getGroup(Game::groupPlayers))
		{
			plCol = player->getComponent<ColliderComponent>().collider;
		}
		
		if (Collision::AABB(prCol, plCol))
		{
			std::cout << "enemy hit player" << std::endl;
			//player.getComponent<HealthComponent>().gotHit();
			//player.getComponent<HealthComponent>().update();
			for (auto& player : manager.getGroup(Game::groupPlayers))
			{
				player->getComponent<HealthComponent>().decreaseHP(pr->getComponent<ProjectileComponent>().getDamage());
			}
			
			pr->destroy();
		}
		else
		{
			//player.getComponent<HealthComponent>().releaseHit();
		}
		
	}
#endif 

	//Blocking Collision
	SDL_Rect playerCol;
	for (auto& player : manager.getGroup(Game::groupPlayers))
	{
		playerCol = player->getComponent<ColliderComponent>().collider;
	}
	
	SDL_Rect enemyCol;
	for (auto& c : colliders)
	{
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(playerCol, cCol))
		{
			for (auto& player : manager.getGroup(Game::groupPlayers))
			{
				player->getComponent<TransformComponent>().position = prevPlayerPos;
			}
			
		}
		for (auto& e : enemies)
		{
			enemyCol = e->getComponent<ColliderComponent>().collider;
			if (Collision::AABB(enemyCol, cCol))
			{
				if (e->isActive())
					e->getComponent<TransformComponent>().position = e->getComponent<TransformComponent>().prevPosition;
			}
		}
		for (auto& pr : EnemyProjectiles)
		{
			if (pr->getComponent<ProjectileComponent>().projType == ProjectileType::Ricochet)
			{
				SDL_Rect prCol = pr->getComponent<ColliderComponent>().collider;
				if (Collision::AABB(prCol, cCol))
				{
					if (abs(pr->getComponent<TransformComponent>().position.y - 70.0f) < 50)
					{
						pr->getComponent<ProjectileComponent>().changeRicochetDirY();
						pr->getComponent<ColliderComponent>().update();
					}
					else
					{
						pr->getComponent<ProjectileComponent>().changeRicochetDirX();
						pr->getComponent<ColliderComponent>().update();
					}
				}
			}
		}
	}



	//Enemy attack
	
	for (auto& e : enemies)
	{
		e->getComponent<TransformComponent>().updPrevPostition();
		if(isEntityOnDisplay(e))
		{
			//e->getComponent<TransformComponent>().getCentralPos()
			e->getComponent<FigthComponent>().Attack(e->getComponent<TransformComponent>().position, Vector2D(0, 1), SoundHandler::soundType::soundEnemyAttack);
		}
	}
	
}



void Game::render()
{
	SDL_RenderClear(renderer);

	for (auto& t : tiles)
	{
		t->draw();
	}
	
	for (auto& e : enemies)
	{
		e->draw();
	}

	//for (auto& c : colliders)
	//{
	//	c->draw();
	//}
	
	for (auto& pr : PlayerProjectiles)
	{
		pr->draw();
	}

	for (auto& pr : EnemyProjectiles)
	{
		pr->draw();
	}

	for (auto& p : players)
	{
		p->draw();
	}
	

	label.draw();
	
	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void Game::CreateMenu()
{
	m_gameMenu = new Menu();
}

void Game::prepareGameMenu()
{
	SDL_Color white = { 255,255,255,255 };
	playLabel.addComponent<UILabel>(m_playWidth / 2 - 48, m_playHeight / 2 - 200,      "play", "courier", white);
	exitLabel.addComponent<UILabel>(m_playWidth / 2 - 48, m_playHeight / 2 - 200 + 48, "exit", "courier", white);
	//playLabel.addComponent<UILabel>(m_gameMenu->getLabelbyKey("play")); TBD: find out why it is not working
}

void Game::updateGameMenu()
{
	//update all components to the current frame state
	manager.refresh();
	manager.update();

	if (gameMenuCooldown != 0)
		--gameMenuCooldown;
	if (gameMenuCooldown2 != 0)
		--gameMenuCooldown2;

	SDL_Color red   = { 255,0,0,255 };
	SDL_Color white = { 255,255,255,255 };

	switch (m_gameMenu->getCurrentButton())
	{
	case MenuButtons::Play: 
		playLabel.getComponent<UILabel>().SetLabelColor(red);
		exitLabel.getComponent<UILabel>().SetLabelColor(white);
		break;
	case MenuButtons::Exit:
		exitLabel.getComponent<UILabel>().SetLabelColor(red);
		playLabel.getComponent<UILabel>().SetLabelColor(white);
	default:
		break;
	}
}

bool Game::AllEnemiesAreDead()
{
	int DeadEnemies = 0;
	for (auto& e : enemies)
	{
		if (!e->isActive())
			++DeadEnemies;
	}
	if (DeadEnemies == enemies.size())
		return true;
	else
		return false;
}

bool Game::isEntityOnDisplay(Entity * e)
{
	if (Collision::AABB(camera, e->getComponent<ColliderComponent>().collider))
	{
		return true;
	}
	return false;
}

bool Game::isGameMenuActive()
{
	return m_gameMenu->isMenuActive();
}

bool Game::isGameMenuGameClosed()
{
	return m_gameMenu->isMenuGameClosed();
}

void Game::switchToGameMenu()
{
	currentStage = LevelStage::StageMenu;
	m_gameMenu->resetCurrentButton();
	renderGameMenu();
}



void Game::renderGameMenu()
{
	SDL_RenderClear(renderer);

	drawBackground();

	playLabel.draw();
	exitLabel.draw();
	
	SDL_RenderPresent(renderer);
}

void Game::handleGameMenu()
{
	SDL_PollEvent(&event);

	SDL_Color red   = { 255,0,0,255 };
	SDL_Color white = { 255,0,0,255 };

	if (Game::event.type == SDL_KEYDOWN)
	{
		if (gameMenuCooldown == 0)
		{
			gameMenuCooldown = gameMenuDelay;
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_DOWN:
				m_gameMenu->keyDown();
				break;
			case SDLK_UP:
				m_gameMenu->keyUp();
				break;
			default:
				break;
			}
		}
	}

	if (Game::event.type == SDL_KEYUP)
	{
		if (gameMenuCooldown2 == 0)
		{
			gameMenuCooldown2 = gameMenuDelay;
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_DOWN:
				break;
			case SDLK_RETURN:
				activateMenuButton(m_gameMenu->getCurrentButton());
				break;

			case SDLK_ESCAPE:
				//m_gameMenu->changeMenuStatus(MenuStatus::GameClosed);
				//Game::isRunning = false;
				break;
			default:
				break;
			}
		}
	}

	std::cout << "Current Button is : " << m_gameMenu->getCurrentButton() << std::endl;
}

void Game::activateMenuButton(int currentButton)
{
	switch (currentButton)
	{
	case MenuButtons::Play:
		std::cout << "Play button was hit!" << std::endl;
		m_gameMenu->changeMenuStatus(MenuStatus::Finished);
		currentStage = LevelStage::StageRegular;
		break;
	case MenuButtons::Exit:
		std::cout << "Exit button was hit!" << std::endl;
		m_gameMenu->changeMenuStatus(MenuStatus::GameClosed);
		Game::isRunning = false;
		break;
	default:
		break;
	}
}

void Game::drawBackground()
{
	for (auto& t : tiles)
	{
		t->draw();
	}
}

void Game::LoadAssets()
{
	assets->AddTexture("space", "assets/space_map_tileset.png");

	assets->AddTexture("Ship", "assets/ship_anims.png");
	assets->AddTexture("Player_Attack", "assets/player_proj_8.png", 8);

	assets->AddTexture("Enemy_ship_Grey" , "assets/enemy_ship_anims_grey.png");
	assets->AddTexture("Enemy_ship_Green", "assets/enemy_ship_anims_green.png");
	assets->AddTexture("Enemy_ship_Red"  , "assets/enemy_ship_anims_red.png");
	assets->AddTexture("ShipAttack", "assets/Ship_Attack.png");
	assets->AddTexture("Enemy_Attack", "assets/enemy_proj_8.png", 8);
	assets->AddTexture("EnemyShipAttack", "assets/enemy_Ship_Attack.png");
	
	assets->AddTexture("BossCenterAttack", "assets/BossCentrAttack.png",8);
	assets->AddTexture("BossShip", "assets/BossShipAnims.png", 64);
	assets->AddTexture("BossUsualAttack", "assets/BossUsualAttack.png", 8);
	assets->AddTexture("BossRicochetAttack", "assets/BossRicochetAttack.png", 8);
	assets->AddTexture("BossDenchik", "assets/BossDen.png", 64);

	assets->AddFont("courier", "assets/cour.ttf", 48);

	map = new Map("space", MAP_TILE_SCALE, 32);
	map->LoadMap("assets/space_map.map", WIDTH_IN_TILES, HEIGHT_IN_TILES,walkinMap);
}

void Game::InitWeapon(WeaponParams * weapon,  SoundHandler * sounder)
{

}

void Game::WeaponsInit()
{
	/*	
	int range;
	int speed;
	int damage;
	int scale;
	int delay;
	std::string textureID;
	bool isAnimated = true;
	bool rotate = false;
	*/

	//Move to another function
	Attack PlayerAttack;

	PlayerAttack.range = 400;
	PlayerAttack.speed = 3;
	PlayerAttack.scale = 2;
	PlayerAttack.damage = 25;
	PlayerAttack.delay = 20;
	PlayerAttack.rotate = false;
	PlayerAttack.isAnimated = false;
	PlayerAttack.textureID = "Player_Attack";
	PlayerAttack.attackType = UsualAttack;
	PlayerAttack.attackSound = SoundHandler::soundUpAttack;
	PlayerAttack.projType = ProjectileType::StraightForward;

	//Initialize PlayerWeapon Type
	PlayerWeaponParams.pSounder = sounder;
	PlayerWeaponParams.attacks.emplace(UsualAttack, PlayerAttack);


	Attack EnemyAttack;

	EnemyAttack.range = 400;
	EnemyAttack.speed = 3;
	EnemyAttack.scale = 2;
	EnemyAttack.damage = 100;
	EnemyAttack.delay = 60;
	EnemyAttack.rotate = false;
	EnemyAttack.isAnimated = false;
	EnemyAttack.textureID = "Enemy_Attack";
	EnemyAttack.attackType = UsualAttack;
	EnemyAttack.attackSound = SoundHandler::soundEnemyAttack;
	PlayerAttack.projType = ProjectileType::StraightForward;

	//Initialize EnemyWeapon Type
	EnemyWeaponParams.pSounder = sounder;
	EnemyWeaponParams.attacks.emplace(UsualAttack, EnemyAttack);

	Attack BossUsualAttack;

	BossUsualAttack.range = 400;
	BossUsualAttack.speed = 3;
	BossUsualAttack.scale = 3;
	BossUsualAttack.damage = 100;
	BossUsualAttack.delay = 60;
	BossUsualAttack.rotate = false;
	BossUsualAttack.isAnimated = false;
	BossUsualAttack.textureID = "BossUsualAttack";
	BossUsualAttack.attackType = UsualAttack;
	BossUsualAttack.attackSound = SoundHandler::soundEnemyAttack;
	PlayerAttack.projType = ProjectileType::StraightForward;

	Attack BossCenterAttack;

	BossCenterAttack.range = 400;
	BossCenterAttack.speed = 3;
	BossCenterAttack.scale = 3;
	BossCenterAttack.damage = 100;
	BossCenterAttack.delay = 20;
	BossCenterAttack.rotate = false;
	BossCenterAttack.isAnimated = false;
	BossCenterAttack.textureID = "BossCenterAttack";
	BossCenterAttack.attackType = SpecialAttack;
	BossCenterAttack.attackSound = SoundHandler::soundEnemyAttack;
	PlayerAttack.projType = ProjectileType::StraightForward;

	Attack BossRicochetAttack;

	BossRicochetAttack.range = 1000;
	BossRicochetAttack.speed = 3;
	BossRicochetAttack.scale = 2;
	BossRicochetAttack.damage = 100;
	BossRicochetAttack.delay = 60;
	BossRicochetAttack.rotate = false;
	BossRicochetAttack.isAnimated = false;
	BossRicochetAttack.textureID = "BossRicochetAttack";
	BossRicochetAttack.attackType = RicochetAttack;
	BossRicochetAttack.attackSound = SoundHandler::soundEnemyAttack;
	BossRicochetAttack.projType = ProjectileType::Ricochet;

	//Initialize BossWeapon Type
	BossWeaponParams.pSounder = sounder;
	BossWeaponParams.attacks.emplace(UsualAttack, BossUsualAttack);
	BossWeaponParams.attacks.emplace(SpecialAttack, BossCenterAttack);
	BossWeaponParams.attacks.emplace(RicochetAttack, BossRicochetAttack);
	

	player_weapon = new Weapon(PlayerWeaponParams);
	enemy_weapon = new Weapon(EnemyWeaponParams);
	boss_weapon = new Weapon(BossWeaponParams);
}

void Game::playerInit()
{
	                          //rg, sp, dam,   texid,        delay, rot?,sounder,              sound,              anim, scale_ = 1
	//Getting an exception while the second game round
	//cuz player has been removed from manager? smth like this
	//need to get rid of using global player and access player from manager.getGroup(groupPlayers) in every place that calls player
	//then we might be able to create player again and again like enemies
	//since it has keyboard controller we will have the control...
	if (manager.getGroup(groupPlayers).empty())
	{
		auto& player(manager.addEntity());
		player.addGroup(groupPlayers);
	}
	for (auto& player : manager.getGroup(Game::groupPlayers))
	{
		//player.addGroup(groupPlayers);
		player->addComponent<TransformComponent>(610.0f, 815.0f, PLAYER_SCALE, 7);
		player->addComponent<SpriteComponent>("Ship", true);
		player->addComponent<TimingComponent>();
		player->addComponent<FigthComponent>(assets, player_weapon, Game::groupLabels::groupPlayerProjectiles);
		player->addComponent<ColliderComponent>("player");
		player->addComponent<HealthComponent>(100);
		player->addComponent<KeyboardController>(sounder);
	}
	
	
}

void Game::enemyInit(float xpos, float ypos, std::string texture)
{
	auto& enemy(manager.addEntity());                 //scale
	enemy.addGroup(gpoupEnemies);
	enemy.addComponent<TransformComponent>(xpos, ypos,    3,    6);
	enemy.addComponent<SpriteComponent>(texture, true);
	enemy.addComponent<FigthComponent>(assets, enemy_weapon, Game::groupLabels::groupEnemyProjectiles);
	enemy.addComponent<ColliderComponent>("enemy");
	enemy.addComponent<HealthComponent>();
	enemy.addComponent<LogicComponent>(LogicType::EasyLogic);
	
}

void Game::BossInit(float xpos, float ypos, const char * texture)
{
	auto& boss(manager.addEntity());
	boss.addGroup(groupBoss);
	boss.addComponent<TransformComponent>(xpos, ypos, assets->GetTextureWidth(texture), assets->GetTextureWidth(texture), 5, 1);
	boss.addComponent<SpriteComponent>(texture, false,assets->GetTextureWidth(texture));
	boss.addComponent<FigthComponent>(assets, boss_weapon, Game::groupLabels::groupEnemyProjectiles);
	boss.addComponent<ColliderComponent>("Boss");
	boss.addComponent<HealthComponent>(500);
	boss.addComponent<LogicComponent>(LogicType::BossLogic);
	
}

std::string PickTexture(int rand_id)
{
	/*
	"Enemy_ship_Grey" 
	"Enemy_ship_Green"
	"Enemy_ship_Red"
	*/
	switch (rand_id)
	{
	case 0:
		return std::string("Enemy_ship_Grey");
	case 1:
		return std::string("Enemy_ship_Green");
	case 2:
		return std::string("Enemy_ship_Red");
	default:
		break;
	}
}

void Game::InitLvl1(int Num_Enemies)
{
	std::uniform_int_distribution<int> xpos(70, 1115);
	std::uniform_int_distribution<int> TexId(0, 2);
	std::vector <int> positions;
	while(positions.size() != Num_Enemies)
	{
		int new_pos = xpos(rng);
		//Returns a pair consisting of an iterator to the inserted element 
		//(or to the element that prevented the insertion) and a bool value set to true if the insertion took place.
		bool fit = true;
		//Make sure Textures won't intersept 
		for (int pos : positions)
		{
			if (abs(new_pos - pos) < 32 * 3)
			{
				fit = false;
				break;
			}
		}
		if (fit)
		{
			positions.emplace_back(new_pos);
			enemyInit(new_pos, 70.0f, PickTexture(TexId(rng)));
		}
	}
}

void Game::killEnemies()
{
	//manager.getGroup(gpoupEnemies).clear();
	//manager.getGroup(groupBoss).clear();
	//manager.getGroup(gpoupWeapons).clear();
	//manager.getGroup(groupPlayerProjectiles).clear();
	//manager.getGroup(groupEnemyProjectiles).clear();
	//manager.getGroup(groupWeapons).clear();
	for (auto& e : enemies)
		e->destroy();
	for (auto& ep : EnemyProjectiles)
		ep->destroy();
	for (auto &pp : PlayerProjectiles)
		pp->destroy();
}

void Game::resetGame()
{
	//LoadAssets(); //also map gets created
	//manager.refresh();
	//manager.update();

	//Label Init
	///SDL_Color white = { 255,255,255,255 };
	///label.addComponent<UILabel>(10, 10, "Hello", "courier", white);
	//
	//

	killEnemies();

	manager.refresh();
	manager.update();
}

void Game::reInit()
{
	WeaponsInit();

	playerInit();

	InitLvl1(3);

	BossInit(500.0f, 70.0f, "BossShip");
}

bool Game::isPlayerActive()
{
	for (auto& player : manager.getGroup(Game::groupPlayers))
	{
		//make sure that there are only one player at a time in groupPlayes
		return player->isActive();
	}
	
}

