#pragma once

#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS/ECS.h"
#include "SDL_ttf.h"

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

	void AddFont(std::string id, std::string path, int fontSize);
	TTF_Font* GetFont(std::string id);

private:
	Manager* manager;
	std::map<std::string, std::pair<SDL_Texture*, int>> textures;
	std::map<std::string, TTF_Font*> fonts;
	int cooldown = 60;
};
