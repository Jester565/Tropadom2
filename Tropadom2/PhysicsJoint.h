#pragma once
#include <Box2D/Box2D.h>

class WorldManager;
class PhysicsBody;

class PhysicsJoint
{
public:
	PhysicsJoint(WorldManager* worldManager, PhysicsBody* body1, PhysicsBody* body2, float body1X, float body1Y, float body2X, float body2Y, bool inMeters = false, bool collide = false);
	~PhysicsJoint();

protected:
	WorldManager* worldManager;
	b2WeldJoint* joint;
	PhysicsBody* body1;
	PhysicsBody* body2;
};

