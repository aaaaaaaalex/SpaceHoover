#pragma once
#include "WorldObject.h"
#include "Visitor.h"

class Asteroid: public WorldObject {
public:
	Asteroid(int x, int y, int width, int height, int mass, Direction direc);
	void accept(Visitor &v);
};