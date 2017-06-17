#include "physics_Visitor.h"
#include <cmath>

//sets
void physics_Visitor::setTarget(int xy[2]) {
	this->target[0] = xy[0];
	this->target[1] = xy[1];
}

void physics_Visitor::setBounds(int maxX, int maxY)
{
	this->maxX = maxX;
	this->maxY = maxY;
}

void physics_Visitor::setGravitationalConstant(double g)
{
	this->gravitational_constant = g;
}

CollisionLog * physics_Visitor::getCollision(WorldObject * o)
{
	int object_x = o->getX();
	int object_y = o->getY();
	int object_w = o->getW();
	int object_h = o->getH();

	// search for a collision. multiple collisions at once are not possible, yet.
	bool found = false;
	CollisionLog* log = NULL;
	for (int i = 0; (i < logs_size && !found); i++) {
		int xPos = collision_logs[i]->x;
		int w = collision_logs[i]->w;
		if ((xPos < (object_x + object_w)) && ((object_x) < xPos + w)) {
			int yPos = collision_logs[i]->y;
			int h = collision_logs[i]->h;
			if (yPos < (object_y + object_h) && ((object_y) < yPos + h )) {
				log = collision_logs[i];
				found = true;
	}}}

	// add a log for the object currently querying
	CollisionLog* new_log = new CollisionLog;
	new_log->x = object_x;
	new_log->y = object_y;
	new_log->w = object_w;
	new_log->h = object_h;
	new_log->obj = o;
	addCollisionLog(new_log);

	return log;
}

void physics_Visitor::wipeCollisionLogs()
{
	int max = logs_size;
	for (int i = 0; i < max; i++) {
		delete collision_logs[i];
		collision_logs[i] = NULL;
		logs_size--;
	}
	stars_all_gone = true;
}

void physics_Visitor::addCollisionLog(CollisionLog * log)
{
	bool found = false;
	for (int i = 0; i < 10000 && !found; i++) {
		if (collision_logs[i] == NULL) {
			found = true;
			collision_logs[i] = log;
			logs_size++;
}}}

void physics_Visitor::setSFXModule(SFX_Module * m)
{
	this->sfx = m;
}

bool physics_Visitor::getStarsGone()
{
	return this->stars_all_gone;
}

// ---------------------------------------------VISITS
void physics_Visitor::visit(Asteroid* a) {
	if (a->isDestroyed()) return;

	int x_coord = a->getX(),
		y_coord = a->getY();

	// proceed in current direction
	switch (a->direction) {
	case UP:
		a->setY( a->getY() + a->getSpeed() );
		break;
	case DOWN:
		a->setY(a->getY() - a->getSpeed());
		break;
	case LEFT:
		a->setX(a->getX() - a->getSpeed());
		break;
	case RIGHT:
		a->setX(a->getX() + a->getSpeed());
		break;
	}

	// boundary management
	if (x_coord > this->maxX) a->setX(0);
	if (x_coord < 0) a->setX(this->maxX);
	if (y_coord > this->maxY) a->setY(0);
	if (y_coord < 0) a->setY(this->maxY);

	//check for collisions
	CollisionLog* log = getCollision(a);
	if (log != NULL) {
		sfx->addToQueue(COLLISION, 100);
		WorldObject* obj = log->obj;
		if (obj->getMass() > a->getMass()) {
			a->setDestroyed(true);
			obj->setSpeed( obj->getSpeed() - (a->getSpeed() / 2) );
		}
		else {
			obj->setDestroyed(true);
		}
	}
}

