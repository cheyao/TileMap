#include <SDL3/SDL.h>
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <cstdlib>
#include <ctime>

#include "game.hpp"

int SDL_AppInit(void **appstate, int argc, char **argv) {
	(void)argc;
	(void)argv;

	srand(time(nullptr));

	Game *game = new Game();
	game->init();
	*appstate = game;

	return 0;
}

int SDL_AppEvent(void *appstate, const SDL_Event *event) {
	return ((Game *)appstate)->event(*event);
}

int SDL_AppIterate(void *appstate) { 
	return ((Game *)appstate)->iterate(); 
}

void SDL_AppQuit(void *appstate) {
	delete ((Game *)appstate);
}

