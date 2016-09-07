#pragma once
#include "TerrainConstants.h"
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
	
	TerrainManager(WorldManager* wm);

	void initBlockTextures();

	void draw();

	void drawLBDebug();

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

