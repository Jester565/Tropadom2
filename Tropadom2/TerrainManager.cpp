#include "TerrainManager.h"
#include "PerlinManager.h"
#include "CaveManager.h"
#include "BlockColumn.h"
#include "BlockCreator.h"
#include "WorldManager.h"
#include "DebugBox.h"
#include "Packets/Trop.pb.h"
#include <Client.h>
#include <IPacket.h>
#include <OPacket.h>
#include <TCPConnection.h>
#include <time.h>

TerrainManager::TerrainManager(WorldManager* wm, Client* client)
	:PKeyOwner(client->getPacketManager()), client(client), loaded(false), begin(0), end(BLOCK_LOAD_BW - 1), wm(wm), bX(0), bY(INITIAL_BY)
{
	addKey(new PKey("C1", this, &TerrainManager::keyC1));
	addKey(new PKey("C3", this, &TerrainManager::keyC3));
	addKey(new PKey("D0", this, &TerrainManager::keyD0));
	addKey(new PKey("D1", this, &TerrainManager::keyD1));
	blockCreator = new BlockCreator();
	blockCreator->init(wm, this);
	sendC0();
	wm->debugBox->addField("BX", "?");
	wm->debugBox->addField("BY", "?");
}

void TerrainManager::requestCol(bool posShift)
{
	TropPackets::PackC2 packC2;
	packC2.set_horizontal(true);
	packC2.set_shiftpositive(posShift);
	boost::shared_ptr <OPacket> opC2(new OPacket("C2", true));
	opC2->setData(boost::make_shared <std::string>(packC2.SerializeAsString()));
	opC2->addSendToID(0);
	client->getTCPConnection()->send(opC2);
	shiftCount++;
}

void TerrainManager::requestRow(bool posShift)
{
	TropPackets::PackC2 packC2;
	packC2.set_horizontal(false);
	packC2.set_shiftpositive(posShift);
	boost::shared_ptr <OPacket> opC2(new OPacket("C2", true));
	opC2->setData(boost::make_shared <std::string>(packC2.SerializeAsString()));
	opC2->addSendToID(0);
	client->getTCPConnection()->send(opC2);
	shiftCount++;
}

void TerrainManager::reInitLight(int bcI, int bI)
{
	int adjustedBCI = bcI - begin;
	if (bcI < begin)
	{
		adjustedBCI = bcI + (BLOCK_LOAD_BW - begin);
	}
	if (adjustedBCI >= BLOCK_LIGHT_OFF_BW && adjustedBCI < BLOCK_LOAD_BW - BLOCK_LIGHT_OFF_BW)
	{
		blockCols.at(bcI)->reInitLight(bI);
	}
}

void TerrainManager::sendC0()
{
	TropPackets::PackC0 packC0;
	packC0.set_bx(bX);
	packC0.set_by(bY);
	boost::shared_ptr <OPacket> opC0(new OPacket("C0", true));
	opC0->setData(boost::make_shared <std::string>(packC0.SerializeAsString()));
	opC0->addSendToID(0);
	std::cout << "SENT opC0" << std::endl;
	client->getTCPConnection()->send(opC0);
}

void TerrainManager::draw()
{
	for (int j = 1; j <= blockCreator->getBlockTypeSize(); j++)
	{
		double x;
		if (wm->getPixelWorldX() < 0)
		{
			x = (int)(-wm->getPixelWorldX()) % BLOCK_WIDTH - BLOCK_WIDTH * (BLOCK_DRAW_OFF + BLOCK_LIGHT_OFF + 1);
		}
		else
		{
			x = (int)(-(abs((int)wm->getPixelWorldX()) % BLOCK_WIDTH)) - BLOCK_WIDTH * (BLOCK_DRAW_OFF + BLOCK_LIGHT_OFF);
		}
		al_hold_bitmap_drawing(true);
		int bIndex = (begin + BLOCK_LIGHT_OFF_BW) % BLOCK_LOAD_BW;
		for (int i = 0; i < BLOCK_DRAW_BW + BLOCK_LIGHT_OFF * 2; i++)
		{
			if (i > BLOCK_LIGHT_OFF && i <= BLOCK_DRAW_BW + BLOCK_LIGHT_OFF)
			{
				blockCols.at(bIndex)->draw(x, j);
			}
			else if (j == 1)
			{
				blockCols.at(bIndex)->updateLBC(x);
			}
			x += BLOCK_WIDTH;
			bIndex++;
			if (bIndex >= BLOCK_LOAD_BW)
			{
				bIndex = 0;
			}
		}
		al_hold_bitmap_drawing(false);
	}
	wm->debugBox->setField("BX", std::to_string(bX));
	wm->debugBox->setField("BY", std::to_string(bY));
}

