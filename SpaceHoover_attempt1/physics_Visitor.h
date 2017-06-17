#pragma once
#include "Visitor.h"
#include "SFX_Module.h"
#include "Asteroid.h"
#include "Star.h"
#include "Player.h"
#include "Weapon.h"

struct CollisionLog {
	WorldObject* obj;
	int x;
	int y;
	int w;
	int h;
};

class physics_Visitor: public Visitor {
public:
	// This visitor enables the chasing of a target
	physics_Visitor();
	~physics_Visitor();

	// VISITS
	void visit(Asteroid * a);
	void visit(Star * a);
	void visit(Player * p);
	void visit(Weapon * w);

	//sets
	void setTarget(int xy[2]);
	void setBounds(int maxX, int maxY);
	void setGravitationalConstant(double g);

	CollisionLog* getCollision(WorldObject* o);
	void wipeCollisionLogs();
	void addCollisionLog(CollisionLog* log);

	void setSFXModule(SFX_Module* m);

	// gets
	bool getStarsGone();
private:
	int target[2];
	double gravitational_constant;
	int effective_distance;
	int maxX;
	int maxY;
	bool stars_all_gone;

	CollisionLog* collision_logs[600];
	int logs_size = 0;

	SFX_Module* sfx;
};