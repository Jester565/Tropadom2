#include "Block.h"
#include "WorldManager.h"
#include "TerrainManager.h"
#include <LightLayer.h>
#include <ShapeRenderer.h>
#include <Box2D/Box2D.h>


Block::Block(WorldManager* wm, TerrainManager* tm)
	:wm(wm), tm(tm), texture(nullptr), bodySet(false)
{

}

Block::Block(const Block & original)
	:blockID(original.blockID), wm(original.wm), tm(original.tm), texture(nullptr), body(nullptr)
{
	
}

b2FixtureDef Block::getFixDef()
{
	b2FixtureDef fixDef;
	fixDef.density = 1;
	fixDef.restitution = 0;
	return fixDef;
}


void Block::draw(float pX, float pY)
{
	texture->draw(pX, pY);
}

void Block::initSurround(Block * n, Block * e, Block * s, Block * w, int64_t bX, int64_t bY)
{
	if (n == nullptr || e == nullptr || s == nullptr || w == nullptr);
	{
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(bX * (BLOCK_WIDTH / B2D_SCALE) - (BLOCK_WIDTH / 2) / B2D_SCALE, -bY * (BLOCK_WIDTH / B2D_SCALE) + (BLOCK_WIDTH / 2) / B2D_SCALE);
		body = wm->getWorld()->CreateBody(&bodyDef);
		b2FixtureDef boxFixDef = getFixDef();
		b2EdgeShape shape;
		if (n == nullptr)
		{
			shape.Set(b2Vec2((-BLOCK_WIDTH / B2D_SCALE) / 2, (BLOCK_WIDTH / B2D_SCALE) / 2), b2Vec2((BLOCK_WIDTH / B2D_SCALE) / 2, (BLOCK_WIDTH / B2D_SCALE) / 2));
			boxFixDef.shape = &shape;
			nFixture = body->CreateFixture(&boxFixDef);
		}
		if (e == nullptr)
		{
			shape.Set(b2Vec2((BLOCK_WIDTH / B2D_SCALE) / 2, (BLOCK_WIDTH / B2D_SCALE) / 2), b2Vec2((BLOCK_WIDTH / B2D_SCALE) / 2, (-BLOCK_WIDTH / B2D_SCALE) / 2));
			boxFixDef.shape = &shape;
			eFixture = body->CreateFixture(&boxFixDef);
		}
		if (s == nullptr)
		{
			shape.Set(b2Vec2((-BLOCK_WIDTH / B2D_SCALE) / 2, (-BLOCK_WIDTH / B2D_SCALE) / 2), b2Vec2((BLOCK_WIDTH / B2D_SCALE) / 2, (-BLOCK_WIDTH / B2D_SCALE) / 2));
			boxFixDef.shape = &shape;
			sFixture = body->CreateFixture(&boxFixDef);
		}
		if (w == nullptr)
		{
			shape.Set(b2Vec2((-BLOCK_WIDTH / B2D_SCALE) / 2, (BLOCK_WIDTH / B2D_SCALE) / 2), b2Vec2((-BLOCK_WIDTH / B2D_SCALE) / 2, (-BLOCK_WIDTH / B2D_SCALE) / 2));
			boxFixDef.shape = &shape;
			wFixture = body->CreateFixture(&boxFixDef);
		}
	}
	/*
	if (!bodySet)
	{
		
	}
	else if (body != nullptr)
	{
		if (n != nullptr && e != nullptr && s != nullptr && w != nullptr)
		{
			bodySet = false;
			wm->getWorld()->DestroyBody(body);
			nFixture = nullptr;
			eFixture = nullptr;
			sFixture = nullptr;
			wFixture = nullptr;
			body = nullptr;
		}
		else
		{
			if (nFixture != nullptr && n != nullptr)
			{
				body->DestroyFixture(nFixture);
				nFixture = nullptr;
			}
			if (eFixture != nullptr && e != nullptr)
			{
				body->DestroyFixture(eFixture);
				eFixture = nullptr;
			}
			if (sFixture != nullptr && s != nullptr)
			{
				body->DestroyFixture(sFixture);
				sFixture = nullptr;
			}
			if (wFixture != nullptr && w != nullptr)
			{
				body->DestroyFixture(wFixture);
				wFixture = nullptr;
			}
		}
	}
	*/
}

void Block::updateSurround(float pX, float pY)
{

}

void Block::destroySurround()
{
	if (body != nullptr)
	{
		bodySet = false;
		wm->getWorld()->DestroyBody(body);
		body = nullptr;
		nFixture = nullptr;
		eFixture = nullptr;
		sFixture = nullptr;
		wFixture = nullptr;
	}
}

Block::~Block()
{
	destroySurround();
}
