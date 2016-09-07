#pragma once
#include <Image.h>

class WorldManager;
class LightBlockerContainer;
class TerrainManager;

class Block
{
public:
	Block(WorldManager* wm, TerrainManager* tm);

	void initLight(Block* n, Block* e, Block* s, Block* w);

	void draw(double x, double y, uint8_t textureID);

	void updateLBCPos(double x, double y);

	void destroyLBC();

	~Block();

	LightBlockerContainer* lbc;

private:
	uint8_t textureID;
	bool firstDraw;
	WorldManager* wm;
	TerrainManager* tm;
	AllegroExt::Graphics::Image* texture;
};

