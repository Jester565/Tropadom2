/*
#include "BlockColumn.h"
#include "WorldManager.h"
#include "PerlinManager.h"
#include "TerrainManager.h"
#include "CaveManager.h"
#include <LightLayer.h>
#include <AboveLightBlocker.h>
#include <iostream>

BlockColumn::BlockColumn(WorldManager* wm, TerrainManager* tm, PerlinManager* perlinManager, int bX, int bY)
	:begin(0), end(BLOCK_LOAD_BH - 1), wm(wm), perlinManager(perlinManager), bX(bX), bY(bY), alb(nullptr), tm(tm), initLightCalled(false)
{
	groundY = perlinManager->getPerlinVal(bX, 100, 60);
	airRanges.emplace_back(-1, groundY);
	initBlocks();
}

void BlockColumn::initBlocks()
{
	for (int i = 0; i < BLOCK_LOAD_BH; i++)
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
	if (rowI >= BLOCK_LOAD_BH)
	{
		rowI -= BLOCK_LOAD_BH;
	}
	if (rowI < BLOCK_LOAD_BH && rowI >= 0)
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
		y = (int)(-wm->getPixelWorldY()) % BLOCK_WIDTH - BLOCK_WIDTH * (BLOCK_DRAW_OFF + 1);
	}
	else
	{
		y = (int)(-(abs((int)wm->getPixelWorldY()) % BLOCK_WIDTH)) - BLOCK_WIDTH * (BLOCK_DRAW_OFF);
	}
	if (bY + 1 + BLOCK_OFF_BH > groundY && x >= -BLOCK_WIDTH * 2 && x <= STANDARD_WIDTH)
	{
		if (alb == nullptr)
		{
			alb = new AboveLightBlocker(x, 0, BLOCK_WIDTH);
			wm->getLightLayer()->addAboveLightBlocker(alb);
		}
		alb->setGlobalX(x);
	}
	else
	{
		if (alb != nullptr)
		{
			wm->getLightLayer()->removeAboveLightBlocker(alb);
			delete alb;
			alb = nullptr;
		}
	}
	int bIndex = begin + BLOCK_OFF_BH;
	if (bIndex >= BLOCK_LOAD_BH)
	{
		bIndex -= BLOCK_LOAD_BH;
	}
	for (int i = 0; i < BLOCK_DRAW_BH; i++)
	{
		if (blocks.at(bIndex) != nullptr)
		{
			blocks.at(bIndex)->draw(x, y);
		}
		y += BLOCK_WIDTH;
		bIndex++;
		if (bIndex >= BLOCK_LOAD_BH)
		{
			bIndex = 0;
		}
	}
}

void BlockColumn::drawLBDebug(double x)
{
	double y;
	if (wm->getPixelWorldY() < 0)
	{
		y = (int)(-wm->getPixelWorldY()) % BLOCK_WIDTH - BLOCK_WIDTH * (BLOCK_DRAW_OFF + BLOCK_OFF_BH + 1);
	}
	else
	{
		y = (int)(-(abs((int)wm->getPixelWorldY()) % BLOCK_WIDTH)) - BLOCK_WIDTH * (BLOCK_DRAW_OFF + BLOCK_OFF_BH);
	}
	if (bY + 1 + BLOCK_OFF_BH > groundY && x >= -BLOCK_WIDTH * 2 && x <= STANDARD_WIDTH)
	{
		if (alb == nullptr)
		{
			alb = new AboveLightBlocker(x, 0, BLOCK_WIDTH);
			wm->getLightLayer()->addAboveLightBlocker(alb);
		}
		alb->setGlobalX(x);
	}
	else
	{
		if (alb != nullptr)
		{
			wm->getLightLayer()->removeAboveLightBlocker(alb);
			delete alb;
			alb = nullptr;
		}
	}
	int bIndex = begin;
	for (int i = 0; i < BLOCK_LOAD_BH; i++)
	{
		if (blocks.at(bIndex) != nullptr)
		{
			blocks.at(bIndex)->draw(x, y);
		}
		y += BLOCK_WIDTH;
		bIndex++;
		if (bIndex >= BLOCK_LOAD_BH)
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
		if (begin >= BLOCK_LOAD_BH)
		{
			begin = 0;
		}
		if (begin < 0)
		{
			begin = BLOCK_LOAD_BH - 1;
		}
		if (begin == 0)
		{
			end = BLOCK_LOAD_BH - 1;
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
				int lbDestroy = (begin + BLOCK_OFF_BH) % BLOCK_LOAD_BH;
				if (blocks.at(lbDestroy) != nullptr && blocks.at(lbDestroy)->lbc != nullptr)
				{
					delete blocks.at(lbDestroy)->lbc;
					blocks.at(lbDestroy)->lbc = nullptr;
				}
				int lbInit = (begin + BLOCK_OFF_BH + 1) % BLOCK_LOAD_BH;
				if (blocks.at(lbInit) != nullptr && blocks.at(lbInit)->lbc == nullptr)
				{
					int lbSouth = (lbInit + 1) % BLOCK_LOAD_BH;
					int lbNorth = (lbInit - 1);
					if (lbNorth < 0)
					{
						lbNorth = BLOCK_LOAD_BH - 1;
					}
					blocks.at(lbInit)->initLight(blocks.at(lbNorth), tm->getBlock(bX + 1, bY + 1 + BLOCK_OFF_BH), blocks.at(lbSouth), tm->getBlock(bX - 1, bY + 1 + BLOCK_OFF_BH));
				}
			}
		}
		else
		{
			if (blocks.at(end) != nullptr)
			{
				delete blocks.at(end);
			}
			blocks.at(end) = initBlock(bY + BLOCK_LOAD_BH - 1);
			if (initLightCalled)
			{
				int lbDestroy = end - BLOCK_OFF_BH;
				if (lbDestroy < 0)
				{
					lbDestroy += BLOCK_LOAD_BH;
				}
				if (blocks.at(lbDestroy) != nullptr && blocks.at(lbDestroy)->lbc != nullptr)
				{
					delete blocks.at(lbDestroy)->lbc;
					blocks.at(lbDestroy)->lbc = nullptr;
				}
				int lbInit = end - BLOCK_OFF_BH - 1;
				if (lbInit < 0)
				{
					lbInit += BLOCK_LOAD_BH;
				}
				if (blocks.at(lbInit) != nullptr && blocks.at(lbInit)->lbc == nullptr)
				{
					int lbNorth = lbInit - 1;
					if (lbNorth < 0)
					{
						lbNorth = BLOCK_LOAD_BH - 1;
					}
					int lbSouth = (lbInit + 1) % BLOCK_LOAD_BH;
					blocks.at(lbInit)->initLight(blocks.at(lbNorth), tm->getBlock(bX + 1, bY + BLOCK_LOAD_BH - 2 - BLOCK_OFF_BH), blocks.at(lbSouth), tm->getBlock(bX - 1, bY + BLOCK_LOAD_BH - 2 - BLOCK_OFF_BH));
				}
			} 
		}
	}
}

void BlockColumn::initLight()
{
	if (!initLightCalled)
	{
		int bIndex = (begin + BLOCK_OFF_BH) % BLOCK_LOAD_BH;
		for (int i = 0; i < BLOCK_DRAW_BH; i++)
		{
			if (i >= BLOCK_LIGHT_OFF && i < BLOCK_DRAW_BH - BLOCK_LIGHT_OFF && blocks.at(bIndex) != nullptr)
			{
				int northBlock = bIndex - 1;
				if (northBlock < 0)
				{
					northBlock = BLOCK_LOAD_BH - 1;
				}
				int southBlock = bIndex + 1;
				if (southBlock >= BLOCK_LOAD_BH)
				{
					southBlock = 0;
				}
				blocks.at(bIndex)->initLight(blocks.at(northBlock), tm->getBlock(bX + 1, bY + i + BLOCK_OFF_BH), blocks.at(southBlock), tm->getBlock(bX - 1, bY + i + BLOCK_OFF_BH));
			}
			bIndex++;
			if (bIndex >= BLOCK_LOAD_BH)
			{
				bIndex = 0;
			}
		}
		initLightCalled = true;
	}
}

void BlockColumn::destroyLight()
{
	if (initLightCalled)
	{
		for (int i = 0; i < BLOCK_LOAD_BH; i++)
		{
			if (blocks.at(i) != nullptr && blocks.at(i)->lbc != nullptr)
			{
				blocks.at(i)->destroyLBC();
			}
		}
		initLightCalled = false;
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
*/


