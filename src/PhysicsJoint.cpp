#include "PhysicsJoint.h"
#include "WorldManager.h"
#include "PhysicsBody.h"
#include <iostream>	//temp include

PhysicsJoint::PhysicsJoint(WorldManager* worldManager, PhysicsBody* body1, PhysicsBody* body2, float body1X, float body1Y, float body2X, float body2Y, bool inMeters, bool collide)
	:worldManager(worldManager), body1(body1), body2(body2)
{
	b2RevoluteJointDef jointDef;
	jointDef.bodyA = body1->getBody();
	jointDef.bodyB = body2->getBody();
	b2Vec2 bodyAMeterCords;
	b2Vec2 bodyBMeterCords;
	if (!inMeters)
	{
		bodyAMeterCords = body1->pixelToMeters(body1X, body1Y);
		bodyBMeterCords = body2->pixelToMeters(body2X, body2Y);
	}
	else
	{
		bodyAMeterCords = body1->centerMeters(body1X, body1Y);
		bodyBMeterCords = body2->centerMeters(body2X, body2Y);
	}
	jointDef.localAnchorA.Set(bodyAMeterCords.x, bodyAMeterCords.y);
	jointDef.localAnchorB.Set(bodyBMeterCords.x, bodyBMeterCords.y);
	jointDef.collideConnected = collide;
	joint = (b2WeldJoint*)worldManager->getWorld()->CreateJoint(&jointDef);
	body1->addJoint(this);
	body2->addJoint(this);
}

PhysicsJoint::~PhysicsJoint()
{
	worldManager->getWorld()->DestroyJoint(joint);
	body1->removeJoint(this);
	body2->removeJoint(this);
}