Block* TerrainManager::getBlockAtPixels(double x, double y)
{
	std::pair <int, int> blockII = getIndexsAtPixels(x, y);
	if (blockII.first != -1 && blockII.second != -1)
	{
		return blockCols.at(blockII.first)->blocks.at(blockII.second);
	}
	return nullptr;
}

std::pair<int, int> TerrainManager::getIndexsAtPixels(double x, double y)
{
	int xOff;
	if (wm->getPixelWorldX() < 0)
	{
		xOff = (int)(-wm->getPixelWorldX()) % BLOCK_WIDTH - BLOCK_WIDTH * (BLOCK_LIGHT_OFF + BLOCK_DRAW_OFF + BLOCK_LIGHT_OFF_BW + 1);
	}
	else
	{
		xOff = (int)(-(abs((int)wm->getPixelWorldX()) % BLOCK_WIDTH) - BLOCK_WIDTH * (BLOCK_LIGHT_OFF + BLOCK_DRAW_OFF + BLOCK_LIGHT_OFF_BW));
	}
	int bcI = (x - xOff) / BLOCK_WIDTH;
	if (bcI >= 0 && bcI < BLOCK_LOAD_BW)
	{
		bcI += begin;
		bcI %= BLOCK_LOAD_BW;
		int bI = blockCols.at(bcI)->getIndexAtPixels(y);
		return std::pair <int, int>(bcI, bI);
	}
	return std::pair <int, int>(-1, -1);
}

std::pair<int, int> TerrainManager::getIndexAtBlockCords(int64_t searchBX, int64_t searchBY)
{
	int colI = getBlockColumnI(searchBX);
	if (colI != -1)
	{
		int rowI = blockCols.at(colI)->getBlockI(searchBY);
		if (rowI != -1)
		{
			return std::pair <int, int>(colI, rowI);
		}
	}
	return std::pair <int, int>(-1, -1);
}

void TerrainManager::replaceBlock(std::pair <int, int> blockII, uint16_t newType)
{
	if (blockII.first != -1 && blockII.second != -1)
	{
		Block* originalBlock = blockCols.at(blockII.first)->blocks.at(blockII.second);
		if (originalBlock == nullptr && newType != 0|| originalBlock != nullptr && originalBlock->getBlockID() != newType)
		{
			TropPackets::PackD0 packD0;
			int adjustedBCI = blockII.first - begin;
			if (blockII.first < begin)
			{
				adjustedBCI = blockII.first + (BLOCK_LOAD_BW - begin);
			}
			int blocksBegin = blockCols.at(blockII.first)->getBegin();
			int adjustedBI = blockII.second - blocksBegin;
			if (blockII.second < blocksBegin)
			{
				adjustedBI = blockII.second + (BLOCK_LOAD_BH - blocksBegin);
			}
			packD0.set_bx(bX + adjustedBCI);
			packD0.set_by(bY + adjustedBI);
			packD0.set_type(newType);
			boost::shared_ptr <OPacket> opD0(new OPacket("D0", true));
			opD0->addSendToID(0);
			opD0->setData(boost::make_shared <std::string>(packD0.SerializeAsString()));
			client->getTCPConnection()->send(opD0);
			Block* newBlock = blockCreator->getBlock(newType);
			blockCols.at(blockII.first)->blocks.at(blockII.second) = newBlock;
			if ((originalBlock == nullptr || originalBlock->isTransparent()) != (newBlock == nullptr || newBlock->isTransparent()))
			{
				if (newBlock != nullptr && !newBlock->isTransparent())
				{
					reInitLight(blockII.first, blockII.second);
				}
				int lowBCI = blockII.first - 1;
				int highBCI = blockII.first + 1;
				if (highBCI >= BLOCK_LOAD_BW)
				{
					highBCI -= BLOCK_LOAD_BW;
				}
				if (lowBCI < 0)
				{
					lowBCI += BLOCK_LOAD_BW;
				}
				int lowBI = blockII.second - 1;
				int highBI = blockII.second + 1;
				if (lowBI < 0)
				{
					lowBI += BLOCK_LOAD_BH;
				}
				if (highBI >= BLOCK_LOAD_BH)
				{
					highBI -= BLOCK_LOAD_BH;
				}
				reInitLight(lowBCI, blockII.second);
				reInitLight(highBCI, blockII.second);
				reInitLight(blockII.first, lowBI);
				reInitLight(blockII.first, highBI);
			}
			else if (newBlock != nullptr && !newBlock->isTransparent())
			{
				((ShadeBlock*)newBlock)->lbc = ((ShadeBlock*)originalBlock)->lbc;
				((ShadeBlock*)originalBlock)->lbc = nullptr;
			}
			delete originalBlock;
			originalBlock = nullptr;
		}
	}
	else
	{
		std::cerr << "replace block out of range" << std::endl;
	}
}

