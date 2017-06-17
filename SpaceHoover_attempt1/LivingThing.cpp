#include "LivingThing.h"

LivingThing::LivingThing(int x, int y, int width, int height, int health, int speed):
	WorldObject(x, y, width, height)
{
	this->health = health;
	this->max_health = health;
	this->is_shooting = false;
	this->direction = DOWN;
}

void LivingThing::setHealth(int h)
{
	this->health = h;
	if (h <= 0) setDestroyed(true);
}

int LivingThing::getHealth()
{
	return this->health;
}

void LivingThing::setIsShooting(bool s)
{
	this->is_shooting = s;
}

bool LivingThing::getIsShooting()
{
	return this->is_shooting;
}

void LivingThing::setMaxHealth(int h)
{
	this->max_health = h;
}

int LivingThing::getMaxHealth()
{
	return this->max_health;
}
