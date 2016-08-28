#include "BlockColumn.h"
#include "WorldManager.h"
#include "PerlinManager.h"
#include "TerrainManager.h"
#include "CaveManager.h"
#include <LightLayer.h>
#include <AboveLightBlocker.h>
#include <iostream>

BlockColumn::BlockColumn(WorldManager* wm, TerrainManager* tm, PerlinManager* perlinManager, int bX, int bY)
	:begin(0), end(BLOCKS_SIZE - 1), wm(wm), perlinManager(perlinManager), bX(bX), bY(bY), alb(nullptr), tm(tm), initLightCalled(false)
{
	groundY = perlinManager->getPerlinVal(bX, 100, 60);
	airRanges.emplace_back(-1, groundY);
	initBlocks();
}

void BlockColumn::initBlocks()
{
	for (int i = 0; i < BLOCKS_SIZE; i++)
	{
		blocks.push_back(initBlock(bY + i));
	}
}

Block* BlockColumn::initBlock(int bY)
{
	bool air = false;
	if (bY >= 0 && bY < MAX_BY)
	{
		for (int i = 0; i < airRanges.size(); i++)
		{
			if (bY >= airRanges.at(i).first && bY <= airRanges.at(i).second)
			{
				air = true;
			}
		}
	}
	if (!air)
	{
		if (!tm->getCaveManager()->isAir(bX, bY))
		{
			return new Block(wm, tm);
		}
	}
	return nullptr;
}

Block* BlockColumn::getBlock(int searchBY)
{
	int rowI = searchBY - bY;
	rowI += begin;
	if (rowI >= BLOCKS_SIZE)
	{
		rowI -= BLOCKS_SIZE;
	}
	if (rowI < BLOCKS_SIZE && rowI >= 0)
	{
		return blocks.at(rowI);
	}
	return nullptr;
}

void BlockColumn::draw(double x)
{
	double y;
	if (wm->getPixelWorldY() < 0)
	{
		y = (int)(-wm->getPixelWorldY()) % Block::BLOCK_WIDTH - Block::BLOCK_WIDTH * (BLOCKS_OFF + 1);
	}
	else
	{
		y = (int)(-(abs((int)wm->getPixelWorldY()) % Block::BLOCK_WIDTH)) - Block::BLOCK_WIDTH * (BLOCKS_OFF);
	}
	if (bY + 1 > groundY)
	{
		if (alb == nullptr)
		{
			alb = new AboveLightBlocker(x, 0, Block::BLOCK_WIDTH);
			wm->getLightLayer()->addAboveLightBlocker(alb);
		}
		alb->setGlobalX(x);
	}
	if (bY + 1 <= groundY)
	{
		if (alb != nullptr)
		{
			wm->getLightLayer()->removeAboveLightBlocker(alb);
			delete alb;
			alb = nullptr;
		}
	}
	int bIndex = begin;
	for (int i = 0; i < BLOCKS_SIZE; i++)
	{
		if (blocks.at(bIndex) != nullptr)
		{
			blocks.at(bIndex)->draw(x, y);
		}
		y += Block::BLOCK_WIDTH;
		bIndex++;
		if (bIndex >= BLOCKS_SIZE)
		{
			bIndex = 0;
		}
	}
}

void BlockColumn::shiftBY(int count)
{
	for (int i = 0; i < abs(count); i++)
	{
		begin+=count/abs(count);
		bY+=count/abs(count); 
		if (begin >= BLOCKS_SIZE)
		{
			begin = 0;
		}
		if (begin < 0)
		{
			begin = BLOCKS_SIZE - 1;
		}
		if (begin == 0)
		{
			end = BLOCKS_SIZE - 1;
		}
		else
		{
			end = begin - 1;
		}
		if (count < 0)
		{
			if (blocks.at(begin) != nullptr)
			{
				delete blocks.at(begin);
			}
			blocks.at(begin) = initBlock(bY);
			if (initLightCalled)
			{
				int lbInit = begin + 1;
				if (lbInit >= BLOCKS_SIZE)
				{
					lbInit = 0;
				}
				if (blocks.at(lbInit) != nullptr && blocks.at(lbInit)->lbc == nullptr)
				{
					int lbSouth = lbInit + 1;
					if (lbSouth >= BLOCKS_SIZE)
					{
						lbSouth = 0;
					}
					blocks.at(lbInit)->initLight(blocks.at(begin), tm->getBlock(bX + 1, bY + 1), blocks.at(lbSouth), tm->getBlock(bX - 1, bY + 1));
				}
			}
		}
		else
		{
			if (blocks.at(end) != nullptr)
			{
				delete blocks.at(end);
			}
			blocks.at(end) = initBlock(bY + BLOCKS_SIZE - 1);
			if (initLightCalled)
			{
				int lbInit = end - 1;
				if (lbInit < 0)
				{
					lbInit = BLOCKS_SIZE - 1;
				}
				if (blocks.at(lbInit) != nullptr && blocks.at(lbInit)->lbc == nullptr)
				{
					int lbNorth = lbInit - 1;
					if (lbNorth < 0)
					{
						lbNorth = BLOCKS_SIZE - 1;
					}
					blocks.at(lbInit)->initLight(blocks.at(lbNorth), tm->getBlock(bX + 1, bY + BLOCKS_SIZE - 2), blocks.at(end), tm->getBlock(bX - 1, bY + BLOCKS_SIZE - 2));
				}
			} 
		}
	}
}

void BlockColumn::initLight()
{
	if (!initLightCalled)
	{
		int bIndex = begin;
		for (int i = 0; i < BLOCKS_SIZE; i++)
		{
			if (i >= BLOCK_LIGHT_OFF && i < BLOCKS_SIZE - BLOCK_LIGHT_OFF && blocks.at(bIndex) != nullptr)
			{
				int northBlock = bIndex - 1;
				if (northBlock < 0)
				{
					northBlock = BLOCKS_SIZE - 1;
				}
				int southBlock = bIndex + 1;
				if (southBlock >= BLOCKS_SIZE)
				{
					southBlock = 0;
				}
				blocks.at(bIndex)->initLight(blocks.at(northBlock), tm->getBlock(bX + 1, bY + i), blocks.at(southBlock), tm->getBlock(bX - 1, bY + i));
			}
			bIndex++;
			if (bIndex >= BLOCKS_SIZE)
			{
				bIndex = 0;
			}
		}
		initLightCalled = true;
	}
}

void BlockColumn::destroyLight()
{
	/*
	if (initLightCalled)
	{
		for (int i = 0; i < BLOCKS_SIZE; i++)
		{
			if (blocks.at(i) != nullptr && blocks.at(i)->lbc != nullptr)
			{
				blocks.at(i)->destroyLBC();
			}
		}
		initLightCalled = false;
	}
	*/
}

BlockColumn::~BlockColumn()
{
	for (int i = 0; i < blocks.size(); i++)
	{
		if (blocks.at(i) != nullptr)
		{
			delete blocks.at(i);
		}
	}
	if (alb != nullptr)
	{
		wm->getLightLayer()->removeAboveLightBlocker(alb);
		delete alb;
	}
}
