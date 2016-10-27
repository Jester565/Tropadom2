#include "BlockColumn.h"
#include "WorldManager.h"
#include "TerrainManager.h"
#include <LightLayer.h>
#include <AboveLightBlocker.h>
#include <iostream>

BlockColumn::BlockColumn(WorldManager* wm, TerrainManager* tm, int bX, int bY)
	:begin(0), end(BLOCK_LOAD_BH - 1), wm(wm), bX(bX), bY(bY), alb(nullptr), tm(tm), initLightCalled(false)
{
	groundY = 10;	//TODO: Get groundY from the server
	blocks.resize(BLOCK_LOAD_BH, nullptr);
}

Block* BlockColumn::getBlock(int searchBY)
{
	int rowI = getBlockI(searchBY);
	if (rowI != -1)
	{
		return blocks.at(rowI);
	}
	return nullptr;
}

int BlockColumn::getBlockI(int searchBY)
{
	int rowI = searchBY - bY;
	rowI += begin;
	if (rowI >= BLOCK_LOAD_BH)
	{
		rowI -= BLOCK_LOAD_BH;
	}
	if (rowI < BLOCK_LOAD_BH && rowI >= 0)
	{
		return rowI;
	}
	return -1;
}

Block * BlockColumn::getBlockAtPixels(double y)
{
	int bI = getIndexAtPixels(y);
	if (bI != -1)
	{
		return blocks.at(bI);
	}
	return nullptr;
}

int BlockColumn::getIndexAtPixels(double y)
{
	int yOff;
	if (wm->getPixelWorldY() < 0)
	{
		yOff = (int)(-wm->getPixelWorldY()) % BLOCK_WIDTH - BLOCK_WIDTH * (BLOCK_DRAW_OFF + BLOCK_LIGHT_OFF + BLOCK_LIGHT_OFF_BH + 1);
	}
	else
	{
		yOff = (int)(-(abs((int)wm->getPixelWorldY()) % BLOCK_WIDTH)) - BLOCK_WIDTH * (BLOCK_LIGHT_OFF + BLOCK_DRAW_OFF + BLOCK_LIGHT_OFF_BH);
	}
	int bI = (y - yOff) / BLOCK_WIDTH;
	if (bI >= 0 && bI < BLOCK_LOAD_BH)
	{
		bI += begin;
		bI %= BLOCK_LOAD_BH;
		return bI;
	}
	return -1;
}

void BlockColumn::draw(double x, uint16_t blockID)
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
#ifdef USE_LIGHT_V4
			alb = new lighting::AboveLightBlocker(wm->getLightLayer(), x, 0, BLOCK_WIDTH);
#else
			alb = new AboveLightBlocker(x, 0, BLOCK_WIDTH);
			wm->getLightLayer()->addAboveLightBlocker(alb);
#endif
		}
#ifdef USE_LIGHT_V4
		alb->setX(x);
#else
		alb->setGlobalX(x);
