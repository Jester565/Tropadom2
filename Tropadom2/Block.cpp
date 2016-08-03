#include "Block.h"
#include <ShapeRenderer.h>


Block::Block()
{
}

void Block::draw(double x, double y)
{
	AllegroExt::Graphics::ShapeRenderer::drawRectangle(x, y, BLOCK_WIDTH, BLOCK_WIDTH, 255, 0, 0, 255);
}


Block::~Block()
{
}
