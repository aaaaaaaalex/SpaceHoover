#pragma once

#include <SDL_render.h>
#include <vector>
#include "render_Visitor.h"
#include "physics_Visitor.h"
#include "input_Visitor.h"
#include "SFX_Module.h"
#include "Level.h"
#include "WorldObject.h"
#include "Player.h"

class World {
public:
	World(SDL_Renderer* r);
	~World();

	//sets, adds
	void setRenderer(SDL_Renderer* r);
	void setWorldObjects(std::vector<WorldObject*>* o);
	void addWorldObject(WorldObject* o);
	void addWorldObject(Player* p);
	void setFinished(bool f);
	bool getFinished();
	void setWon(bool w);
	bool getWon();

	//physics & game mechanics
	void advancePhysics();
	void advanceRender();
	void advanceInput();

	// Accept a level for setup
	void acceptSetup(Level* l);

	// delete all heap entities currently referenced in the World. All stored entities should be on the heap.
	void clearSetup();

private:
	std::vector<WorldObject*> objects;
	Player* player; // a pointer to player is also found in 'objects', so that visitors act on it like every other object
	SDL_Renderer* renderer;

	// visitors / modules
	render_Visitor render_visitor;
	physics_Visitor physics_visitor;
	input_Visitor input_visitor;
	SFX_Module sound_module;

	bool finished = false;
	bool won = false;
};