#pragma once
#include "Block.h"
#include "TerrainConstants.h"
#include <DisplayManager.h>
#include <vector>

class WorldManager;
class PerlinManager;
class AboveLightBlocker;
class TerrainManager;

class BlockColumn
{
public:

	BlockColumn(WorldManager* wm, TerrainManager* tm, PerlinManager* pm, int bX, int bY);
	void draw(double x, uint8_t textureID);
	void updateLBC(double x);
	void drawLBDebug(double x, uint8_t textureID);
	void shiftBY(int count);
	void initLight();
	void destroyLight();
	Block* getBlock(int bY);
	~BlockColumn();

private:
	void initBlocks();
	Block* initBlock(int bY);
	std::vector<Block*> blocks;
	int bX;
	int bY;
	int begin;
	int end;
	int groundY;
	std::vector<std::pair<int, int>> airRanges;
	PerlinManager* perlinManager;
	AboveLightBlocker* alb;
	WorldManager* wm;
	TerrainManager* tm;
	bool initLightCalled;
};

