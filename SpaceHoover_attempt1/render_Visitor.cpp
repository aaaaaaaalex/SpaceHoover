
#include "render_Visitor.h"

render_Visitor::render_Visitor(SDL_Renderer * r)
{
	this->renderer = r;
	this->loadTextures();
}
render_Visitor::render_Visitor()
{
}

render_Visitor::~render_Visitor()
{
	for (int i = 0; i < this->log_map_size; i++) {
		ObjectAnimationLog* log_pointer = this->animation_logs[i];
		if(log_pointer != NULL)delete log_pointer;
		this->animation_logs[i] = NULL;
}}

void render_Visitor::visit(Asteroid *a) {
	int x = (*a).getX();
	int y = (*a).getY();
	int w = (*a).getW();
	int h = (*a).getH();
	getIsometricXY(&x, &y);
	x += x_offset;
	y += y_offset;

	drawing_rect.x = x;
	drawing_rect.y = y;
	drawing_rect.w = w;
	drawing_rect.h = h;

	int animation_step = 0;
	if (a->isDestroyed()) {
		animation_step = getDeathAnimationStep(a); 
		removeObjectAnimationLog(a->getObjectID());
	}
	else { animation_step = getAnimationStep(a); }
	SDL_Rect animation_frame;

	int frame_width;
	SDL_QueryTexture(asteroid_textures[0], NULL, NULL, &frame_width, NULL);
	animation_frame.x = (frame_width / 5) * animation_step;
	animation_frame.y = 0;
	animation_frame.w = 100;
	animation_frame.h = 150;

	if (!((x > SCREEN_WIDTH || y > SCREEN_HEIGHT) || (x < (0 - w) || y < (0 - h)))) {
		if ((*a).isDestroyed()) {
			SDL_RenderCopy(renderer, asteroid_textures[1], &animation_frame, &drawing_rect);
		}
		else {
			SDL_RenderCopy(renderer, asteroid_textures[0], &animation_frame, &drawing_rect);
		}
	}
	
/*	SDL_SetRenderDrawColor(this->renderer, 0xcc, 0x00, 0xff, 0xff);
	SDL_RenderDrawRect(this->renderer, &drawing_rect);
	SDL_SetRenderDrawColor(this->renderer, 0x00, 0x00, 0x00, 0x00); */
}

void render_Visitor::visit(Star *a) {
	int x = (*a).getX();
	int y = (*a).getY();
	int w = (*a).getW();
	int h = (*a).getH();

	getIsometricXY(&x, &y);
	x += x_offset;
	y += y_offset;

	drawing_rect.x = x;
	drawing_rect.y = y;
	drawing_rect.w = w;
	drawing_rect.h = h;

	int animation_step = 0;
	animation_step = getAnimationStep(a);

	SDL_Rect animation_frame;
	animation_frame.x = 0;
	animation_frame.y = 0;
	animation_frame.w = 500;
	animation_frame.h = 500;

	if (!(*a).isDestroyed()) {
		int texture_selector;
		switch ((*a).type) {
		case BLUE:
			texture_selector = 0;
			break;

		case RED:
			texture_selector = 1;
			break;

		case YELLOW:
			texture_selector = 2;
			break;

		case PURPLE:
			texture_selector = 3;
			break;
		}
		if (!((x > SCREEN_WIDTH || y > SCREEN_HEIGHT) || (x < (0 - w) || y < (0 - h)))) {
			SDL_RenderCopy(renderer, star_textures[texture_selector], &animation_frame, &drawing_rect);
		}
	}
	else {
		removeObjectAnimationLog((*a).getObjectID());
	}
}

