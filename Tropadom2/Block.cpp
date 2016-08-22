#include "Block.h"
#include "WorldManager.h"
#include <LightBlockerContainer.h>
#include <ShapeRenderer.h>


Block::Block(WorldManager* wm)
	:wm(wm), firstDraw(true)
{
	if (WorldManager::LIGHTING_ENABLED)
	{
		lbc = new LightBlockerContainer(wm->getLightLayer());
		lbc->initSquare(BLOCK_WIDTH, BLOCK_WIDTH);
	}
}

void Block::draw(double x, double y)
{
	if (WorldManager::LIGHTING_ENABLED)
	{
		lbc->setXY(x, y);
	}
	AllegroExt::Graphics::ShapeRenderer::drawRectangle(x, y, BLOCK_WIDTH, BLOCK_WIDTH, 255, 0, 0, 255);
}


Block::~Block()
{
	if (WorldManager::LIGHTING_ENABLED)
	{
		delete lbc;
	}
}
