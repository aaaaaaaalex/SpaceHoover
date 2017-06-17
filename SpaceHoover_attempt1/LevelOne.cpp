#include "LevelOne.h"
#include <random>

LevelOne::LevelOne()
{
	this->setMaxX(1000);
	this->setMaxY(1000);

	this->asteroid_count = 10;
	this->star_count = 1000;
	this->player_starting_coords[0] = 400;
	this->player_starting_coords[1] = 400;
}
LevelOne::~LevelOne()
{
	(*world).clearSetup();
}

void LevelOne::visitSetup(World * w)
{
	this->world = w;

	//add player to world
	Player* p = new Player(player_starting_coords[0], player_starting_coords[1], 100, 150, 100, 10);
	this->player = p;
	(*w).addWorldObject(p);
	srand(1);

	// ASTEROIDS
	int x_rand;
	int y_rand;
	int w_rand;
	int mass_rand;
	double animation_step_rate_rand;
	Direction dir_rand;

	srand(clock());
	for (int i = 0; i < asteroid_count; i++) {
		x_rand = (rand() % level_dimensions[0] + 1);
		y_rand = (rand() % level_dimensions[1] + 1);
		w_rand = (rand() % 20)+30;
		mass_rand = w_rand;
		animation_step_rate_rand = (rand() % 5);

		int dir = rand() % 4;
		switch (dir) {
		case 0:
			dir_rand = UP;
			break;
		case 1:
			dir_rand = DOWN;
			break;
		case 2:
			dir_rand = LEFT;
			break;
		case 3:
			dir_rand = RIGHT;
			break;
		}
		Asteroid* a = new Asteroid(x_rand, y_rand, w_rand, w_rand, mass_rand, dir_rand);
		a->setAnimationStepRate(animation_step_rate_rand);
		(*w).addWorldObject(a);
	}

	// STARS
	StarType type_rand;
	int selector;
	for (int i = 0; i < star_count; i++) {
		selector = (rand() % 4)+1;
		switch (selector) {
		case 1:
			type_rand = BLUE;
			break;
		case 2:
			type_rand = RED;
			break;
		case 3:
			type_rand = YELLOW;
			break;
		case 4:
			type_rand = PURPLE;
			break;
		}
		x_rand = (rand() % level_dimensions[0]);
		y_rand = (rand() % level_dimensions[1]);
		w_rand = (rand() % 7) + 1;
		mass_rand = 10000;
		(*w).addWorldObject(new Star(x_rand, y_rand, w_rand, w_rand, type_rand, mass_rand) );
	}
}

LevelBreakType LevelOne::startLevel()
{
	LevelBreakType game_status = FAIL;

	bool finished = false;
	while (!finished) {
		(*world).advancePhysics();
		(*world).advanceRender();
		(*world).advanceInput();

		finished = world->getFinished();
		if (world->getWon()) break;
	}

	if (this->player->isDestroyed()) game_status = FAIL;
	else {
		if (finished == false) {
			game_status = SAVE_AND_QUIT;
		}
		else if (finished == true) {
			game_status = COMPLETE;
		}
	}

	return game_status;
}
