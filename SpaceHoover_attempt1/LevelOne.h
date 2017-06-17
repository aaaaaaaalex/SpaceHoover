#pragma once
#include "Level.h"

#include "World.h"
#include "Player.h"
#include "Star.h"
#include "Asteroid.h"
#include "Player.h"

#include <stdlib.h>
#include <ctime>

// this class will contain all mechanics that operate above the world.
// each level will have unique references and methods for running the game loop
// this allows different rules to be implemented from level to level.
class LevelOne : public Level
{
public:
	LevelOne();
	~LevelOne();

	void visitSetup(World* w);
	LevelBreakType startLevel();

private:
	int asteroid_count;
	int star_count;
	int player_starting_coords[2];
	Player* player;
};

