#include "TerrainManager.h"
#include "PerlinManager.h"
#include "BlockColumn.h"
#include "WorldManager.h"
#include "DebugBox.h"
#include <time.h>

TerrainManager::TerrainManager(WorldManager* wm)
	:begin(0), end(BLOCK_COLS_SIZE - 1), wm(wm), bX(0), bY(INITIAL_BY)
{
	srand(time(NULL));
	//perlinManager = new PerlinManager(int64_t(rand() % 10000000000));
	groundPerlinManager = new PerlinManager(2938457264624365);
	initBlockTextures();
	initCols();

	wm->debugBox->addField("BX", "?");
	wm->debugBox->addField("BY", "?");
}

void TerrainManager::initBlockTextures()
{
	AllegroExt::Graphics::Image::SetPrePath("C:\\Users\\ajcra\\Desktop\\wdir\\trop2\\");
	blockTextures.push_back(new AllegroExt::Graphics::Image("block.png", Block::BLOCK_WIDTH, Block::BLOCK_WIDTH));
}

BlockColumn * TerrainManager::initCol(int bX)
{
	return new BlockColumn(wm, this, perlinManager, bX, bY);
}

void TerrainManager::initCols()
{
	for (int i = 0; i < BLOCK_COLS_SIZE; i++)
	{
		BlockColumn* blockColumn = initCol(bX + i);
		blockCols.push_back(blockColumn);
		if (i > BLOCK_COLS_LIGHT_OFF && i <= BLOCK_COLS_SIZE - BLOCK_COLS_LIGHT_OFF)
		{
			blockCols.at(blockCols.size() - 2)->initLight();
		}
	}
}

void TerrainManager::draw()
{
	wm->debugBox->setField("BX", std::to_string(bX));
	wm->debugBox->setField("BY", std::to_string(bY));
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
	al_hold_bitmap_drawing(false);
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

Block * TerrainManager::getBlock(int searchBX, int searchBY)
{
	int colI = searchBX - bX;
	colI += begin;
	if (colI >= BLOCK_COLS_SIZE)
	{
		colI -= BLOCK_COLS_SIZE;
	}
	if (colI < BLOCK_COLS_SIZE && colI >= 0)
	{
		return blockCols.at(colI)->getBlock(searchBY);
	}
	return nullptr;
}

void TerrainManager::shiftBX(int count)
{
	for (int i = 0; i < abs(count); i++)
	{
		begin += count/abs(count);
		bX += count / abs(count);
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
			blockCols.at(end)->destroyLight();
			int lbInit = begin + 1;
			if (lbInit >= BLOCK_COLS_SIZE)
			{
				lbInit = 0;
			}
			blockCols.at(lbInit)->initLight();
		}
		else
		{
			delete blockCols.at(end);
			blockCols.at(end) = initCol(bX + BLOCK_COLS_SIZE - 1);
			blockCols.at(begin)->destroyLight();
			int lbInit = end - 1;
			if (lbInit < 0)
			{
				lbInit = BLOCK_COLS_SIZE - 1;
			}
			blockCols.at(lbInit)->initLight();
		}
	}
}

void TerrainManager::shiftBY(int count)
{
	bY += count;
	for (int i = 0; i < BLOCK_COLS_SIZE; i++)
	{
		blockCols.at(i)->shiftBY(count);
	}
}

