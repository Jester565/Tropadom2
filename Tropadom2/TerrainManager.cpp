#include "TerrainManager.h"
#include "PerlinManager.h"
#include "CaveManager.h"
#include "BlockColumn.h"
#include "WorldManager.h"
#include "DebugBox.h"
#include <time.h>

TerrainManager::TerrainManager(WorldManager* wm)
	:begin(0), end(BLOCK_LOAD_BW - 1), wm(wm), bX(0), bY(INITIAL_BY)
{
	caveManager = new CaveManager(SEED, 60, 300);
	srand(time(NULL));
	groundPerlinManager = new PerlinManager(SEED);
	initBlockTextures();
	initCols();
	wm->debugBox->addField("BX", "?");
	wm->debugBox->addField("BY", "?");
}

void TerrainManager::initBlockTextures()
{
	blockTextures.push_back(new AllegroExt::Graphics::Image("block.png", BLOCK_WIDTH, BLOCK_WIDTH));
	blockTextures.push_back(new AllegroExt::Graphics::Image("block2.png", BLOCK_WIDTH, BLOCK_WIDTH));
}

BlockColumn * TerrainManager::initCol(int bX)
{
	return new BlockColumn(wm, this, groundPerlinManager, bX, bY);
}

void TerrainManager::initCols()
{
	for (int i = 0; i < BLOCK_LOAD_BW; i++)
	{
		BlockColumn* blockColumn = initCol(bX + i);
		blockCols.push_back(blockColumn);
		if (i >= BLOCK_LIGHT_OFF_BW + 1 && i <= BLOCK_LOAD_BW - BLOCK_LIGHT_OFF_BW)
		{
			blockCols.at(blockCols.size() - 2)->initLight();
		}
	}
}

void TerrainManager::draw()
{
	for (int j = 1; j <= blockTextures.size(); j++)
	{
		double x;
		if (wm->getPixelWorldX() < 0)
		{
			x = (int)(-wm->getPixelWorldX()) % BLOCK_WIDTH - BLOCK_WIDTH * (BLOCK_DRAW_OFF + BLOCK_LIGHT_OFF + 1);
		}
		else
		{
			x = (int)(-(abs((int)wm->getPixelWorldX()) % BLOCK_WIDTH)) - BLOCK_WIDTH * (BLOCK_DRAW_OFF + BLOCK_LIGHT_OFF);
		}
		al_hold_bitmap_drawing(true);
		int bIndex = (begin + BLOCK_LIGHT_OFF_BW) % BLOCK_LOAD_BW;
		for (int i = 0; i < BLOCK_DRAW_BW + BLOCK_LIGHT_OFF * 2; i++)
		{
			if (i > BLOCK_LIGHT_OFF && i <= BLOCK_DRAW_BW + BLOCK_LIGHT_OFF)
			{
				blockCols.at(bIndex)->draw(x, j);
			}
			else if (j == 1)
			{
				blockCols.at(bIndex)->updateLBC(x);
			}
			x += BLOCK_WIDTH;
			bIndex++;
			if (bIndex >= BLOCK_LOAD_BW)
			{
				bIndex = 0;
			}
		}
		al_hold_bitmap_drawing(false);
	}
}

void TerrainManager::drawLBDebug()
{
	for (int j = 1; j <= blockTextures.size(); j++)
	{
		double x;
		if (wm->getPixelWorldX() < 0)
		{
			x = (int)(-wm->getPixelWorldX()) % BLOCK_WIDTH - BLOCK_WIDTH * (BLOCK_OFF_BW + BLOCK_DRAW_OFF + 1);
		}
		else
		{
			x = (int)(-(abs((int)wm->getPixelWorldX()) % BLOCK_WIDTH)) - BLOCK_WIDTH * (BLOCK_OFF_BW + BLOCK_DRAW_OFF);
		}
		al_hold_bitmap_drawing(true);
		int bIndex = begin;
		for (int i = 0; i < BLOCK_LOAD_BW; i++)
		{
			blockCols.at(bIndex)->drawLBDebug(x, j);
			x += BLOCK_WIDTH;
			bIndex++;
			if (bIndex >= BLOCK_LOAD_BW)
			{
				bIndex = 0;
			}
		}
		al_hold_bitmap_drawing(false);
	}
}

void TerrainManager::translate(float dX, float dY)
{
	double changeBX = (wm->getPixelWorldX() + dX) / BLOCK_WIDTH;
	double currentBX = (wm->getPixelWorldX()) / BLOCK_WIDTH;
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
	double changeBY = (wm->getPixelWorldY() + dY) / BLOCK_WIDTH;
	double currentBY = (wm->getPixelWorldY()) / BLOCK_WIDTH;
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

Block * TerrainManager::getBlock(int searchBX, int searchBY)
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

void TerrainManager::shiftBX(int count)
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
			int lightDestroyI = end - BLOCK_LIGHT_OFF_BW + 1;
			if (lightDestroyI < 0)
			{
				lightDestroyI += BLOCK_LOAD_BW;
			}
			blockCols.at(lightDestroyI)->destroyLight();
			int lbInit = begin + BLOCK_LIGHT_OFF_BW;
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
			int lightDestroyI = begin + BLOCK_LIGHT_OFF_BW - 1;
			if (lightDestroyI >= BLOCK_LOAD_BW)
			{
				lightDestroyI -= BLOCK_LOAD_BW;
			}
			blockCols.at(lightDestroyI)->destroyLight();
			int lbInit = end - BLOCK_LIGHT_OFF_BW;
			if (lbInit < 0)
			{
				lbInit += BLOCK_LOAD_BW;
			}
			blockCols.at(lbInit)->initLight();
		}
	}
}

void TerrainManager::shiftBY(int count)
{
	bY += count;
	for (int i = 0; i < BLOCK_LOAD_BW; i++)
	{
		blockCols.at(i)->shiftBY(count);
	}
}

