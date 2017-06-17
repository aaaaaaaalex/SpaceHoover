#include "Star.h"

Star::Star() :
	WorldObject(100, 100, 5, 5)
{
	this->type = BLUE;
	this->setMass(100);
}

Star::Star(int x, int y, int width, int height, StarType type, int mass):
	WorldObject(x, y, width, height)
{
	this->type = type;
	this->setMass(mass);
}

void Star::accept(Visitor & v)
{
	v.visit(this);
}
