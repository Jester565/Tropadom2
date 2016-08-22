#include "PhysicsBodyLight.h"
#include "WorldManager.h"
#include <LightLayer.h>
#include <LightBlockerContainer.h>
#include <Core.h>
#include <Box2D/Box2D.h>

PhysicsBodyLight::PhysicsBodyLight(LightLayer * lightLayer, WorldManager * world, BodyMode bm, float x, float y, float degs)
	:PhysicsBody(world, bm, x, y, degs)
{
	lbc = new LightBlockerContainer(lightLayer);
}

void PhysicsBodyLight::draw()
{
	PhysicsBody::draw();
	b2Vec2 bodyPos = body->GetPosition();
	lbc->setRads(-body->GetAngle());
	lbc->setXY((bodyPos.x - w / 2) * worldManager->getXScale(), STANDARD_HEIGHT - (bodyPos.y + h / 2) * worldManager->getYScale());
}

PhysicsBodyLight::~PhysicsBodyLight()
{
	delete lbc;
	lbc = nullptr;
}
