#pragma once
class World;

enum LevelBreakType {
	COMPLETE,
	PAUSE,
	FAIL,
	SAVE_AND_QUIT,
	NONE
};

class Level {
public:
	// individual levels inherit
	virtual void visitSetup(World* w) = 0;
	virtual LevelBreakType startLevel() = 0;

	int getMaxX();
	int getMaxY();

	void setMaxX(int x);
	void setMaxY(int y);
protected:
	int level_dimensions[2]; // an x and y coordinate dictating the boundaries of the level
	World* world;
};