#endif
	}
	else
	{
		if (alb != nullptr)
		{
#ifndef USE_LIGHT_V4
			wm->getLightLayer()->removeAboveLightBlocker(alb);
#endif
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
				if (blockID == blocks.at(bIndex)->getBlockID())
				{
					blocks.at(bIndex)->draw(x, y);
				}
			}
			else if (blockID == 1)
			{
				blocks.at(bIndex)->updateSurround(x, y);
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
	if (alb != nullptr)
	{
#ifndef USE_LIGHT_V4
		wm->getLightLayer()->removeAboveLightBlocker(alb);
#endif
		delete alb;
		alb = nullptr;
	}
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
			blocks.at(bIndex)->updateSurround(x, y);
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
				blocks.at(begin) = nullptr;
			}
			if (initLightCalled)
			{
				int lbDestroy = end - BLOCK_LIGHT_OFF_BH + 1;
				if (lbDestroy < 0)
				{
					lbDestroy += BLOCK_LOAD_BH;
				}
				if (blocks.at(lbDestroy) != nullptr)
				{
					blocks.at(lbDestroy)->destroySurround();
				}
				int lbInit = begin + BLOCK_LIGHT_OFF_BH;
				if (lbInit >= BLOCK_LOAD_BH)
				{
					lbInit -= BLOCK_LOAD_BH;
				}
				if (blocks.at(lbInit) != nullptr && !blocks.at(lbInit)->isTransparent() && ((ShadeBlock*)blocks.at(lbInit))->lbc == nullptr)
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
					blocks.at(lbInit)->initSurround(blocks.at(lbNorth), tm->getBlock(bX + 1, bY + BLOCK_LIGHT_OFF_BH), blocks.at(lbSouth), tm->getBlock(bX - 1, bY + BLOCK_LIGHT_OFF_BH), bX, bY + BLOCK_LIGHT_OFF_BH);
				}
			}
		}
		else
		{
			if (blocks.at(end) != nullptr)
			{
				delete blocks.at(end);
				blocks.at(end) = nullptr;
			}
			if (initLightCalled)
			{
				int lbDestroy = (begin + BLOCK_LIGHT_OFF_BH - 1) % BLOCK_LOAD_BH;
				if (blocks.at(lbDestroy) != nullptr && !blocks.at(lbDestroy)->isTransparent())
				{
					blocks.at(lbDestroy)->destroySurround();
				}
				int lbInit = end - BLOCK_LIGHT_OFF_BH;
				if (lbInit < 0)
				{
					lbInit += BLOCK_LOAD_BH;
				}
				if (blocks.at(lbInit) != nullptr && !blocks.at(lbInit)->isTransparent() && ((ShadeBlock*)blocks.at(lbInit))->lbc == nullptr)
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
					blocks.at(lbInit)->initSurround(blocks.at(lbNorth), tm->getBlock(bX + 1, bY + BLOCK_LOAD_BH - BLOCK_LIGHT_OFF_BH - 1), blocks.at(lbSouth), tm->getBlock(bX - 1, bY + BLOCK_LOAD_BH - BLOCK_LIGHT_OFF_BH - 1), bX, bY + BLOCK_LOAD_BH - BLOCK_LIGHT_OFF_BH - 1);
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
			if (blocks.at(bIndex) != nullptr && !blocks.at(bIndex)->isTransparent())
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
				blocks.at(bIndex)->initSurround(blocks.at(northBlock), tm->getBlock(bX + 1, bY + i), blocks.at(southBlock), tm->getBlock(bX - 1, bY + i), bX, bY + i);
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

void BlockColumn::reInitLight(int bI)
{
	int adjustedBI = bI - begin;
	if (bI < begin)
	{
		adjustedBI = bI + (BLOCK_LOAD_BH - begin);
	}
	if (adjustedBI >= BLOCK_LIGHT_OFF_BH && adjustedBI < BLOCK_LOAD_BH - BLOCK_LIGHT_OFF_BH)
	{
		if (blocks.at(bI) != nullptr && !blocks.at(bI)->isTransparent())
		{
			int lbSouth = bI + 1;
			if (lbSouth >= BLOCK_LOAD_BH)
			{
				lbSouth = 0;
			}
			int lbNorth = bI - 1;
			if (lbNorth < 0)
			{
				lbNorth = BLOCK_LOAD_BH - 1;
			}
			blocks.at(bI)->initSurround(blocks.at(lbNorth), tm->getBlock(bX + 1, bY + adjustedBI), blocks.at(lbSouth), tm->getBlock(bX - 1, bY + adjustedBI), bX, bY + adjustedBI);
		}
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
				blocks.at(i)->destroySurround();
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
#ifndef USE_LIGHT_V4
		wm->getLightLayer()->removeAboveLightBlocker(alb);
#endif
		delete alb;
	}
}

