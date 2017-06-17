#pragma once

#include "Visitor.h"
#include <SDL.h>
#include <SDL_image.h>
#include <ctime>
#include <cmath>

#include "Asteroid.h"
#include "Star.h"
#include "Player.h"
#include "Weapon.h"

struct ObjectAnimationLog {
	int WorldObject_id;
	int WorldObject_animationStep;
	int animationStep_count; // amount of steps before resetting
	int last_step_time; // time when last step was taken
	double steps_per_sec; // how many steps should take per second
};

class render_Visitor: public Visitor {
public:
	render_Visitor(SDL_Renderer* r);
	render_Visitor();
	~render_Visitor();

	void visit(Asteroid * a);
	void visit(Star * a);
	void visit(Player * p);
	void visit(Weapon * w);
	void drawBounds();
	void drawYouWin();
	void drawYouLose();

	// sets
	void setRenderer(SDL_Renderer* r);
	void removeObjectAnimationLog(int objectID);
	void removeObjectDeathAnimationLog( int objectID );
	void setBounds(int w, int h);

	// gets
	int getAnimationStep(WorldObject* o);
	int getDeathAnimationStep(WorldObject* o);
	void getIsometricXY(int* original_x, int* original_y);
	void getCartesianXY(int* original_x, int* original_y);

	//loads
	void loadTextures();
	void addAnimationLog(WorldObject* o);

private:
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;
	int x_midpoint = SCREEN_WIDTH / 2;
	int y_midpoint = SCREEN_HEIGHT / 2;
	int bounds[2];

	// the player is always centred in the screen, so subtract the player x,y to get x,y offset.
	int x_offset;
	int y_offset;

	// rendering utilities
	SDL_Renderer* renderer;
	SDL_Rect drawing_rect;

	const int log_map_size = 5000;
	ObjectAnimationLog* animation_logs[5000];
	ObjectAnimationLog* death_animation_logs[5000];
	
	// Texture buffers --------------------------------------------------------
	// Each texture will contain 5 frames to be played sequencially.----------|
	// The current frame to be rendered is found using ObjectAnimation logs.--|
	SDL_Texture* player_textures[4];  // look-up, look-down, look-left, look-right
	SDL_Texture* asteroid_textures[2]; // hovering, exploding
	SDL_Texture* star_textures[4]; // just hovering, but four different types of star: blue, red, yellow, purple 
	SDL_Texture* weapon_textures[2]; // firing, not firing
	SDL_Texture* bullet_textures[4]; // four different types of bullet
	SDL_Texture* world_border_texture;

	// presentation textures
	SDL_Texture* you_lose;
	SDL_Texture* you_win;
};