void TerrainManager::translate(float dX, float dY)
{
	double changeBX = (wm->getPixelWorldX() + dX) / BLOCK_WIDTH;
	double currentBX = (wm->getPixelWorldX()) / BLOCK_WIDTH;
	if (changeBX < 0)
	{
		changeBX--;
	}
	if (currentBX < 0)
	{
		currentBX--;
	}
	if ((int)changeBX - (int)currentBX != 0)
	{
		shiftBX(((int)changeBX - (int)currentBX));
	}
	double changeBY = (wm->getPixelWorldY() + dY) / BLOCK_WIDTH;
	double currentBY = (wm->getPixelWorldY()) / BLOCK_WIDTH;
	if (changeBY < 0)
	{
		changeBY--;
	}
	if (currentBY < 0)
	{
		currentBY--;
	}
	if ((int)changeBY - (int)currentBY != 0)
	{
		shiftBY((int)changeBY - (int)currentBY);
	}
}

TerrainManager::~TerrainManager()
{
}

Block * TerrainManager::getBlock(int searchBX, int searchBY)
{
	BlockColumn* blockCol = getBlockColumn(searchBX);
	if (blockCol != nullptr)
	{
		return blockCol->getBlock(searchBY);
	}
	return nullptr;
}

int TerrainManager::getBlockColumnI(int searchBX)
{
	int colI = searchBX - bX;
	colI += begin;
	if (colI >= BLOCK_LOAD_BW)
	{
		colI -= BLOCK_LOAD_BW;
	}
	if (colI < BLOCK_LOAD_BW && colI >= 0)
	{
		return colI;
	}
	return -1;
}

BlockColumn * TerrainManager::getBlockColumn(int searchBX)
{
	int colI = getBlockColumnI(searchBX);
	if (colI != -1)
	{
		return blockCols.at(colI);
	}
	return nullptr;
}

void TerrainManager::shiftBX(int count)
{
	shiftMutex.lock();
	for (int i = 0; i < abs(count); i++)
	{
		begin += count / abs(count);
		bX += count / abs(count);
		if (begin >= BLOCK_LOAD_BW)
		{
			begin = 0;
		}
		if (begin < 0)
		{
			begin = BLOCK_LOAD_BW - 1;
		}
		if (begin == 0)
		{
			end = BLOCK_LOAD_BW - 1;
		}
		else
		{
			end = begin - 1;
		}
		if (count < 0)
		{
			delete blockCols.at(begin);
			blockCols.at(begin) = new BlockColumn(wm, this, bX, bY);
			requestCol(false);
			int lightDestroyI = end - BLOCK_LIGHT_OFF_BW + 1;
			if (lightDestroyI < 0)
			{
				lightDestroyI += BLOCK_LOAD_BW;
			}
			blockCols.at(lightDestroyI)->destroyLight();
			int lbInit = begin + BLOCK_LIGHT_OFF_BW;
			if (lbInit >= BLOCK_LOAD_BW)
			{
				lbInit -= BLOCK_LOAD_BW;
			}
			blockCols.at(lbInit)->initLight();
		}
		else
		{
			delete blockCols.at(end);
			blockCols.at(end) = new BlockColumn(wm, this, bX + BLOCK_LOAD_BW - 1, bY);
			requestCol(true);
			int lightDestroyI = begin + BLOCK_LIGHT_OFF_BW - 1;
			if (lightDestroyI >= BLOCK_LOAD_BW)
			{
				lightDestroyI -= BLOCK_LOAD_BW;
			}
			blockCols.at(lightDestroyI)->destroyLight();
			int lbInit = end - BLOCK_LIGHT_OFF_BW;
			if (lbInit < 0)
			{
				lbInit += BLOCK_LOAD_BW;
			}
			blockCols.at(lbInit)->initLight();
		}
	}
	shiftMutex.unlock();
}

void TerrainManager::shiftBY(int count)
{
	shiftMutex.lock();
	bY += count;
	for (int i = 0; i < BLOCK_LOAD_BW; i++)
	{
		blockCols.at(i)->shiftBY(count);
	}
	shiftMutex.unlock();
	for (int i = 0; i < abs(count); i++)
	{
		if (count > 0)
		{
			requestRow(true);
		}
		else
		{
			requestRow(false);
		}
	}
}

