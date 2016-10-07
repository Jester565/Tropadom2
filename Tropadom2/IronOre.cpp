#include "IronOre.h"
#include "TerrainConstants.h"

AllegroExt::Graphics::Image* IronOre::IronOreImg = nullptr;

IronOre::~IronOre()
{
}

IronOre::IronOre(WorldManager * wm, TerrainManager * tm)
	:ShadeBlock(wm, tm)
{
	blockID = IRON_ORE_ID;
	IronOreImg = new AllegroExt::Graphics::Image("iron.jpg", BLOCK_WIDTH, BLOCK_WIDTH);
}

IronOre::IronOre(const IronOre & original)
	:ShadeBlock(original)
{
	texture = IronOreImg;
}

Block * IronOre::copy()
{
	return new IronOre(*this);
}
