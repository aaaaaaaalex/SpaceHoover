#include "World.h"
#include "LevelOne.h"

#include <SDL.h>
#include <SDL_Image.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <ctime>
#include <iostream>

int main(int argc, char* args[]) {
	int sdl_init_code = SDL_Init(SDL_INIT_EVERYTHING);
	printf("SDL initialized with code: %d\n", sdl_init_code);

	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;

	SDL_Window* window = SDL_CreateWindow("space hoover", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	World world(renderer);
	Level* level = new LevelOne();

	LevelBreakType game_status = NONE;
	while (!(game_status == SAVE_AND_QUIT)){
		game_status = NONE;
		world.clearSetup();
		world.acceptSetup(level);
		game_status = level->startLevel();
		SDL_Delay(1000);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	delete level;
	return 0;
}