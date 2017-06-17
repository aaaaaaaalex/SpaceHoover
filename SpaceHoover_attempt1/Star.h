#pragma once
#include "WorldObject.h"
#include "Visitor.h"

enum StarType { RED, BLUE, YELLOW, PURPLE };

class Star : public WorldObject {
public:
	Star();
	Star(int x, int y, int width, int height, StarType type, int mass);

	/*virtual*/void accept(Visitor &v);
	StarType type;

};