#include "BlockColumn.h"
#include "WorldManager.h"
#include "PerlinManager.h"
#include "TerrainManager.h"
#include "CaveManager.h"
#include <LightLayer.h>
#include <AboveLightBlocker.h>
#include <iostream>

BlockColumn::BlockColumn(WorldManager* wm, TerrainManager* tm, PerlinManager* perlinManager, int bX, int bY)
	:begin(0), end(BLOCK_LOAD_BH - 1), wm(wm), perlinManager(perlinManager), bX(bX), bY(bY), alb(nullptr), tm(tm), initLightCalled(false)
{
	groundY = perlinManager->getPerlinVal(bX, 100, 60);
	airRanges.emplace_back(-1, groundY);
	initBlocks();
}

void BlockColumn::initBlocks()
{
	for (int i = 0; i < BLOCK_LOAD_BH; i++)
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
	if (rowI >= BLOCK_LOAD_BH)
	{
		rowI -= BLOCK_LOAD_BH;
	}
	if (rowI < BLOCK_LOAD_BH && rowI >= 0)
	{
		return blocks.at(rowI);
	}
	return nullptr;
}

void BlockColumn::draw(double x, uint8_t textureID)
{
	double y;
	if (wm->getPixelWorldY() < 0)
	{
		y = (int)(-wm->getPixelWorldY()) % BLOCK_WIDTH - BLOCK_WIDTH * (BLOCK_DRAW_OFF + BLOCK_LIGHT_OFF + 1);
	}
	else
	{
		y = (int)(-(abs((int)wm->getPixelWorldY()) % BLOCK_WIDTH)) - BLOCK_WIDTH * (BLOCK_LIGHT_OFF + BLOCK_DRAW_OFF);
	}
	if (bY + BLOCK_LIGHT_OFF_BH >= groundY)
	{
		if (alb == nullptr)
		{
			alb = new AboveLightBlocker(x, 0, BLOCK_WIDTH);
			wm->getLightLayer()->addAboveLightBlocker(alb);
		}
		alb->setGlobalX(x);
	}
	else
	{
		if (alb != nullptr)
		{
			wm->getLightLayer()->removeAboveLightBlocker(alb);
			delete alb;
			alb = nullptr;
		}
	}
	int bIndex = (begin + BLOCK_LIGHT_OFF_BH) % BLOCK_LOAD_BH;
	for (int i = 0; i < BLOCK_DRAW_BH + BLOCK_LIGHT_OFF * 2; i++)
	{
		if (blocks.at(bIndex) != nullptr)
		{
			if (i > BLOCK_LIGHT_OFF && i <= BLOCK_DRAW_BH + BLOCK_LIGHT_OFF)
			{
				blocks.at(bIndex)->draw(x, y, textureID);
			}
			else if (textureID == 1)
			{
				blocks.at(bIndex)->updateLBCPos(x, y);
			}
		}
		y += BLOCK_WIDTH;
		bIndex++;
		if (bIndex >= BLOCK_LOAD_BH)
		{
			bIndex = 0;
		}
	}
}

