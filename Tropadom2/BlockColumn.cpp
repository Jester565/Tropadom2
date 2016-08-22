#include "BlockColumn.h"
#include "WorldManager.h"
#include "PerlinManager.h"
#include <LightLayer.h>
#include <AboveLightBlocker.h>
#include <iostream>

BlockColumn::BlockColumn(WorldManager* wm, PerlinManager* perlinManager, int bX, int bY)
	:begin(0), end(BLOCKS_SIZE - 1), wm(wm), perlinManager(perlinManager), bX(bX), bY(bY), alb(nullptr)
{
	groundY = perlinManager->getPerlinVal(bX, 100, 230);
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
	if (bY >= 0 && bY < MAX_BY)
	{
		for (int i = 0; i < airRanges.size(); i++)
		{
			if (bY == airRanges.at(i).first || bY == airRanges.at(i).second)
			{
				return new Block(wm);
			}
		}
	}
	return nullptr;
}

void BlockColumn::draw(double x)
{
	double y;
	if (wm->getPixelWorldY() < 0)
	{
		y = (int)(-wm->getPixelWorldY()) % Block::BLOCK_WIDTH - Block::BLOCK_WIDTH;
	}
	else
	{
		y = (int)(-(abs((int)wm->getPixelWorldY()) % Block::BLOCK_WIDTH));
	}
	if (bY > groundY)
	{
		if (alb == nullptr)
		{
			alb = new AboveLightBlocker(x, 0, Block::BLOCK_WIDTH);
			wm->getLightLayer()->addAboveLightBlocker(alb);
		}
		alb->setGlobalX(x);
	}
	if (bY <= groundY)
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
		}
		else
		{
			if (blocks.at(end) != nullptr)
			{
				delete blocks.at(end);
			}
			blocks.at(end) = initBlock(bY + BLOCKS_SIZE - 1);
		}
	}
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
