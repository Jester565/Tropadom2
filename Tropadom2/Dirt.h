#pragma once
#include "ShadeBlock.h"

class Dirt : public ShadeBlock
{
	friend class BlockCreator;
public:
	static const uint16_t DIRT_ID = 3;
	static AllegroExt::Graphics::Image* DirtImg;
	~Dirt();

protected:
	Dirt(WorldManager* wm, TerrainManager* tm);
	Dirt(const Dirt& original);
	Block* copy() override;
};

