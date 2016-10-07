#include "PhysicsBody.h"
#include "WorldManager.h"
#include "PhysicsJoint.h"
#include <ShapeRenderer.h>
#include <Box2D/Box2D.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <InputManager.h>
#include <iostream>

PhysicsBody::PhysicsBody(WorldManager* worldManager, BodyMode bm, float x, float y, float degs)
	:worldManager(worldManager), body(nullptr)
{
	b2BodyDef boxDef;
	switch (bm)
	{
	case BODY_DYNAM:
		boxDef.type = b2_dynamicBody;
		break;
	case BODY_STAT:
		boxDef.type = b2_staticBody;
		break;
	case BODY_KINEM:
		boxDef.type = b2_kinematicBody;
	}
	boxDef.position.Set(x, y);
	boxDef.angle = degs * (M_PI / 180);
	body = worldManager->getWorld()->CreateBody(&boxDef);
}

std::pair<float,float> PhysicsBody::getXY()
{
	b2Vec2 pos = body->GetPosition();
	return std::pair<float, float> (pos.x, pos.y);
}

float PhysicsBody::getX()
{
	b2Vec2 pos = body->GetPosition();
	return pos.x;
}

float PhysicsBody::getY()
{
	b2Vec2 pos = body->GetPosition();
	return pos.y;
}

void PhysicsBody::setBmp()
{
	this->bitmapW = w * B2D_SCALE;
	this->bitmapH = h * B2D_SCALE;
	bodyBitMap = al_create_bitmap(w * B2D_SCALE, h * B2D_SCALE);
	ALLEGRO_BITMAP* previousBitmap = al_get_target_bitmap();
	al_set_target_bitmap(bodyBitMap);
	al_clear_to_color(al_map_rgba(255, 0, 0, 255));
	al_set_target_bitmap(previousBitmap);
}

void PhysicsBody::setFixedRotation(bool mode)
{
	{
		body->SetFixedRotation(true);
	}
}

float PhysicsBody::getDegs()
{
	return (180.0/M_PI) * body->GetAngle();
}

void PhysicsBody::setDegV(float av)
{
	body->SetAngularVelocity(av * (M_PI / 180.0));
}

b2Vec2 PhysicsBody::pixelToMeters(int pixelX, int pixelY)
{
	b2Vec2 meterCords;
	meterCords.x = pixelX * (w / bitmapW);
	meterCords.y = pixelY * (h / bitmapH);
	return centerMeters(meterCords.x, meterCords.y);
}

b2Vec2 PhysicsBody::centerMeters(float meterX, float meterY)
{
	b2Vec2 meterCenteredCords;
	meterCenteredCords.x = meterX - w / 2.0;
	meterCenteredCords.y = meterY - h / 2.0;
	return meterCenteredCords;
}

void PhysicsBody::addJoint(PhysicsJoint* joint)
{
	joints.push_back(joint);
}

void PhysicsBody::removeJoint(PhysicsJoint * joint)
{
	for (int i = 0; i < joints.size(); i++)
	{
		if (joint == joints.at(i))
		{
			joints.erase(joints.begin() + i);
			return;
		}
	}
}

void PhysicsBody::draw()
{
	if (bodyBitMap != nullptr)
	{
		b2Vec2 bodyPos = body->GetPosition();
		int y = -(bodyPos.y - worldManager->getMeterWorldY()) * B2D_SCALE;
		al_draw_scaled_rotated_bitmap(bodyBitMap, (bitmapW / 2.0), (bitmapH / 2.0), (bodyPos.x - worldManager->getMeterWorldX()) * B2D_SCALE, y, (w * B2D_SCALE) / bitmapW, (h * B2D_SCALE) / bitmapH, -body->GetAngle(), NULL);
	}
}

PhysicsBody::~PhysicsBody()
{
	for (int i = 0; i < joints.size(); i++)
	{
		delete joints.at(i);
	}
	joints.clear();
	worldManager->getWorld()->DestroyBody(body);
	if (bodyBitMap != nullptr)
	{
		al_destroy_bitmap(bodyBitMap);
		bodyBitMap = nullptr;
	}
}
