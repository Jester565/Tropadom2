#pragma once
#include "Block.h"
#include <DisplayManager.h>
#include <vector>

class WorldManager;
class PerlinManager;
class AboveLightBlocker;

class BlockColumn
{
public:
	static const int BLOCKS_SIZE = AllegroExt::Graphics::WINDOW_Y/Block::BLOCK_WIDTH + 2;
	static const int MAX_BY = 500;
	BlockColumn(WorldManager* wm, PerlinManager* pm, int bX, int bY);
	void draw(double x);
	void shiftBY(int count);
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
};

