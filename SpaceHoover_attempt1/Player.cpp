#include "Player.h"

Player::Player(int x, int y, int width, int height, int health, int speed):
	LivingThing(x, y, width, height, health, speed)
{
	this->setAnimationStepRate(3);
	this->setMass(100000);
}

void Player::accept(Visitor & v)
{
	v.visit(this);
}

double Player::getXSpeed()
{
	return this->xSpeed;
}

double Player::getYSpeed()
{
	return this->ySpeed;
}

void Player::setXSpeed(double s)
{
	this->xSpeed = s;
}

void Player::setYSpeed(double s)
{
	this->ySpeed = s;
}


