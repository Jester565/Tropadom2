#include "TerrainManager.h"
#include "PerlinManager.h"
#include "BlockColumn.h"
#include "WorldManager.h"

TerrainManager::TerrainManager(WorldManager* wm)
	:begin(0), end(BLOCK_COLS_SIZE - 1), wm(wm), bX(0)
{
	perlinManager = new PerlinManager(int64_t(23424213424));
	initCols( );
}

BlockColumn * TerrainManager::initCol(int bX)
{
	return new BlockColumn(wm, perlinManager, bX, bY);
}

void TerrainManager::initCols()
{
	for (int i = 0; i < BLOCK_COLS_SIZE; i++)
	{
		blockCols.push_back(initCol(bX + i));
	}
}

void TerrainManager::draw()
{
	double x;
	if (wm->getPixelWorldX() < 0)
	{
		x = (int)(-wm->getPixelWorldX()) % Block::BLOCK_WIDTH - Block::BLOCK_WIDTH;
	}
	else
	{
		x = (int)(-(abs((int)wm->getPixelWorldX()) % Block::BLOCK_WIDTH));
	}
	int bIndex = begin;
	for (int i = 0; i < BLOCK_COLS_SIZE; i++)
	{
		blockCols.at(bIndex)->draw(x);
		x += Block::BLOCK_WIDTH;
		bIndex++;
		if (bIndex >= BLOCK_COLS_SIZE)
		{
			bIndex = 0;
		}
	}
}

void TerrainManager::translate(float dX, float dY)
{
	double changeBX = (wm->getPixelWorldX() + dX) / Block::BLOCK_WIDTH;
	double currentBX = (wm->getPixelWorldX()) / Block::BLOCK_WIDTH;
	if (changeBX < 0)
	{
		changeBX--;
	}
	if (currentBX < 0)
	{
		currentBX--;
	}
	if ((int)changeBX - (int)currentBX != 0)
	{
		shiftBX(((int)changeBX - (int)currentBX));
	}
	double changeBY = (wm->getPixelWorldY() + dY) / Block::BLOCK_WIDTH;
	double currentBY = (wm->getPixelWorldY()) / Block::BLOCK_WIDTH;
	if (changeBY < 0)
	{
		changeBY--;
	}
	if (currentBY < 0)
	{
		currentBY--;
	}
	if ((int)changeBY - (int)currentBY != 0)
	{
		shiftBY((int)changeBY - (int)currentBY);
	}
}

TerrainManager::~TerrainManager()
{
}

void TerrainManager::shiftBX(int count)
{
	for (int i = 0; i < abs(count); i++)
	{
		begin += count/abs(count);
		bX -= count / abs(count);
		if (begin >= BLOCK_COLS_SIZE)
		{
			begin = 0;
		}
		if (begin < 0)
		{
			begin = BLOCK_COLS_SIZE - 1;
		}
		if (begin == 0)
		{
			end = BLOCK_COLS_SIZE - 1;
		}
		else
		{
			end = begin - 1;
		}
		if (count < 0)
		{
			delete blockCols.at(begin);
			blockCols.at(begin) = initCol(bX);
		}
		else
		{
			delete blockCols.at(end);
			blockCols.at(end) = initCol(bX + BLOCK_COLS_SIZE - 1);
		}
	}
}

void TerrainManager::shiftBY(int count)
{
	bY -= count;
	for (int i = 0; i < BLOCK_COLS_SIZE; i++)
	{
		blockCols.at(i)->shiftBY(count);
	}
}

