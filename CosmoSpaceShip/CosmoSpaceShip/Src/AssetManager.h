#pragma once

#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS/ECS.h"

class AssetManager
{
public:
	AssetManager(Manager* man);
	~AssetManager();

	//Game Objects
	void CreateProjectile(Vector2D pos, int width, Vector2D vel, int range,
		int speed, int damage, std::string id, bool isAnimated, bool flip,
		int scale_, Game::groupLabels pr_group, ProjectileType prType);

	//Texture management
	void AddTexture(std::string id, const char* path, int width = 32);
	SDL_Texture* GetTexture(std::string id);
	int GetTextureWidth(std::string id)
	{
		return textures[id].second;
	}

private:
	Manager* manager;
	std::map<std::string, std::pair<SDL_Texture*, int>> textures;
	int cooldown = 60;
};
