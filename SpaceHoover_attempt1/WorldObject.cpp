#include "WorldObject.h"


int WorldObject::WORLDOBJECT_COUNT = 0;

WorldObject::WorldObject()
{
	is_destroyed = false;

	this->objectID = WorldObject::WORLDOBJECT_COUNT;
	WorldObject::WORLDOBJECT_COUNT++;

	this->xPos = 0;
	this->yPos = 0;
	this->w = 100;
	this->h = 150;

	this->animation_step_count = 5;
	this->animation_step_rate = 0.3;

	this->is_removed_from_play = false;
}

WorldObject::WorldObject(int x, int y, int width, int height)
{
	this->objectID = WorldObject::WORLDOBJECT_COUNT;
	WorldObject::WORLDOBJECT_COUNT++;

	this->xPos = x;
	this->yPos = y;
	this->w = width;
	this->h = height;

	this->animation_step_count = 5;
	this->animation_step_rate = 0.5;

	this->is_removed_from_play = false;
}

WorldObject::~WorldObject()
{
	WorldObject::WORLDOBJECT_COUNT--;
}

void WorldObject::setObjectID(int id)
{
	this->objectID = id;
}

void WorldObject::setSpeed(double s)
{
	this->speed = s;
}

void WorldObject::setMass(int m)
{
	this->mass = m;
}

void WorldObject::setAnimationStepCount(int count)
{
	this->animation_step_count = count;
}

void WorldObject::setAnimationStepRate(double rate)
{
	this->animation_step_rate = rate;
}

void WorldObject::setRemovedFromPlay(bool r)
{
	this->is_removed_from_play = r;
}

bool WorldObject::isDestroyed()
{
	return this->is_destroyed;
}

void WorldObject::setDestroyed(bool val)
{
	this->is_destroyed = val;
}

bool WorldObject::isMoving()
{
	return this->is_moving;
}

void WorldObject::setMoving(bool m)
{
	this->is_moving = m;
}

int WorldObject::getObjectID()
{
	return this->objectID;
}

double WorldObject::getX()
{
	return this->xPos;
}

double WorldObject::getY()
{
	return this->yPos;
}

int WorldObject::getW()
{
	return this->w;
}

int WorldObject::getH()
{
	return this->h;
}

double WorldObject::getSpeed()
{
	return this->speed;
}

int WorldObject::getMass()
{
	return this->mass;
}

bool WorldObject::getRemovedFromPlay()
{
	return this->is_removed_from_play;
}

int WorldObject::getAnimationStepCount()
{
	return this->animation_step_count;
}

double WorldObject::getAnimationStepRate()
{
	return this->animation_step_rate;
}

void WorldObject::setX(double x)
{
	this->xPos = x;
}

void WorldObject::setY(double y)
{
	this->yPos = y;
}

void WorldObject::setW(int w)
{
	this->w = w;
}

void WorldObject::setH(int h)
{
	this->h = h;
}
