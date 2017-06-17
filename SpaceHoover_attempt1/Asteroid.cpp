#include "Asteroid.h"

Asteroid::Asteroid(int x, int y, int width, int height, int mass, Direction direc):
	WorldObject(x, y, width, height)
{
	this->setMass(mass);
	this->direction = direc;
}

void Asteroid::accept(Visitor & v)
{
	v.visit(this);
}
