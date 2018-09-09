#include "ShadeBlock.h"
#include "TerrainConstants.h"
#include "WorldManager.h"
#include <LightBlockerContainer.h>

ShadeBlock::ShadeBlock(WorldManager* wm, TerrainManager* tm)
	:Block(wm, tm)
{
}

ShadeBlock::ShadeBlock(const ShadeBlock & original)
	:Block(original), lbc(nullptr)
{

}

void ShadeBlock::draw(float pX, float pY)
{
	if (lbc != nullptr)
	{
		lbc->setXY(pX, pY);
	}
	Block::draw(pX, pY);
}

void ShadeBlock::initSurround(Block* n, Block* e, Block* s, Block* w, int64_t bX, int64_t bY)
{
	Block::initSurround(n, e, s, w, bX, bY);
	if (n != nullptr && e != nullptr && s != nullptr && w != nullptr)
	{
		return;
	}
	if (lbc != nullptr)
	{
		delete lbc;
		lbc = nullptr;
	}
	if (n == nullptr || n->isTransparent())
	{
		if (lbc == nullptr)
		{
			lbc = new LightBlockerContainer(wm->getLightLayer());
		}
		lbc->addLine(0, 0, BLOCK_WIDTH, 0);
	}
	if (e == nullptr || e->isTransparent())
	{
		if (lbc == nullptr)
		{
			lbc = new LightBlockerContainer(wm->getLightLayer());
		}
		lbc->addLine(BLOCK_WIDTH, 0, BLOCK_WIDTH, BLOCK_WIDTH);
	}
	if (s == nullptr || s->isTransparent())
	{
		if (lbc == nullptr)
		{
			lbc = new LightBlockerContainer(wm->getLightLayer());
		}
		lbc->addLine(0, BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH);
	}
	if (w == nullptr || w->isTransparent())
	{
		if (lbc == nullptr)
		{
			lbc = new LightBlockerContainer(wm->getLightLayer());
		}
		lbc->addLine(0, BLOCK_WIDTH, 0, 0);
	}
	lbc->setXY(0, STANDARD_HEIGHT * 5);
}
void ShadeBlock::updateSurround(float pX, float pY)
{
	Block::updateSurround(pX, pY);
	if (lbc != nullptr)
	{
		lbc->setXY(pX, pY);
	}
}

void ShadeBlock::destroySurround()
{
	Block::destroySurround();
	if (lbc != nullptr)
	{
		delete lbc;
		lbc = nullptr;
	}
}

ShadeBlock::~ShadeBlock()
{
	
}
