#pragma once
#include "PhysicsBody.h"
#include <LightBlockerContainer.h>
#include <LightLayer.h>
#include "LightV4.h"

#ifdef USE_LIGHT_V4
using namespace lighting;
#endif

class PhysicsBodyLight : public PhysicsBody
{
public:
	PhysicsBodyLight(LightLayer* lightLayer, WorldManager* world, BodyMode bm, float x, float y, float degs = 0);

	void draw() override;

	~PhysicsBodyLight();

protected:
	LightBlockerContainer* lbc;
};

