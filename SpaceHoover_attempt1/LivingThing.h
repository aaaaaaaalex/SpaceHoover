#pragma once
#include "WorldObject.h"


class LivingThing : public WorldObject{
public:
	LivingThing(int x, int y, int width, int height, int health, int speed);

	virtual void accept(Visitor &v) = 0;

	void setHealth(int h);
	int getHealth();

	void setIsShooting(bool s);
	bool getIsShooting();

	void setMaxHealth(int h);
	int getMaxHealth();

private:
	int health;
	int max_health;
	bool is_shooting;
};