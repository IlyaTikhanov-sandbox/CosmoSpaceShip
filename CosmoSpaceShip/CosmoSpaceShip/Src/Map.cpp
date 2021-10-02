#include "Map.h"
#include "Game.h"
#include <fstream>
#include "ECS\ECS.h"
#include "ECS\Components.h"

extern Manager manager;

Map::Map(std::string tID,int ms,int ts):texID(tID),mapScale(ms),tileSize(ts)
{
	scaledSize = mapScale * tileSize;
}

Map::~Map()
{

}

void Map::LoadMap(std::string path, int sizeX, int sizeY, int window_widgth, int window_height)
{
	char c;

	int srcX, srcY;

	for (int y = -1; y < sizeY + 1; ++y)
	{
		for (int x = -1; x < sizeX + 1; ++x)
		{
			if (isCollidingBorder(x,y,window_widgth,window_height))
			{
				auto& tcol(manager.addEntity());
				tcol.addComponent<ColliderComponent>("terrain", x * scaledSize, y * scaledSize, scaledSize);
				tcol.addGroup(Game::groupColliders);
			}
			else
			{
				srcY = 0;
				srcX = 0;
				AddTile(srcX, srcY, x * (tileSize * mapScale), y * (tileSize * mapScale));
			}
		}
	}

	/*
	for (int y = 0; y < sizeY; ++y)
	{
		for (int x = 0; x < sizeX; ++x)
		{
			if (c == '1')
			{
				auto& tcol(manager.addEntity());
				tcol.addComponent<ColliderComponent>("terrain", x * scaledSize, y * scaledSize, scaledSize);
				tcol.addGroup(Game::groupColliders);
			}
		}
	}
	*/
	
}

void Map::AddTile(int srcX, int srcY, int xpos, int ypos)
{
	auto & tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize,mapScale,texID);
	tile.addGroup(Game::groupMap);
}

bool Map::isCollidingBorder(int x, int y, int window_widgth, int window_height)
{
	return  x == -1 || y == -1 || x * scaledSize >= window_widgth || y * scaledSize >= window_height;
}