void BlockColumn::updateLBC(double x)
{
	double y;
	if (wm->getPixelWorldY() < 0)
	{
		y = (int)(-wm->getPixelWorldY()) % BLOCK_WIDTH - BLOCK_WIDTH * (BLOCK_DRAW_OFF + BLOCK_LIGHT_OFF + 1);
	}
	else
	{
		y = (int)(-(abs((int)wm->getPixelWorldY()) % BLOCK_WIDTH)) - BLOCK_WIDTH * (BLOCK_LIGHT_OFF + BLOCK_DRAW_OFF);
	}
	int bIndex = (begin + BLOCK_LIGHT_OFF_BH) % BLOCK_LOAD_BH;
	for (int i = 0; i < BLOCK_DRAW_BH + BLOCK_LIGHT_OFF * 2; i++)
	{
		if (blocks.at(bIndex) != nullptr)
		{
			blocks.at(bIndex)->updateLBCPos(x, y);
		}
		y += BLOCK_WIDTH;
		bIndex++;
		if (bIndex >= BLOCK_LOAD_BH)
		{
			bIndex = 0;
		}
	}
}

void BlockColumn::drawLBDebug(double x, uint8_t textureID)
{
	double y;
	if (wm->getPixelWorldY() < 0)
	{
		y = (int)(-wm->getPixelWorldY()) % BLOCK_WIDTH - BLOCK_WIDTH * (BLOCK_OFF_BH + BLOCK_DRAW_OFF + 1);
	}
	else
	{
		y = (int)(-(abs((int)wm->getPixelWorldY()) % BLOCK_WIDTH)) - BLOCK_WIDTH * (BLOCK_OFF_BH + BLOCK_DRAW_OFF);
	}
	if (bY + BLOCK_LIGHT_OFF_BH > groundY)
	{
		if (alb == nullptr)
		{
			alb = new AboveLightBlocker(x, 0, BLOCK_WIDTH);
			wm->getLightLayer()->addAboveLightBlocker(alb);
		}
		alb->setGlobalX(x);
	}
	else
	{
		if (alb != nullptr)
		{
			wm->getLightLayer()->removeAboveLightBlocker(alb);
			delete alb;
			alb = nullptr;
		}
	}
	int bIndex = begin;
	for (int i = 0; i < BLOCK_LOAD_BH; i++)
	{
		if (blocks.at(bIndex) != nullptr)
		{
			blocks.at(bIndex)->draw(x, y, textureID);
			blocks.at(bIndex)->updateLBCPos(x, y);
		}
		y += BLOCK_WIDTH;
		bIndex++;
		if (bIndex >= BLOCK_LOAD_BH)
		{
			bIndex = 0;
		}
	}
}

