#include "Dirt.h"
#include "TerrainConstants.h"

AllegroExt::Graphics::Image* Dirt::DirtImg = nullptr;

Dirt::Dirt(WorldManager * wm, TerrainManager * tm)
	:ShadeBlock(wm, tm)
{
	blockID = DIRT_ID;
	DirtImg = new AllegroExt::Graphics::Image("dirt.jpg", BLOCK_WIDTH, BLOCK_WIDTH);
}

Dirt::Dirt(const Dirt & original)
	:ShadeBlock(original)
{
	texture = DirtImg;
}

Block * Dirt::copy()
{
	return new Dirt(*this);
}

Dirt::~Dirt()
{
}
