/*
#include "OnlineTerrainManager.h"
#include "WorldManager.h"

OnlineTerrainManager::OnlineTerrainManager(WorldManager* wm)
{
	initBlockTextures();
}

void OnlineTerrainManager::initBlockTextures()
{
	blockTextures.push_back(new AllegroExt::Graphics::Image("block.png", Block::BLOCK_WIDTH, Block::BLOCK_WIDTH));
}

void OnlineTerrainManager::draw()
{
	double x;
	if (wm->getPixelWorldX() < 0)
	{
		x = (int)(-wm->getPixelWorldX()) % Block::BLOCK_WIDTH - Block::BLOCK_WIDTH * (BLOCK_COLS_OFF + 1);
	}
	else
	{
		x = (int)(-(abs((int)wm->getPixelWorldX()) % Block::BLOCK_WIDTH)) - Block::BLOCK_WIDTH * BLOCK_COLS_OFF;
	}
	al_hold_bitmap_drawing(true);
	int bIndex = begin + BLOCK_COLS_OFF;
	if (bIndex >= BLOCK_LOAD_BW)
	{
		bIndex -= BLOCK_LOAD_BW;
	}
	for (int i = 0; i < BLOCK_DRAW_BW; i++)
	{
		blockCols.at(bIndex)->draw(x);
		x += Block::BLOCK_WIDTH;
		bIndex++;
		if (bIndex >= BLOCK_LOAD_BW)
		{
			bIndex = 0;
		}
	}
	al_hold_bitmap_drawing(false);
}

void OnlineTerrainManager::translate(float dX, float dY)
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

OnlineTerrainManager::~OnlineTerrainManager()
{
}

Block * OnlineTerrainManager::getBlock(int searchBX, int searchBY)
{
	int colI = searchBX - bX;
	colI += begin;
	if (colI >= BLOCK_LOAD_BW)
	{
		colI -= BLOCK_LOAD_BW;
	}
	if (colI < BLOCK_LOAD_BW && colI >= 0)
	{
		return blockCols.at(colI)->getBlock(searchBY);
	}
	return nullptr;
}

void OnlineTerrainManager::shiftBX(int count)
{
	for (int i = 0; i < abs(count); i++)
	{
		begin += count / abs(count);
		bX += count / abs(count);
		if (begin >= BLOCK_LOAD_BW)
		{
			begin = 0;
		}
		if (begin < 0)
		{
			begin = BLOCK_LOAD_BW - 1;
		}
		if (begin == 0)
		{
			end = BLOCK_LOAD_BW - 1;
		}
		else
		{
			end = begin - 1;
		}
		if (count < 0)
		{
			delete blockCols.at(begin);
			blockCols.at(begin) = initCol(bX);
			int lightDestroyI = end - BLOCK_COLS_OFF;
			if (lightDestroyI < 0)
			{
				lightDestroyI += BLOCK_LOAD_BW;
			}
			blockCols.at(lightDestroyI)->destroyLight();
			int lbInit = begin + BLOCK_COLS_OFF + 1;
			if (lbInit >= BLOCK_LOAD_BW)
			{
				lbInit -= BLOCK_LOAD_BW;
			}
			blockCols.at(lbInit)->initLight();
		}
		else
		{
			delete blockCols.at(end);
			blockCols.at(end) = initCol(bX + BLOCK_LOAD_BW - 1);
			int lightDestroyI = begin;
			lightDestroyI += BLOCK_COLS_OFF;
			if (lightDestroyI >= BLOCK_LOAD_BW)
			{
				lightDestroyI -= BLOCK_LOAD_BW;
			}
			blockCols.at(lightDestroyI)->destroyLight();
			int lbInit = end - BLOCK_COLS_OFF - 1;
			if (lbInit < 0)
			{
				lbInit += BLOCK_LOAD_BW;
			}
			blockCols.at(lbInit)->initLight();
		}
	}
}

void OnlineTerrainManager::shiftBY(int count)
{
	bY += count;
	for (int i = 0; i < BLOCK_LOAD_BW; i++)
	{
		blockCols.at(i)->shiftBY(count);
	}
}
*/