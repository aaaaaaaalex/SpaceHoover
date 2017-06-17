#pragma once

class Player;
class Asteroid;
class Weapon;
class Star;

class Visitor {
public:
	virtual void visit(Player *p) = 0;
	virtual void visit(Asteroid *a) = 0;
	virtual void visit(Weapon *w) = 0;
	virtual void visit(Star *s) = 0;
};