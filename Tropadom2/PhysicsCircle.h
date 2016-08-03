#pragma once
#include "PhysicsBody.h"
#include <Box2D/Box2D.h>

class PhysicsCircle : public PhysicsBody
{
public:
	PhysicsCircle(WorldManager* worldManager, BodyMode bm, float x, float y, float radius, float degs = 0, int r = 0, int g = 0, int b = 0, int a = UINT8_MAX);
	PhysicsCircle(WorldManager* worldManager, BodyMode bm, b2FixtureDef& fixDef, float x, float y, float radius, float degs = 0, int r = 0, int g = 0, int b = 0, int a = UINT8_MAX);
	
	~PhysicsCircle();

protected:
	void init(b2FixtureDef& fixDef, float radius, int r = 0, int g = 0, int b = 0, int a = UINT8_MAX);
};

