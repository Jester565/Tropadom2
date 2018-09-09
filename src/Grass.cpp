#include "Grass.h"
#include "TerrainConstants.h"

AllegroExt::Graphics::Image* Grass::GrassImg = nullptr;

Grass::Grass(WorldManager * wm, TerrainManager * tm)
	:ShadeBlock(wm, tm)
{
	blockID = GRASS_ID;
	GrassImg = new AllegroExt::Graphics::Image("grass.jpg", BLOCK_WIDTH, BLOCK_WIDTH);
}

Grass::Grass(const Grass & original)
	:ShadeBlock(original)
{
	texture = GrassImg;
}

Block* Grass::copy()
{
	return new Grass(*this);
}

Grass::~Grass()
{
}
