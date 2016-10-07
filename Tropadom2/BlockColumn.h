#pragma once
#include "ShadeBlock.h"
#include "TerrainConstants.h"
#include <DisplayManager.h>
#include <vector>

class WorldManager;
class PerlinManager;
class AboveLightBlocker;
class TerrainManager;

class BlockColumn
{
public:

	BlockColumn(WorldManager* wm, TerrainManager* tm, int bX, int bY);
	void draw(double x, uint16_t blockID);
	void updateLBC(double x);
	void shiftBY(int count);
	void initLight();
	void reInitLight(int bI);
	void destroyLight();
	Block* getBlock(int bY);

	int getBlockI(int bY);

	std::vector<Block*> blocks;

	Block* getBlockAtPixels(double y);

	int getIndexAtPixels(double y);

	int64_t getBX()
	{
		return bX;
	}

	int64_t getBY()
	{
		return bY;
	}

	int getBegin()
	{
		return begin;
	}

	int getGroundY()
	{
		return groundY;
	}

	void setGroundY(uint32_t groundY)
	{
		this->groundY = groundY;
	}

	~BlockColumn();

private:
	int64_t bX;
	int64_t bY;
	int begin;
	int end;
	uint32_t groundY;
	AboveLightBlocker* alb;
	WorldManager* wm;
	TerrainManager* tm;
	bool initLightCalled;
};

