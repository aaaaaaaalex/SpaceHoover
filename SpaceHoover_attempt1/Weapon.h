#pragma once
#include "WorldObject.h"
#include "Visitor.h"

class Weapon : public WorldObject {
public:
	Weapon(int x, int y, int width, int height, int damage);
	/*virtual*/void accept(Visitor &v);

private:
	int damage;
};