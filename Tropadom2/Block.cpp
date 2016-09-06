#include "Block.h"
#include "WorldManager.h"
#include "TerrainManager.h"
#include <LightBlockerContainer.h>
#include <ShapeRenderer.h>


Block::Block(WorldManager* wm, TerrainManager* tm)
	:wm(wm), tm(tm), firstDraw(true), lbc(nullptr)
{
	texture = tm->getTexture(1);
}

void Block::initLight(Block* n, Block* e, Block* s, Block* w)
{
	if (n != nullptr && e != nullptr && s != nullptr && w != nullptr)
	{
		return;
	}
	
	if (lbc != nullptr)
	{
		delete lbc;
		lbc = nullptr;
	}
	lbc = new LightBlockerContainer(wm->getLightLayer());
	
	if (n == nullptr)
	{
		lbc->addLine(0, 0, BLOCK_WIDTH, 0);
	}
	if (e == nullptr)
	{
		lbc->addLine(BLOCK_WIDTH, 0, BLOCK_WIDTH, BLOCK_WIDTH);
	}
	if (s == nullptr)
	{
		lbc->addLine(0, BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH);
	}
	if (w == nullptr)
	{
		lbc->addLine(0, BLOCK_WIDTH, 0, 0);
	}
}

void Block::draw(double x, double y)
{
	if (lbc != nullptr)
	{
		lbc->setXY(x, y);
	}
	texture->draw(x, y);
}


Block::~Block()
{
	if (lbc != nullptr)
	{
		delete lbc;
		lbc = nullptr;
	}
}
