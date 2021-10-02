#pragma once
#include <vector>
#include <string>

class Map
{
public:

	Map(std::string tID, int ms, int ts);
	~Map();

	void LoadMap(std::string path, int sizeX, int sizeY, int window_widgth, int window_height);
	void AddTile(int srcX, int srcY, int xpos, int ypos);
	bool isCollidingBorder(int x, int y, int window_widgth, int window_height);

private:
	std::string texID;
	int mapScale;
	int tileSize;
	int scaledSize;

};