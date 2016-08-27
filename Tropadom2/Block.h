#pragma once
#include <Image.h>

class WorldManager;
class LightBlockerContainer;
class TerrainManager;

class Block
{
public:
	static const int BLOCK_WIDTH = 20;
	Block(WorldManager* wm, TerrainManager* tm);

	void initLight(Block* n, Block* e, Block* s, Block* w);

	void draw(double x, double y);

	~Block();

	LightBlockerContainer* lbc;

private:
	bool firstDraw;
	WorldManager* wm;
	TerrainManager* tm;
	AllegroExt::Graphics::Image* texture;
};

