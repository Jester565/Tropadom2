#pragma once
#include <PKeyOwner.h>
#include "TerrainConstants.h"
#include "Block.h"
#include <DisplayManager.h>
#include <Image.h>
#include <boost/thread/mutex.hpp>
#include <vector>

class BlockCreator;
class BlockColumn;
class WorldManager;
class Client;

class TerrainManager : public PKeyOwner
{
public:
	
	TerrainManager(WorldManager* wm, Client* client);

	bool isLoaded()
	{
		return loaded;
	}

	void requestCol(bool posShift);

	void requestRow(bool posShift);

	void reInitLight(int bcI, int bI);

	void draw();

	Block* getBlockAtPixels(double x, double y);

	std::pair <int, int> getIndexsAtPixels(double x, double y);

	std::pair <int, int> getIndexAtBlockCords(int64_t bX, int64_t bY);

	void replaceBlock(std::pair <int, int> blockII, uint16_t newType);

	void translate(float dX, float dY);

	Block* getBlock(int bX, int bY);

	int getBlockColumnI(int bX);

	BlockColumn* getBlockColumn(int bX);

	AllegroExt::Graphics::Image* getTexture(int id)
	{
		return blockTextures.at(id - 1);
	}

	int64_t getBX()
	{
		return bX;
	}

	int64_t getBY()
	{
		return bY;
	}

	~TerrainManager();

protected:
	void sendC0();
	std::vector<BlockColumn*> blockCols;
	std::vector<AllegroExt::Graphics::Image*> blockTextures;
	void shiftBX(int count);
	void shiftBY(int count);
	int begin;
	int end;
	int64_t bX;
	int64_t bY;
	WorldManager* wm;
	BlockCreator* blockCreator;

	void keyC1(boost::shared_ptr<IPacket> iPack);
	void keyC3(boost::shared_ptr<IPacket> iPack);
	void keyD0(boost::shared_ptr<IPacket> iPack);
	void keyD1(boost::shared_ptr<IPacket> iPack);
	boost::mutex shiftMutex;
	bool loaded;
	int shiftCount;
	int c3ShiftCount;
	Client* client;
};

