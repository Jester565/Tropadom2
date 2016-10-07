#include "StoneBrick.h"
#include "TerrainConstants.h"

AllegroExt::Graphics::Image* StoneBrick::StoneBrickImg = nullptr;

StoneBrick::StoneBrick(WorldManager* wm, TerrainManager* tm)
	:ShadeBlock(wm, tm)
{
	blockID = STONE_BRICK_ID;
	StoneBrickImg = new AllegroExt::Graphics::Image("block.png", BLOCK_WIDTH, BLOCK_WIDTH);
}

StoneBrick::StoneBrick(const StoneBrick & original)
	:ShadeBlock(original)
{
	texture = StoneBrickImg;
}

Block * StoneBrick::copy()
{
	return new StoneBrick(*this);
}

StoneBrick::~StoneBrick()
{
}