void BlockColumn::shiftBY(int count)
{
	for (int i = 0; i < abs(count); i++)
	{
		begin += count / abs(count);
		bY += count / abs(count);
		if (begin >= BLOCK_LOAD_BH)
		{
			begin = 0;
		}
		if (begin < 0)
		{
			begin = BLOCK_LOAD_BH - 1;
		}
		if (begin == 0)
		{
			end = BLOCK_LOAD_BH - 1;
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
				int lbDestroy = end - BLOCK_LIGHT_OFF_BH + 1;
				if (lbDestroy < 0)
				{
					lbDestroy += BLOCK_LOAD_BH;
				}
				if (blocks.at(lbDestroy) != nullptr && blocks.at(lbDestroy)->lbc != nullptr)
				{
					blocks.at(lbDestroy)->destroyLBC();
				}
				int lbInit = begin + BLOCK_LIGHT_OFF_BH;
				if (lbInit >= BLOCK_LOAD_BH)
				{
					lbInit -= BLOCK_LOAD_BH;
				}
				if (blocks.at(lbInit) != nullptr && blocks.at(lbInit)->lbc == nullptr)
				{
					int lbSouth = lbInit + 1;
					if (lbSouth >= BLOCK_LOAD_BH)
					{
						lbSouth = 0;
					}
					int lbNorth = lbInit - 1;
					if (lbNorth < 0)
					{
						lbNorth = BLOCK_LOAD_BH - 1;
					}
					blocks.at(lbInit)->initLight(blocks.at(lbNorth), tm->getBlock(bX + 1, bY + BLOCK_LIGHT_OFF_BH), blocks.at(lbSouth), tm->getBlock(bX - 1, bY + BLOCK_LIGHT_OFF_BH));
				}
			}
		}
		else
		{
			if (blocks.at(end) != nullptr)
			{
				delete blocks.at(end);
			}
			blocks.at(end) = initBlock(bY + BLOCK_LOAD_BH - 1);
			if (initLightCalled)
			{
				int lbDestroy = (begin + BLOCK_LIGHT_OFF_BH - 1) % BLOCK_LOAD_BH;
				if (blocks.at(lbDestroy) != nullptr && blocks.at(lbDestroy)->lbc != nullptr)
				{
					blocks.at(lbDestroy)->destroyLBC();
				}
				int lbInit = end - BLOCK_LIGHT_OFF_BH;
				if (lbInit < 0)
				{
					lbInit += BLOCK_LOAD_BH;
				}
				if (blocks.at(lbInit) != nullptr && blocks.at(lbInit)->lbc == nullptr)
				{
					int lbNorth = lbInit - 1;
					if (lbNorth < 0)
					{
						lbNorth = BLOCK_LOAD_BH - 1;
					}
					int lbSouth = lbInit + 1;
					if (lbSouth >= BLOCK_LOAD_BH)
					{
						lbSouth = 0;
					}
					blocks.at(lbInit)->initLight(blocks.at(lbNorth), tm->getBlock(bX + 1, bY + BLOCK_LOAD_BH - BLOCK_LIGHT_OFF_BH - 1), blocks.at(lbSouth), tm->getBlock(bX - 1, bY + BLOCK_LOAD_BH - BLOCK_LIGHT_OFF_BH - 1));
				}
			}
		}
	}
}

void BlockColumn::initLight()
{
	if (!initLightCalled)
	{
		int bIndex = (begin + BLOCK_LIGHT_OFF_BH) % BLOCK_LOAD_BH;
		for (int i = BLOCK_LIGHT_OFF_BH; i <= BLOCK_LOAD_BH - BLOCK_LIGHT_OFF_BH; i++)
		{
			if (blocks.at(bIndex) != nullptr)
			{
				int northBlock = bIndex - 1;
				if (northBlock < 0)
				{
					northBlock = BLOCK_LOAD_BH - 1;
				}
				int southBlock = bIndex + 1;
				if (southBlock >= BLOCK_LOAD_BH)
				{
					southBlock = 0;
				}
				blocks.at(bIndex)->initLight(blocks.at(northBlock), tm->getBlock(bX + 1, bY + i), blocks.at(southBlock), tm->getBlock(bX - 1, bY + i));
			}
			bIndex++;
			if (bIndex >= BLOCK_LOAD_BH)
			{
				bIndex = 0;
			}
		}
		initLightCalled = true;
	}
}

void BlockColumn::destroyLight()
{
	if (initLightCalled)
	{
		for (int i = 0; i < BLOCK_LOAD_BH; i++)
		{
			if (blocks.at(i) != nullptr)
			{
				blocks.at(i)->destroyLBC();
			}
		}
		initLightCalled = false;
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

