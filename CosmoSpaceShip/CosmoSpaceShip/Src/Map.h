#pragma once
#include <vector>
#include <string>

class Map
{
public:

	Map(std::string tID, int ms, int ts);
	~Map();

	void LoadMap(std::string path, int sizeX, int sizeY, int window_widgth, int window_height);
	void LoadMapOneTile(int windowWidth, int windowHeight);
	void AddTile(int srcX, int srcY, int xpos, int ypos);
	void AddTileSized(int srcX, int srcY, int xpos, int ypos, int w, int h);
	bool isCollidingBorder(int x, int y, int window_widgth, int window_height);
private:
	std::string texID;
	int mapScale;
	int tileSize;
	int scaledSize;

};