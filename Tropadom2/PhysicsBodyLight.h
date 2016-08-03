#pragma once
#include "PhysicsBody.h"

class LightLayer;
class LightBlockerContainer;

class PhysicsBodyLight : public PhysicsBody
{
public:
	PhysicsBodyLight(LightLayer* lightLayer, WorldManager* world, BodyMode bm, float x, float y, float degs = 0);

	void draw() override;

	~PhysicsBodyLight();

protected:
	LightBlockerContainer* lbc;
};

