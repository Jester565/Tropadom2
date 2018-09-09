#include "BlockCreator.h"
#include "StoneBrick.h"
#include "Grass.h"
#include "Dirt.h"
#include "CobbleStone.h"
#include "IronOre.h"

BlockCreator::BlockCreator()
{
}

void BlockCreator::init(WorldManager* wm, TerrainManager* tm)
{
	blocks.push_back(new StoneBrick(wm, tm));
	blocks.push_back(new Grass(wm, tm));
	blocks.push_back(new Dirt(wm, tm));
	blocks.push_back(new CobbleStone(wm, tm));
	blocks.push_back(new IronOre(wm, tm));
}

Block * BlockCreator::getBlock(uint16_t type)
{
	if (type > 0)
	{
		return blocks.at(type - 1)->copy();
	}
	return nullptr;
}

BlockCreator::~BlockCreator()
{
}
