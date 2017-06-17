#pragma once
#include "LivingThing.h"
#include "Visitor.h"

class Player : public LivingThing {
public:
	Player(int x, int y, int width, int height, int health, int speed);
	/*virtual*/void accept(Visitor &v);

	double getXSpeed();
	double getYSpeed();

	void setXSpeed(double s);
	void setYSpeed(double s);
private:
	double xSpeed;
	double ySpeed;
};