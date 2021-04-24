#include "AssetManager.h"
#include "ECS/Components.h"

AssetManager::AssetManager(Manager * man):manager(man)
{
}

AssetManager::~AssetManager()
{
}

void AssetManager::CreateProjectile(Vector2D pos, int width, Vector2D vel, int range, int speed, int damage, std::string id, bool isAnimated, bool flip, int scale_, Game::groupLabels pr_group,ProjectileType prType)
{
	int height = width;
	auto & projectile(manager->addEntity());
	projectile.addComponent<TransformComponent>(pos.x, pos.y, width, height, scale_);
	projectile.addComponent<SpriteComponent>(id, isAnimated);
	projectile.addComponent<ProjectileComponent>(range, speed,damage,vel,prType);
	projectile.addComponent<ColliderComponent>("projectile");
	projectile.addGroup(pr_group);
	if (flip)
		projectile.getComponent<SpriteComponent>().spriteFlip = SDL_FLIP_HORIZONTAL;
	if (isAnimated)
	{
		projectile.getComponent<SpriteComponent>().Play(Anims::Idle);
	}
}

void AssetManager::AddTexture(std::string id, const char * path, int width)
{
	std::pair<SDL_Texture*, int> tempTexture = std::make_pair(TextureManager::LoadTexture(path), width);
	textures.emplace(id, tempTexture);
}

SDL_Texture * AssetManager::GetTexture(std::string id)
{
	return textures[id].first;
}

void AssetManager::AddFont(std::string id, std::string path, int fontSize)
{
	fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}

TTF_Font * AssetManager::GetFont(std::string id)
{
	return fonts[id];
}
