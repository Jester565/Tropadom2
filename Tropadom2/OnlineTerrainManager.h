/*
#pragma once
#include "Block.h"
#include <DisplayManager.h>
#include <Image.h>
#include <vector>

class OnlineBlockColumn;
class WorldManager;

class OnlineTerrainManager
{
public:
	static const int BLOCK_COLS_LIGHT_OFF = 1;
	static const int BLOCK_DRAW_OFF = 3;
	static const int BLOCK_DRAW_BW = AllegroExt::Graphics::WINDOW_X / Block::BLOCK_WIDTH + BLOCK_DRAW_OFF * 2;
	static const int BLOCK_LOAD_BW = 80;
	static const int BLOCK_LOAD_BH = 58;
	static const int BLOCK_COLS_OFF = (BLOCK_LOAD_BW - BLOCK_DRAW_BW) / 2;
	static const int INITIAL_BY = 200;
	OnlineTerrainManager(WorldManager* wm);

	void initBlockTextures();

	void draw();

	void translate(float dX, float dY);

	Block* getBlock(int bX, int bY);

	AllegroExt::Graphics::Image* getTexture(int id)
	{
		return blockTextures.at(id - 1);
	}

	~OnlineTerrainManager();

protected:
	std::vector<OnlineBlockColumn*> blockCols;
	std::vector<AllegroExt::Graphics::Image*> blockTextures;
	void shiftBX(int count);
	void shiftBY(int count);
	int begin;
	int end;
	int bX;
	int bY;
	WorldManager* wm;
};
*/