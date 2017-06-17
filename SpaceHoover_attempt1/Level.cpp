#include "Level.h"

int Level::getMaxX()
{
	return this->level_dimensions[0];
}

int Level::getMaxY()
{
	return this->level_dimensions[1];
}

void Level::setMaxX(int x)
{
	this->level_dimensions[0] = x;
}

void Level::setMaxY(int y)
{
	this->level_dimensions[1] = y;
}
