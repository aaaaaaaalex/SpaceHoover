#include "input_Visitor.h"
#include "World.h"

input_Visitor::input_Visitor()
{
}
input_Visitor::~input_Visitor()
{
}

void input_Visitor::visit(Player * p)
{
	SDL_PollEvent(&mainEvent);

	if (mainEvent.type == SDL_KEYDOWN && (mainEvent.type != previous_event.type)) {
		switch (mainEvent.key.keysym.sym) {
		case SDLK_a:
			(*p).direction = LEFT;
			(*p).setMoving(true);
			break;
		case SDLK_s:
			(*p).direction = DOWN;
			(*p).setMoving(true);
			break;
		case SDLK_d:
			(*p).direction = RIGHT;
			(*p).setMoving(true);
			break;
		case SDLK_w:
			(*p).direction = UP;
			(*p).setMoving(true);
			break;
		}
	}
	else if (mainEvent.type == SDL_KEYUP) {
		(*p).setMoving(false);
	}
	else if (mainEvent.type == SDL_QUIT) {
		if (this->world != NULL)this->world->setWon(true);
	}

	previous_event = mainEvent;
}

void input_Visitor::visit(Asteroid * a)
{
}

void input_Visitor::visit(Star * s)
{
}

void input_Visitor::visit(Weapon * w)
{
}

void input_Visitor::setWorld(World * w)
{
	this->world = w;
}
