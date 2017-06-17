#pragma once
#include "Visitor.h"

#include "Player.h"
#include "Asteroid.h"
#include "Star.h"
#include "Weapon.h"
#include <SDL_events.h>
class World;

class input_Visitor :
	public Visitor
{
public:
	input_Visitor();
	~input_Visitor();

	void visit(Player* p);
	void visit(Asteroid* a);
	void visit(Star* s);
	void visit(Weapon* w);
	void setWorld(World* w);
private:
	SDL_Event mainEvent;
	SDL_Event previous_event;
	World* world;
};

