#pragma once
#include "Block.h"
#include <DisplayManager.h>
#include <vector>

class BlockColumn;
class WorldManager;
class PerlinManager;

class TerrainManager
{
public:
	static const int BLOCK_COLS_SIZE = AllegroExt::Graphics::WINDOW_X/Block::BLOCK_WIDTH + 2;
	static const int INITIAL_BY = 300;
	TerrainManager(WorldManager* wm);

	void draw();

	void translate(float dX, float dY);

	~TerrainManager();

protected:
	BlockColumn* initCol(int bX);
	void initCols();
	PerlinManager* perlinManager;
	std::vector<BlockColumn*> blockCols;
	void shiftBX(int count);
	void shiftBY(int count);
	int begin;
	int end;
	int bX;
	int bY;
	WorldManager* wm;
};

