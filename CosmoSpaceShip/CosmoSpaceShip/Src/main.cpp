
#include "Game.h"

Game *game = nullptr;
SDL_Event main_event;

int main(int argc, char *argv[])
{

	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	game = new Game();
	game->init("GameWindow", WINDOW_WIDTH - 32 * MAP_TILE_SCALE * 2, WINDOW_HEIGHT - 32 * MAP_TILE_SCALE * 2, false);

	while (game->running())
	{
		//Handling of 1 Frame

		frameStart = SDL_GetTicks();

		game->handleEvents();

		if (Game::isRunning)
		{
			game->update();
		}
		else
			break;
		if (Game::isRunning)
			game->render();
		else
			break;

		frameTime = SDL_GetTicks() - frameStart;

		//Stitck to our FPS
		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}

	}

	game->clean();
	std::cout << "Game Cleaned" << std::endl;
	return 0;
}