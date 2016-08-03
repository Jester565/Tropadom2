#pragma once
#include "PhysicsBodyLight.h"
#include <Box2D/Box2D.h>

class LightLayer;

class PhysicsSquareLight : public PhysicsBodyLight
{
public:
	PhysicsSquareLight(LightLayer* lightLayer, WorldManager* world, BodyMode bm, float x, float y, float w, float h, float degs, uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = UINT8_MAX);
	
	PhysicsSquareLight(LightLayer* lightLayer, WorldManager* world, BodyMode bm, b2FixtureDef & boxFixDef, float x, float y, float w, float h, float degs, uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = UINT8_MAX);

	void init(b2FixtureDef & boxFixDef, float w, float h, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	~PhysicsSquareLight();
};

