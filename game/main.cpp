#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "game.h"

SDL_AppResult SDL_AppInit(void** appState, int argc, char* argv[]) {
	SDL_SetAppMetadata("Untitled Game", "0.1", "com.draker.opengl-game");
	
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
		SDL_Log("SDL Initialization Failed: %s", SDL_GetError());
		return SDL_APP_FAILURE;
	}
	
	game* gameState = new game;
	
	*appState = gameState;
	
	gameState->init();
	
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appState) {
	game* gameState = static_cast<game*>(appState);
	
	gameState->update();
	gameState->render();
	
	beginInputFrame();
	
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appState, SDL_Event* event) {
	game* gameState = static_cast<game*>(appState);
	
	gameState->processInput(*event);
	
	if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
	
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appState, SDL_AppResult result) {
	game* gameState = static_cast<game*>(appState);
	
	if (gameState) {
		gameState->terminate();
		delete gameState;
	}
	
	SDL_Quit();
}
