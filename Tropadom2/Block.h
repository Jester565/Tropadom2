#pragma once
#include <Image.h>
#include <Box2D/Dynamics/b2Fixture.h>

class WorldManager;
class LightBlockerContainer;
class TerrainManager;
class b2Body;

class Block
{
	friend class BlockCreator;
public:
	virtual void draw(float pX, float pY);

	virtual void initSurround(Block* n, Block* e, Block* s, Block* w, int64_t bX, int64_t bY);

	virtual void updateSurround(float pX, float pY);

	virtual void destroySurround();

	virtual bool isTransparent()
	{
		return true;
	}

	uint16_t getBlockID()
	{
		return blockID;
	}

	virtual ~Block();

protected:
	Block(WorldManager* wm, TerrainManager* tm);
	Block(const Block& original);

	b2Body* body;
	bool bodySet;
	virtual Block* copy() = 0;
	virtual b2FixtureDef getFixDef();
	uint16_t blockID;
	WorldManager* wm;
	TerrainManager* tm;
	b2Fixture* nFixture;
	b2Fixture* eFixture;
	b2Fixture* sFixture;
	b2Fixture* wFixture;
	AllegroExt::Graphics::Image* texture;
};