void render_Visitor::visit(Player *a) {
	int x = x_midpoint;
	int y = y_midpoint;
	int w = (*a).getW();
	int h = (*a).getH();

	int iso_x = a->getX(),
		iso_y = a->getY();

	getIsometricXY(&iso_x, &iso_y);

	this->x_offset = x_midpoint - iso_x;
	this->y_offset = y_midpoint - iso_y;

	drawing_rect.x = x;
	drawing_rect.y = y;
	drawing_rect.w = w;
	drawing_rect.h = h;

	int animation_step = 0;
	animation_step = getAnimationStep(a);
	SDL_Rect animation_frame;
	animation_frame.x = (animation_step * w);
	animation_frame.y = 0;
	animation_frame.w = w;
	animation_frame.h = h;

	if (!(*a).isDestroyed()) {
		int texture_selector = 0;

		switch ((*a).direction) {
		case UP:
			texture_selector = 0;
			break;
		case DOWN:
			texture_selector = 1;
			break;
		case LEFT:
			texture_selector = 2;
			break;
		case RIGHT:
			texture_selector = 3;
			break;
		}
		SDL_RenderCopy(renderer, player_textures[texture_selector], &animation_frame, &drawing_rect);
	}
	else {
		removeObjectAnimationLog((*a).getObjectID());
	}

	// render health container/bar.
	int bar_width = 120;
	SDL_Rect health_container = { x - 10, y - 10, bar_width, 20 };
	SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderDrawRect(this->renderer, &health_container);

	double health = a->getHealth();
	if (health > 0) {
		double max_health = a->getMaxHealth();
		double amount_left = health / max_health;
		if (amount_left > 0.5) SDL_SetRenderDrawColor(this->renderer, 0x00, 0xFF, 0x00, 0xAA);
		else if (amount_left > 0.25) SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0x00, 0xAA);
		else SDL_SetRenderDrawColor(this->renderer, 0xFF, 0x00, 0x00, 0xAA);

		int health_width = bar_width * amount_left;
		SDL_Rect health_left = { x - 10, y - 10, health_width, 20 };
		SDL_RenderFillRect(this->renderer, &health_left);
	}
	SDL_SetRenderDrawColor(this->renderer, 0x00, 0x00, 0x00, 0x00);


	/*
	// draw bounding box (debug)
	int test_x = a->getX();
	int test_y = a->getY();
	getTrueXY(&test_x, &test_y);
	SDL_Rect bounding_box = { test_x, test_y, a->getW(), a->getH() };
	SDL_SetRenderDrawColor(this->renderer, 0xcc, 0x00, 0xff, 0xff);
	SDL_RenderDrawRect(this->renderer, &bounding_box);
	SDL_SetRenderDrawColor(this->renderer, 0x00, 0x00, 0x00, 0x00);*/
}
void render_Visitor::visit(Weapon *a) {
	int x = (*a).getX();
	int y = (*a).getY();
	int w = (*a).getW();
	int h = (*a).getH();
	x += x_offset;
	y += y_offset;

	drawing_rect.x = x;
	drawing_rect.y = y;
	drawing_rect.w = w;
	drawing_rect.h = h;

	int animation_step = 0;
	animation_step = getAnimationStep(a);
	SDL_Rect animation_frame;
	animation_frame.x = (animation_step * w);
	animation_frame.y = 0;
	animation_frame.w = w;
	animation_frame.h = h;

	if (!(*a).isDestroyed()) {
		int texture_selector = 0;

		if ( !((x > SCREEN_WIDTH || y > SCREEN_HEIGHT) || (x < (0-w) || y < (0-h))) ) {
			SDL_RenderCopy(renderer, player_textures[texture_selector], &animation_frame, &drawing_rect);
		}
	}
	else {
		removeObjectAnimationLog((*a).getObjectID());
	}
}

void render_Visitor::drawBounds()
{
	int cart_x = 0, cart_y = 0,
		cart_w = bounds[0], cart_h = bounds[1];
	// temp values for finding true width of the map
	int zero1 = 0, zero2 = 0;
	getIsometricXY(&zero1, &cart_h); //get cartesian value of left-est point
	getIsometricXY(&cart_w, &zero2); // get cartesian value of right-est point
	cart_w = cart_w - zero1;
	cart_x += x_offset - (cart_w/2);
	cart_y += y_offset;

	SDL_Rect image_placement = { cart_x, cart_y, cart_w, cart_w/2 };
	SDL_RenderCopy(this->renderer, world_border_texture, NULL, &image_placement);
}

