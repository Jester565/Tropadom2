#pragma once
#include "ShadeBlock.h"

class CobbleStone : public ShadeBlock
{
	friend class BlockCreator;
public:
	static const uint16_t COBBLE_STONE_ID = 4;
	static AllegroExt::Graphics::Image* CobbleStoneImg;
	~CobbleStone();

protected:
	CobbleStone(WorldManager* wm, TerrainManager* tm);
	CobbleStone(const CobbleStone& original);
	Block* copy() override;
};
