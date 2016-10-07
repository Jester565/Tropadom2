#pragma once
#include "ShadeBlock.h"

class IronOre : public ShadeBlock
{
	friend class BlockCreator;
public:
	static const uint16_t IRON_ORE_ID = 5;
	static AllegroExt::Graphics::Image* IronOreImg;

	~IronOre();

protected:
	IronOre(WorldManager* wm, TerrainManager* tm);
	IronOre(const IronOre& original);
	Block* copy() override;
};

