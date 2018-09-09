#include "CobbleStone.h"
#include "TerrainConstants.h"

AllegroExt::Graphics::Image* CobbleStone::CobbleStoneImg = nullptr;

CobbleStone::CobbleStone(WorldManager * wm, TerrainManager * tm)
	:ShadeBlock(wm, tm)
{
	blockID = COBBLE_STONE_ID;
	CobbleStoneImg = new AllegroExt::Graphics::Image("cobble.jpg", BLOCK_WIDTH, BLOCK_WIDTH);
}

CobbleStone::CobbleStone(const CobbleStone & original)
	:ShadeBlock(original)
{
	texture = CobbleStoneImg;
}

Block * CobbleStone::copy()
{
	return new CobbleStone(*this);
}

CobbleStone::~CobbleStone()
{
}
