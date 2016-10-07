#pragma once
#include "ShadeBlock.h"

class Grass : public ShadeBlock
{
	friend class BlockCreator;
public:
	static const uint16_t GRASS_ID = 2;
	static AllegroExt::Graphics::Image* GrassImg;
	
	~Grass();

protected:
	Grass(WorldManager* wm, TerrainManager* tm);
	Grass(const Grass& original);
	Block* copy() override;
};

