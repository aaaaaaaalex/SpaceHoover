#include "World.h"

void World::setRenderer(SDL_Renderer* r) {
	this->input_visitor.setWorld(this);
	this->renderer = r;
	this->render_visitor.setRenderer(r);
}

void World::setWorldObjects(std::vector<WorldObject*>* o) {
	this->objects = *o;
}

void World::addWorldObject(WorldObject* o) {
	this->objects.push_back(o);
}
void World::addWorldObject(Player * p)
{
	this->player = p;
	int xy[] = {p->getX(), p->getY()};
	this->physics_visitor.setTarget(xy);
}

void World::setFinished(bool f)
{
	this->finished = f;
}

bool World::getFinished()
{
	return this->finished;
}

void World::setWon(bool w)
{
	this->won = w;
}

bool World::getWon()
{
	return this->won;
}

void World::advancePhysics() {
	int xy[] = {player->getX() + (player->getW()), player->getY() + (player->getH()/2)};
	this->physics_visitor.setTarget(xy);
	for (unsigned i = 0; i < this->objects.size(); i++) {
		WorldObject* o = objects.at(i);
		o->accept(this->physics_visitor);
	}
	if (!player->isDestroyed())(*player).accept(this->physics_visitor);
	// is the level finished? 
	else this->finished = true;
	if (physics_visitor.getStarsGone()) {
		finished = true;
		won = true;
	}
	physics_visitor.wipeCollisionLogs();
}

void World::advanceRender(){
	SDL_RenderClear(this->renderer);
	this->render_visitor.drawBounds();
	for (unsigned i = 0; i < this->objects.size(); i++) {
		WorldObject* o = objects.at(i);
		if (!o->getRemovedFromPlay()) (*o).accept(this->render_visitor);
	}
	(*player).accept(this->render_visitor);

	// lose / win message
	if (this->finished) {
		if (this->won) {
			this->render_visitor.drawYouWin();
		}
		else {
			this->render_visitor.drawYouLose();
		}
	}
	else {
		if (this->won) this->render_visitor.drawYouLose();
	}

	SDL_RenderPresent(this->renderer);
	sound_module.processQueue();
}

void World::advanceInput()
{
	(*player).accept(this->input_visitor);
}

void World::acceptSetup(Level * l)
{
	this->physics_visitor.setBounds((*l).getMaxX(), (*l).getMaxY());
	this->render_visitor.setBounds((*l).getMaxX(), (*l).getMaxY());
	(*l).visitSetup(this);
}

void World::clearSetup()
{
	this->finished = false;
	this->won = false;

	for (unsigned i = 0; i < this->objects.size(); i++) {
		delete this->objects.at(i);
	}
	this->objects.clear();
	delete this->player;
}

// CONSTRUCTOR
World::World(SDL_Renderer * r)
{
	this->renderer = r;
	this->render_visitor.setRenderer(r);
	this->input_visitor.setWorld(this);
	this->physics_visitor.setSFXModule(&sound_module);
}

World::~World()
{
	clearSetup();
	SDL_DestroyRenderer(this->renderer);
}
