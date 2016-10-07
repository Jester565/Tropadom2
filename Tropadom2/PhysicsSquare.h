#pragma once
#include "PhysicsBody.h"
#include <Box2D/Box2D.h>

class PhysicsSquare : public PhysicsBody
{
public:
	PhysicsSquare(WorldManager* world, BodyMode bm, float x, float y, float w, float h, float degs = 0);
	PhysicsSquare(WorldManager* world, BodyMode bm, b2FixtureDef& boxFixDef, float x, float y, float w, float h, float degs = 0);
	PhysicsSquare(WorldManager* world, BodyMode bm, float x, float y, float w, float h, float degs, uint8_t r, uint8_t g, uint8_t b, uint8_t a = UINT8_MAX);
	PhysicsSquare(WorldManager* world, BodyMode bm, b2FixtureDef& boxFixDef, float x, float y, float w, float h, float degs, uint8_t r, uint8_t g, uint8_t b, uint8_t a = UINT8_MAX);

	~PhysicsSquare();

protected:
	void initBmp(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = UINT8_MAX);
	void init(b2FixtureDef& boxFixDef, float w, float h);
};

