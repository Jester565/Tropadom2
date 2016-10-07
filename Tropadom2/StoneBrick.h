#pragma once
#include "ShadeBlock.h"

class StoneBrick : public ShadeBlock
{
	friend class BlockCreator;
public:
	static const uint16_t STONE_BRICK_ID = 1;
	static AllegroExt::Graphics::Image* StoneBrickImg;

	~StoneBrick();

protected:
	StoneBrick(WorldManager* wm, TerrainManager* tm);
	StoneBrick(const StoneBrick& original);
	Block* copy() override;
};