void TerrainManager::keyC1(boost::shared_ptr<IPacket> iPack)
{
	std::cout << "C1 Recieved" << std::endl;
	TropPackets::PackC1 packC1;
	packC1.ParseFromString(*iPack->getData());
	for (int i = 0; i < packC1.blockarr_size(); i++)
	{
		int col = i / BLOCK_LOAD_BH;
		int row = i % BLOCK_LOAD_BH;
		if (row == 0)
		{
			blockCols.push_back(new BlockColumn(wm, this, bX + col, bY));	
			blockCols.at(blockCols.size() - 1)->setGroundY(packC1.groundys().Get(col));
		}
		if (packC1.blockarr().Get(i) != 0)
		{
			blockCols.at(col)->blocks.at(row) = blockCreator->getBlock(packC1.blockarr().Get(i));
		}
		if (row == BLOCK_LOAD_BH - 1 && col > BLOCK_LIGHT_OFF_BW && col <= BLOCK_LOAD_BW - BLOCK_LIGHT_OFF_BW)
		{
			blockCols.at(col - 1)->initLight();
		}
	}
	loaded = true;
}

void TerrainManager::keyC3(boost::shared_ptr<IPacket> iPack)
{
	c3ShiftCount++;
	shiftMutex.lock();
	TropPackets::PackC3 packC3;
	packC3.ParseFromString(*iPack->getData());
	int64_t bX = packC3.bx();
	int64_t bY = packC3.by();
	if (packC3.horizontal())
	{
		BlockColumn* blockCol = getBlockColumn(bX);
		if (blockCol != nullptr)
		{
			blockCol->setGroundY(packC3.groundy());
			int i = blockCol->getBY() - bY;
			int blockI = 0;
			if (i < 0)
			{
				blockI = -i;
				i = 0;
			}
			for (; blockI < BLOCK_LOAD_BH && i < BLOCK_LOAD_BH; blockI++, i++)
			{
				int adjustedBlockI = (blockI + blockCol->getBegin()) % BLOCK_LOAD_BH;
				if (packC3.blockarr().Get(i) != 0)
				{
					if (blockCol->blocks.at(adjustedBlockI) != nullptr)
					{
						delete blockCol->blocks.at(adjustedBlockI);
					}
					blockCol->blocks.at(adjustedBlockI) = blockCreator->getBlock(packC3.blockarr().Get(i));
				}
			}
		}
	}
	else if (bY >= this->bY && bY < this->bY + BLOCK_LOAD_BH)
	{
		int bI = bY - this->bY;
		if (bI >= 0 && bI < BLOCK_LOAD_BH)
		{
			int i = 0;
			int blockColI = bX - this->bX;
			if (blockColI < 0)
			{
				blockColI = 0;
				i = -blockColI;
			}
			for (; blockColI < BLOCK_LOAD_BW && i < BLOCK_LOAD_BW; blockColI++, i++)
			{
				if (packC3.blockarr().Get(i) != 0)
				{
					int adjustedBlockColI = (blockColI + begin) % BLOCK_LOAD_BW;
					int colBI = (bI + blockCols.at(adjustedBlockColI)->getBegin()) % BLOCK_LOAD_BH;
					if (blockCols.at(adjustedBlockColI)->blocks.at(colBI) != nullptr)
					{
						delete blockCols.at(adjustedBlockColI)->blocks.at(bI);
					}
					blockCols.at(adjustedBlockColI)->blocks.at(colBI) = blockCreator->getBlock(packC3.blockarr().Get(i));
				}
			}
		}
	}
	shiftMutex.unlock();
}

void TerrainManager::keyD0(boost::shared_ptr<IPacket> iPack)
{
	shiftMutex.lock();
	TropPackets::PackD0 packD0;
	packD0.ParseFromString(*iPack->getData());
	std::pair <int, int> blockII = getIndexAtBlockCords(packD0.bx(), packD0.by());
	if (blockII.first != -1 && blockII.second != -1)
	{
		replaceBlock(blockII, packD0.type());
	}
	else
	{
		std::cerr << "D0 cords out of range" << std::endl;
	}
	shiftMutex.unlock();
}

void TerrainManager::keyD1(boost::shared_ptr<IPacket> iPack)
{
	shiftMutex.lock();
	TropPackets::PackD1 packD1;
	packD1.ParseFromString(*iPack->getData());
	BlockColumn* blockCol = getBlockColumn(packD1.bx());
	if (blockCol != nullptr)
	{
		blockCol->setGroundY(packD1.groundy()); 
	}
	else
	{
		std::cerr << "D1 cords out of range" << std::endl;
	}
	shiftMutex.unlock();
}

