#pragma once
#include "Block.h"
#include <DisplayManager.h>
#include <Image.h>
#include <vector>

class BlockColumn;
class WorldManager;
class PerlinManager;
class CaveManager;

class TerrainManager
{
public:
	static const int SEED = 2938457264624365;
	static const int BLOCK_COLS_LIGHT_OFF = 1;
	static const int BLOCK_COLS_OFF = 3;
	static const int BLOCK_COLS_SIZE = AllegroExt::Graphics::WINDOW_X/Block::BLOCK_WIDTH + BLOCK_COLS_OFF * 2;
	static const int INITIAL_BY = 200;
	TerrainManager(WorldManager* wm);

	void initBlockTextures();

	void draw();

	void translate(float dX, float dY);

	Block* getBlock(int bX, int bY);

	CaveManager* getCaveManager()
	{
		return caveManager;
	}

	AllegroExt::Graphics::Image* getTexture(int id)
	{
		return blockTextures.at(id - 1);
	}

	~TerrainManager();

protected:
	BlockColumn* initCol(int bX);
	void initCols();
	PerlinManager* groundPerlinManager;
	std::vector<BlockColumn*> blockCols;
	std::vector<AllegroExt::Graphics::Image*> blockTextures;
	void shiftBX(int count);
	void shiftBY(int count);
	int begin;
	int end;
	int bX;
	int bY;
	WorldManager* wm;
	CaveManager* caveManager;
};