void render_Visitor::drawYouWin()
{
	SDL_RenderCopy(this->renderer, this->you_win, NULL, NULL);
}
void render_Visitor::drawYouLose()
{
	SDL_RenderCopy(this->renderer, this->you_lose, NULL, NULL);
}

void render_Visitor::setRenderer(SDL_Renderer * r)
{
	this->renderer = r;
	this->loadTextures();
}

void render_Visitor::removeObjectAnimationLog(int objectID)
{
	//search for the log with the matching objectID
	ObjectAnimationLog* l = this->animation_logs[objectID];
	if (l != NULL) delete l;
	this->animation_logs[objectID] = NULL;
}
void render_Visitor::removeObjectDeathAnimationLog(int objectID) {
	ObjectAnimationLog* l = this->death_animation_logs[objectID];
	if (l != NULL) delete l;
	this->death_animation_logs[objectID] = NULL;
}

void render_Visitor::setBounds(int w, int h)
{
	this->bounds[0] = w;
	this->bounds[1] = h;
}

int render_Visitor::getAnimationStep(WorldObject* o)
{
	// search for the matching animation log
	ObjectAnimationLog* log = NULL;
	int objectID = (*o).getObjectID();
	if (this->animation_logs[objectID] != NULL) log = animation_logs[objectID];

	// if no log was found, make one
	// logs are stored in the heap, with a pointer stored in the log-vector
	if (log == NULL) {
		int max_step_count = (*o).getAnimationStepCount();
		double step_rate = (*o).getAnimationStepRate();

		ObjectAnimationLog* new_log = new ObjectAnimationLog();
		(*new_log).WorldObject_id = objectID;
		(*new_log).WorldObject_animationStep = 0;
		(*new_log).animationStep_count = max_step_count;
		(*new_log).steps_per_sec = step_rate;

		this->animation_logs[objectID] = new_log;
		log = new_log;
	} 
	if ((*log).WorldObject_animationStep > ((*log).animationStep_count-1)) (*log).WorldObject_animationStep = 0;

	int time_now = clock();
	int last_step = (*log).last_step_time;
	int time_slice = 1000 / (*log).steps_per_sec;  // a thousand miliseconds, shared between steps_per_sec
	if ((time_now - last_step) > time_slice) {
		(*log).last_step_time = clock();

		// return the animationStep and increment it for next time
		int step = (*log).WorldObject_animationStep;
		(*log).WorldObject_animationStep++;
		return step;
	}
	else {
		return (*log).WorldObject_animationStep;
	}
}

int render_Visitor::getDeathAnimationStep(WorldObject * o)
{
	// search for the matching animation log
	ObjectAnimationLog* log = NULL;
	int objectID = (*o).getObjectID();
	if (this->death_animation_logs[objectID] != NULL) log = death_animation_logs[objectID];

	// if no log was found, make one
	// logs are stored in the heap, with a pointer stored in the log-vector
	if (log == NULL) {
		int max_step_count = (*o).getAnimationStepCount();
		double step_rate = (*o).getAnimationStepRate();

		ObjectAnimationLog* new_log = new ObjectAnimationLog();
		(*new_log).WorldObject_id = objectID;
		(*new_log).WorldObject_animationStep = 0;
		(*new_log).animationStep_count = max_step_count;
		(*new_log).steps_per_sec = step_rate;

		this->death_animation_logs[objectID] = new_log;
		log = new_log;
	}
	if ((*log).WorldObject_animationStep > ((*log).animationStep_count - 1)) {
		removeObjectDeathAnimationLog(o->getObjectID());
		o->setRemovedFromPlay(true);
		return o->getAnimationStepCount() - 1;
	}
	int time_now = clock();
	int last_step = (*log).last_step_time;
	int time_slice = 1000 / (*log).steps_per_sec;  // a thousand miliseconds, shared between steps_per_sec
	if ((time_now - last_step) > time_slice) {
		(*log).last_step_time = clock();

		// return the animationStep and increment it for next time
		int step = (*log).WorldObject_animationStep;
		(*log).WorldObject_animationStep++;
		return step;
	}
	else {
		return (*log).WorldObject_animationStep;
	}
}



