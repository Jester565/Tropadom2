#pragma once
#include <vector>

class Block;
class WorldManager;
class TerrainManager;

class BlockCreator
{
public:
	BlockCreator();

	void init(WorldManager* wm, TerrainManager* tm);

	Block* getBlock(uint16_t type);

	int getBlockTypeSize()
	{
		return blocks.size();
	}

	~BlockCreator();

private:
	std::vector <Block*> blocks;
};