void physics_Visitor::visit(Star* a) {
	// newtonian gravitation, but every time they gravitate, only the star's mass is considered.
	// star drifts towards 'target' when close enough
	double s_mass = a->getMass();
	double star_x = a->getX(),
		star_y = a->getY();

	double abs_dist = sqrt(pow(star_x - target[0], 2) + pow(star_y - target[1], 2));
	if (abs_dist < 10) {
		if (!a->isDestroyed())	sfx->addToQueue(TWINKLE, 100);
		a->setDestroyed(true);
	}
	else {
		if (abs_dist < effective_distance) {
			// calculate the net force acting on the star
			double g_force = (s_mass * gravitational_constant) / (abs_dist * abs_dist);
			// resolve x and y vectors by getting the angle of attack (arctan of slope),
			// then aquiring proportion of force in both vectors (sin of angle).
			double slope = (star_y - target[1]) / (star_x - target[0]);
			double y_vector, x_vector, angle;
			if (slope > 1000000) {
				y_vector = g_force;
				x_vector = 0;
			}
			else if (slope < 0.00001) {
				y_vector = 0;
				x_vector = g_force;
			}
			else {
				angle = atan(slope);
				y_vector = g_force * sin(angle);
				x_vector = g_force - y_vector;
			}
			// resolve the direction of x and y vectors by determining the quadrant of the star,
			//   relative to the target.
			int quadrant;
			if (star_y >= target[1]) {
				if (star_x >= target[0]) quadrant = 3;
				else quadrant = 2;
			}
			else {
				if (star_x >= target[0]) quadrant = 0;
				else quadrant = 1;
			}
			// apply vectors
			switch (quadrant) {
			case 0:
				a->setX(star_x - x_vector);
				a->setY(star_y + y_vector);
				break;
			case 1:
				a->setX(star_x + x_vector);
				a->setY(star_y + y_vector);
				break;
			case 2:
				a->setX(star_x + x_vector);
				a->setY(star_y - y_vector);
				break;
			case 3:
				a->setX(star_x - x_vector);
				a->setY(star_y - y_vector);
				break;
			}
		}
	}

	if (!a->isDestroyed()) stars_all_gone = false;
}

void physics_Visitor::visit(Weapon* a) {
}

void physics_Visitor::visit(Player* a) {
	int maxSpeed = 4;
	double xSpeed = a->getXSpeed();
	double ySpeed = a->getYSpeed();
	bool moving = a->isMoving();

	//determine which direction to move in, and at what speed
	switch (a->direction) {
	case UP:
		a->setXSpeed(0);
		a->setY( a->getY() + (ySpeed) );
		if (moving) { if (ySpeed < maxSpeed) { a->setYSpeed(ySpeed - 0.01); } }
		else {
			if (ySpeed > 0.1) a->setYSpeed(ySpeed - 0.01);
			else if (ySpeed < -0.1) a->setYSpeed(ySpeed + 0.01);
			else a->setYSpeed(0);
		}
		break;
	case DOWN:
		a->setXSpeed(0);
		a->setY(a->getY() + (ySpeed));
		if (moving) { if (ySpeed < maxSpeed) { a->setYSpeed(ySpeed + 0.01); } }
		else {
			if (ySpeed > 0.1) a->setYSpeed(ySpeed - 0.01);
			else if (ySpeed < -0.1) a->setYSpeed(ySpeed + 0.01);
			else a->setYSpeed(0);
		}
		break;
	case LEFT:
		a->setYSpeed(0);
		a->setX(a->getX() + (xSpeed));
		if (moving) { if (xSpeed < maxSpeed) { a->setXSpeed(xSpeed - 0.01); } }
		else {
			if(xSpeed > 0.1) a->setXSpeed(xSpeed - 0.01);
			else if(xSpeed < -0.1) a->setXSpeed(xSpeed + 0.01);
			else a->setXSpeed(0);
		}
		break;
	case RIGHT:
		a->setYSpeed(0);
		a->setX(a->getX() + (xSpeed));
		if (moving) { if (xSpeed < maxSpeed) {a->setXSpeed(xSpeed + 0.01); }}
		else {
			if (xSpeed > 0.1) a->setXSpeed(xSpeed - 0.01);
			else if (xSpeed < -0.1) a->setXSpeed(xSpeed + 0.01);
			else a->setXSpeed(0);
		}
		break;
	}


	//check for collisions
	CollisionLog* log = getCollision(a);
	if (log != NULL) {
		WorldObject* obj = log->obj;
		obj->setDestroyed(true);
		int obj_mass = obj->getMass();
		a->setHealth( a->getHealth() - (obj_mass/2) );
		sfx->addToQueue(COLLISION, 100);
}}

physics_Visitor::physics_Visitor()
{
	this->gravitational_constant = 0.5;
	this->effective_distance = 100;
	this->stars_all_gone = false;
}

physics_Visitor::~physics_Visitor()
{
	wipeCollisionLogs();
}