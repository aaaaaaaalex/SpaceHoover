#pragma once
#include <vector>
#include <SDL_render.h>

// will accept visitor objects for inter-object interaction

enum Direction { LEFT, RIGHT, UP, DOWN };

class WorldObject {

public:
	Direction direction;

	WorldObject();
	WorldObject(int x, int y, int width, int height);
	~WorldObject();

	static int WORLDOBJECT_COUNT;

	virtual void accept(class Visitor &v) = 0;

	//gets
	int getObjectID();
	double getX();
	double getY();
	int getW();
	int getH();
	double getSpeed();
	int getMass();
	bool getRemovedFromPlay();
	int getAnimationStepCount();
	double getAnimationStepRate();

	//sets
	void setX(double x);
	void setY(double y);
	void setW(int w);
	void setH(int h);
	void setObjectID(int id);
	void setSpeed(double s);
	void setMass(int m);

	void setAnimationStepCount(int count);
	void setAnimationStepRate(double rate);
	void setRemovedFromPlay(bool r);

	bool isDestroyed();
	void setDestroyed(bool val);

	bool isMoving();
	void setMoving(bool m);

private:
	int objectID;

	bool is_moving;
	bool is_destroyed;
	bool is_removed_from_play;

	//dimensions
	double xPos = 0;
	double yPos = 0;
	int w = 0;
	int h = 0;
	double speed = 1;
	int mass = 10;

	double animation_step_rate; // per second
	int animation_step_count;

protected:
	std::vector<WorldObject*> heldObjects;
};