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

SDL_Rect Game::camera = { 0,0, 0, 0 };

AssetManager* Game::assets = new AssetManager(&manager);
Menu* Game::m_gameMenu = nullptr;
ResolutionSettings* Game::m_resolutionSettings = nullptr;



bool Game::switch_on = true;
LevelStage Game::currentStage = LevelStage::StageMenu;

int Game::actualPlayWidth  = defaultPlayWidth;
int Game::actualPlayHeight = defaultPlayHeight;



bool Game::isRunning = false;
bool Game::pause = false;

int Game::commonPlayerSpeed = 3;

//auto& player(manager.addEntity());
auto& label(manager.addEntity());
//MenuLabels
auto& playLabel(manager.addEntity());
auto& exitLabel(manager.addEntity());
auto& score(manager.addEntity());


Game::Game()
{
	currentStage      = StageMenu;
	gameMenuCooldown  = gameMenuDelay;
	gameMenuCooldown2 = gameMenuDelay;

	sounder = new SoundHandler();
	m_resolutionSettings = new ResolutionSettings();

	m_resolutionSettings->calculateResolution(1920, 1080);
	camera.w = m_resolutionSettings->getWindowWidth();
	camera.h = m_resolutionSettings->getWindowHeight();
	m_resolutionSettings->printResolutionInfo();
}

Game::~Game()
{}