void render_Visitor::getIsometricXY(int * original_x, int * original_y)
{
	// dereferenced values - do it once and no more
	int x_val = (*original_x);
	int y_val = (*original_y);
	int iso_x = x_val-y_val,
		iso_y = (x_val + y_val)/2;

	*original_x = iso_x;
	*original_y = iso_y;
}

void render_Visitor::getCartesianXY(int *original_x, int *original_y) {
	int temp_x, temp_y;
	temp_x = ((2 * *original_y) + *original_x / 2);
	temp_y = ((2 * *original_y) - *original_x / 2);
	*original_x = temp_x;
	*original_y = temp_y;
}

void render_Visitor::addAnimationLog(WorldObject* o) {
	ObjectAnimationLog* new_log = new ObjectAnimationLog();

	(*new_log).WorldObject_animationStep = 0;
	(*new_log).WorldObject_id = (*o).getObjectID();
	(*new_log).animationStep_count = (*o).getAnimationStepCount();
	(*new_log).steps_per_sec = (*o).getAnimationStepRate();
	(*new_log).last_step_time = clock();

	this->animation_logs[(*o).getObjectID()] = new_log;
}

void render_Visitor::loadTextures()
{
	// PLAYER
	SDL_Texture* player_img_up = IMG_LoadTexture(renderer, "./assets/texture/Player/standingUP.bmp");
	SDL_Texture* player_img_down = IMG_LoadTexture(renderer, "./assets/texture/Player/standingDOWN.bmp");
	SDL_Texture* player_img_left = IMG_LoadTexture(renderer, "./assets/texture/Player/standingLEFT.bmp");
	SDL_Texture* player_img_right = IMG_LoadTexture(renderer, "./assets/texture/Player/standingRIGHT.bmp");

	player_textures[0] = player_img_up;
	player_textures[1] = player_img_down;
	player_textures[2] = player_img_left;
	player_textures[3] = player_img_right;


	// STARS
	SDL_Texture* blue_star = IMG_LoadTexture(renderer, "./assets/texture/Star/blue.bmp");
	SDL_Texture* red_star = IMG_LoadTexture(renderer, "./assets/texture/Star/red.bmp");
	SDL_Texture* yellow_star = IMG_LoadTexture(renderer, "./assets/texture/Star/yellow.bmp");
	SDL_Texture* purple_star = IMG_LoadTexture(renderer, "./assets/texture/Star/purple.bmp");

	star_textures[0] = blue_star;
	star_textures[1] = red_star;
	star_textures[2] = yellow_star;
	star_textures[3] = purple_star;
	
	//WEAPONS
	
	
	// ASTEROIDS
	SDL_Texture* asteroid_hovering = IMG_LoadTexture(renderer, "./assets/texture/Asteroid/hovering.bmp");
	SDL_Texture* asteroid_exploding = IMG_LoadTexture(renderer, "./assets/texture/Asteroid/exploding.bmp");
	asteroid_textures[0] = asteroid_hovering;
	asteroid_textures[1] = asteroid_exploding;


	// WORLD
	world_border_texture = IMG_LoadTexture(renderer, "./assets/texture/World/border.png");

	// presentation
	this->you_lose = IMG_LoadTexture(renderer, "./assets/texture/you_lose.png");
	this->you_win = IMG_LoadTexture(renderer, "./assets/texture/you_win.png");
}
