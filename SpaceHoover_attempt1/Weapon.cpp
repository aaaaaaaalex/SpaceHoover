#include "Weapon.h"

Weapon::Weapon(int x, int y, int width, int height, int damage):
	WorldObject(x, y, width, height)
{
	this->damage = damage;
}

void Weapon::accept(Visitor & v)
{
	v.visit(this);
}
