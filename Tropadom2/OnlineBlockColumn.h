/*
#pragma once
#include "Block.h"
#include <DisplayManager.h>
#include <vector>

class WorldManager;
class PerlinManager;
class AboveLightBlocker;
class TerrainManager;

class BlockColumn
{
public:
	static const int BLOCK_LIGHT_OFF = 1;
	static const int BLOCK_DRAW_BH = AllegroExt::Graphics::WINDOW_Y / Block::BLOCK_WIDTH + BLOCKS_OFF * 2;
	static const int MAX_BY = 500;
	BlockColumn(WorldManager* wm, TerrainManager* tm, PerlinManager* pm, int bX, int bY);
	void draw(double x);
	void shiftBY(int count);
	void initLight();
	void destroyLight();
	Block* getBlock(int bY);
	~BlockColumn();

private:
	std::vector<Block*> blocks;
	int bX;
	int bY;
	int begin;
	int end;
	int groundY;
	AboveLightBlocker* alb;
	WorldManager* wm;
	TerrainManager* tm;
	bool initLightCalled;
};
*/