void Game::init(const char* title, int width, int height, bool fullscreen)
{
	//switch_on = true;
	int flags = 0;

	// --LEGACY
	m_playHeight = height;
	m_playWidth  = width;
	// --/LEGACY

	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystem Initialized..." << std::endl;
		//Use ResolutionSettings
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		if (window)
		{
			std::cout << "Window Created..." << std::endl;
			std::cout << "Window Width = " << WINDOW_WIDTH - (32 * MAP_TILE_SCALE) * 2 << " Window Height = " << WINDOW_HEIGHT - (32 * MAP_TILE_SCALE) * 2 << " | Player Scale = " << PLAYER_SCALE << std::endl;
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

	sounder->init();

	//Preparing data
	LoadAssets(); //also map gets created
	WeaponsInit();

	//kinda levels loading
	playerInit();
	InitLvl1(3);
	BossInit(Game::camera.w / 2 - (m_resolutionSettings->getWindowWidth() * heavyShipScreenCoef / 2), 5.0f, "BossShip");

	//Menu creation
	CreateMenu();
}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& enemies(manager.getGroup(Game::groupEnemies));
//auto& bosses(manager.getGroup(Game::groupBoss));
auto& PlayerProjectiles(manager.getGroup(Game::groupPlayerProjectiles));
auto& EnemyProjectiles(manager.getGroup(Game::groupEnemyProjectiles));
auto& Weapons(manager.getGroup(Game::gpoupWeapons));


void Game::handleEvents()
{
	for (auto& player : manager.getGroup(Game::groupPlayers))
	{
		if (player && !player->isActive())
		{
			resetGame();
			switchToGameMenu();
			new_game = true;
			return;
		}
	}

	sounder->handleEvents();

	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT :
		isRunning = false;
		break;
	default:
		break;
	}

	if (AllEnemiesAreDead())
	{
		changeLevelRequested = true;
	}

	if (lvl_change_timer == LVL_CHANGE)
	{
		lvl_change_timer = 0;
		ChangeLevel();
		changeLevelRequested = false;
	}

	if (changeLevelRequested)
	{
		++lvl_change_timer;
	}

	if (currentStage == LevelStage::StageBoss && changeLevelRequested == false)
	{
		if (enemies.size() == 0)
		{
			enemies.clear();

			enemies = manager.getGroup(Game::groupBoss);
			for (auto& e : enemies)
			{
				//e->changeGroupTo(Game::groupEnemies);
				e->addGroupOnly(Game::groupEnemies);
			}
			std::cout << "Added Boss to enemies" << std::endl;
		}
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
	m_scoreCounter.growScore(currentStage);

	//Player position from previous frame
	Vector2D prevPlayerPos;
	for (auto& player : manager.getGroup(Game::groupPlayers))
	{
		prevPlayerPos = player->getComponent<TransformComponent>().position;
	}

	for (auto& pr : EnemyProjectiles)
	{
		pr->getComponent<TransformComponent>().updPrevPostition();
	}

	SDL_Color white = { 255,255,255,255 };
	SDL_Color red = { 255,0,0,255 };

	//Caught dead enemies and calculate score before dead enemy will be removed
	if (manager.getNotActiveEntitiesNumber(groupEnemies) != 0)
	{
		m_scoreCounter.increaseScore(m_scoreCounter.calculateBonus(manager.getNotActiveEntities(groupEnemies)));
	}

	//update all components to the current frame state
	manager.refresh();
	manager.update();

	std::stringstream ss;
	ss << m_scoreCounter.getScoreStr();
	if (m_scoreCounter.getTimer() > 0)
	{
		score.getComponent<UILabel>().SetLabelText_Color(ss.str(), "courier", red);
	}
	else
	{
		score.getComponent<UILabel>().SetLabelText_Color(ss.str(), "courier", white);
	}


	//Hitting enemy handling
	for (auto& pr : PlayerProjectiles)
	{
		SDL_Rect prCol = pr->getComponent<ColliderComponent>().collider;
		for (auto& e : enemies)
		{
			SDL_Rect enCol = e->getComponent<ColliderComponent>().collider;
			if (e && Collision::AABB(prCol, enCol)) //player hit enemy
			{
				//std::cout << "player hit some enemy" << std::endl;
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
#ifndef DEBUG
	//Enemy hit player detection
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

	//getPlayerCollider
	SDL_Rect playerCol;
	for (auto& player : manager.getGroup(Game::groupPlayers))
	{
		playerCol = player->getComponent<ColliderComponent>().collider;
	}
	
	//
	SDL_Rect enemyCol;
	for (auto& c : colliders)
	{
		//player collision
		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(playerCol, cCol))
		{
			for (auto& player : manager.getGroup(Game::groupPlayers))
			{
				player->getComponent<TransformComponent>().position = prevPlayerPos;
			}
			
		}

		//enemies collision
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
			if (pr->getComponent<ProjectileComponent>().projType == ProjectileType::Ricochet) //Ricochet Handling
			{
				SDL_Rect prCol = pr->getComponent<ColliderComponent>().collider;
				if (Collision::AABB(prCol, cCol))
				{
					Vector2D prPos = pr->getComponent<TransformComponent>().position;
					if( (abs(m_resolutionSettings->getWindowHeight() - prPos.y) < 10 ||
					   abs(prPos.y - 0) < 10) )
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

	for (auto& c : colliders)
	{
		c->draw();
	}
	
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
	score.draw();
	
	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	m_scoreCounter.printScore();
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void Game::ChangeLevel()
{
	int num = currentStage;
	++num;

	currentStage = static_cast<LevelStage>(num);
	sounder->changeBackgroundSound(currentStage);

	if (currentStage > LevelStage::StageBoss)
	{
		currentStage = LevelStage::StageMenu;
		for (auto& player : manager.getGroup(groupPlayers))
		{
			player->destroy();
		}
		new_game = true;
	}

	m_scoreCounter.resetTimer();
}

void Game::CreateMenu()
{
	m_gameMenu = new Menu();
}

void Game::prepareGameMenu()
{
	SDL_Color white = { 255,255,255,255 };
	playLabel.addComponent<UILabel>(m_playWidth / 2 - 48, m_playHeight / 2 - 200,      "play", "courier", white);
	playLabel.addGroup(groupTexts);
	exitLabel.addComponent<UILabel>(m_playWidth / 2 - 48, m_playHeight / 2 - 200 + 48, "exit", "courier", white);
	exitLabel.addGroup(groupTexts);
	score.addComponent<UILabel>(0, m_playHeight - 50, "0","courier", white);
	score.addGroup(groupTexts);

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
	//TBD: Implement stages handling system
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

	//std::cout << "Current Button is : " << m_gameMenu->getCurrentButton() << std::endl;
}

void Game::activateMenuButton(int currentButton)
{
	switch (currentButton)
	{
	case MenuButtons::Play:
		//std::cout << "Play button was hit!" << std::endl;
		m_gameMenu->changeMenuStatus(MenuStatus::Finished);
		ChangeLevel();
		//for (auto& e : manager.getGroup(groupTexts))
		//	e->destroy();
		break;
	case MenuButtons::Exit:
		//std::cout << "Exit button was hit!" << std::endl;
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
	assets->AddTexture("space_16_9", "assets/space_16_9.png");

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

	map = new Map("space_16_9", 1, m_resolutionSettings->getBaseTileSize());
	map->LoadMap("assets/space_map_HD.map", //the map is not used, TO DO: refactor this shit
		m_resolutionSettings->getWidthInTiles(),
		m_resolutionSettings->getHeightInTiles(),
		m_resolutionSettings->getWindowWidth(),
		m_resolutionSettings->getWindowHeight()
	);
}

void Game::InitWeapon(WeaponParams * weapon,  SoundHandler * sounder)
{

}

void Game::WeaponsInit()
{
	//TBD: Implement some smart Weapon system with loading data from binary file
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
	PlayerAttack.scale = 0.001;
	PlayerAttack.screenFactor = 0.01;
	PlayerAttack.damage = 25;
#ifdef DEBUG
	PlayerAttack.damage = 25000000;
#endif // DEBUG
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
	EnemyAttack.screenFactor = 0.01;
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
	BossUsualAttack.screenFactor = 0.01;
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
	BossCenterAttack.screenFactor = 0.01;
	BossCenterAttack.damage = 100;
	BossCenterAttack.delay = 20;
	BossCenterAttack.rotate = false;
	BossCenterAttack.isAnimated = false;
	BossCenterAttack.textureID = "BossCenterAttack";
	BossCenterAttack.attackType = SpecialAttack;
	BossCenterAttack.attackSound = SoundHandler::soundEnemyAttack;
	PlayerAttack.projType = ProjectileType::StraightForward;

	Attack BossRicochetAttack;

	BossRicochetAttack.range = 1600;
	BossRicochetAttack.speed = 3;
	BossRicochetAttack.screenFactor = 0.01;
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
	float playerXpos = Game::camera.w / 2 - (Game::camera.w * mediumShipScreenCoef / 2);
	float playerYpos = Game::camera.h * 0.7;
	if (manager.getGroup(groupPlayers).empty())
	{
		auto& player(manager.addEntity());
		player.addGroup(groupPlayers);
	}
	for (auto& player : manager.getGroup(Game::groupPlayers))
	{
		//player.addGroup(groupPlayers);
		player->addComponent<TransformComponent>(playerXpos, playerYpos, mediumShipScreenCoef , 8);
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
	enemy.addGroup(groupEnemies);
	enemy.addComponent<TransformComponent>(xpos, ypos, mediumShipScreenCoef,    7);
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
	boss.addComponent<TransformComponent>(xpos, ypos, assets->GetTextureWidth(texture), assets->GetTextureWidth(texture), heavyShipScreenCoef, 1);
	boss.addComponent<SpriteComponent>(texture, true,assets->GetTextureWidth(texture));
	boss.addComponent<FigthComponent>(assets, boss_weapon, Game::groupLabels::groupEnemyProjectiles);
	boss.addComponent<ColliderComponent>("Boss");
	boss.addComponent<HealthComponent>(400);
	boss.addComponent<LogicComponent>(LogicType::BossLogic);
	
}

std::string PickTexture(int rand_id)
{
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
	//TBD: Implement some smart Level System generating the next level with progressive difficulty
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
			//TBD: get rid of hardcoded value, need to get currently loading ship sizes , probably it will be a part of Level System
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
	for (auto& e : enemies)
		e->destroy();
	for (auto& ep : EnemyProjectiles)
		ep->destroy();
	for (auto &pp : PlayerProjectiles)
		pp->destroy();
}

void Game::resetGame()
{
	killEnemies();

	manager.refresh();
	manager.update();
}

void Game::reInit()
{
	WeaponsInit();

	playerInit();

	InitLvl1(3);

	m_scoreCounter.resetScore();

	BossInit(Game::camera.w / 2 - (m_resolutionSettings->getWindowWidth() * heavyShipScreenCoef / 2), 5.0f,"BossShip");
}

bool Game::isPlayerActive()
{
	for (auto& player : manager.getGroup(Game::groupPlayers))
	{
		//TBD: make sure that there are only one player at a time in groupPlayes
		if (manager.getGroup(Game::groupPlayers).size() == 1)
			std::cout << "ONE PLAYER NOW! " << std::endl;

		if(player)
			return player->isActive();
	}
}

int Score::calculateBonus(std::vector<const Entity*> deadEntities)
{
	int bonus = 0;

	for (const auto& entity : deadEntities)
	{
		bonus += entity->getComponent<HealthComponent>().getMaxHP();
	}

	return getTimer() > 0 ? bonus * 2 : bonus;
}


void Score::printScore()
{
	std::cout << "Your Score is:" << m_score << std::endl;
}

std::string Score::getScoreStr()
{
	return std::to_string(static_cast<int>(m_score));
}

void Score::printTimer()
{
	std::cout << bonusTimer << std::endl;
}