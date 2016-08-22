#pragma once
class WorldManager;
class LightBlockerContainer;

class Block
{
public:
	static const int BLOCK_WIDTH = 40;
	Block(WorldManager* wm);

	void draw(double x, double y);

	~Block();

private:
	bool firstDraw;
	WorldManager* wm;
	LightBlockerContainer* lbc;
};

