#include "PhysicsImg.h"
#include "WorldManager.h"
#include <allegro5/allegro.h>
#include <Image.h>
#include <iostream>

PhysicsImg::PhysicsImg(WorldManager* worldManager, BodyMode bm, float x, float y, float degs)
	:PhysicsBody(worldManager, bm, x, y, degs)
{
}

void PhysicsImg::initPoly(const std::string & filePath, float w, float h)
{
	b2FixtureDef fixDef;
	fixDef.density = 1;
	initPoly(filePath, w, h, fixDef);
}

void PhysicsImg::initPoly(const std::string & filePath, float w, float h, b2FixtureDef & fixDef)
{
	bodyBitMap = al_load_bitmap(filePath.c_str());
	if (bodyBitMap != nullptr)
	{
		this->w = w;
		this->h = h;
		bitmapW = al_get_bitmap_width(bodyBitMap);
		bitmapH = al_get_bitmap_height(bodyBitMap);
		for (int i = 0; i < vertCords.size(); i++)
		{
			vertCords.at(i).x = (vertCords.at(i).x - centerCords.x) * ((float)w/bitmapW);
			vertCords.at(i).y = (vertCords.at(i).y - centerCords.y) * ((float)h/bitmapH);
		}
		b2PolygonShape polyShape;
		polyShape.Set(vertCords.data(), vertCords.size());
		fixDef.shape = &polyShape;
		body->CreateFixture(&fixDef);
	}
}

void PhysicsImg::initSquare(const std::string& filePath, float w, float h)
{
	b2FixtureDef fixDef;
	fixDef.density = 1;
	initSquare(filePath, w, h, fixDef);
}

void PhysicsImg::initSquare(const std::string& filePath, float w, float h, b2FixtureDef& fixDef)
{
	bodyBitMap = al_load_bitmap(filePath.c_str());
	if (bodyBitMap != nullptr)
	{
		bitmapW = al_get_bitmap_width(bodyBitMap);
		bitmapH = al_get_bitmap_height(bodyBitMap);
		this->w = w;
		this->h = h;
		b2PolygonShape boxShape;
		boxShape.SetAsBox(w / 2.0f, h / 2.0f);
		fixDef.shape = &boxShape;
		body->CreateFixture(&fixDef);
	}
}

void PhysicsImg::initCircle(const std::string& filePath, float radius)
{
	b2FixtureDef fixDef;
	fixDef.density = 1;
	initCircle(filePath, radius, fixDef);
}

void PhysicsImg::initCircle(const std::string & filePath, float radius, b2FixtureDef & fixDef)
{
	bodyBitMap = al_load_bitmap(filePath.c_str());
	if (bodyBitMap != nullptr)
	{
		this->w = radius * 2;
		this->h = radius * 2;
		bitmapW = al_get_bitmap_width(bodyBitMap);
		bitmapH = al_get_bitmap_height(bodyBitMap);
		b2CircleShape circleShape;
		circleShape.m_p.SetZero();
		circleShape.m_radius = radius;
		fixDef.shape = &circleShape;
		body->CreateFixture(&fixDef);
	}
}

void PhysicsImg::addVert(int imgX, int imgY)
{
	b2Vec2 vertPos;
	vertPos.x = imgX;
	vertPos.y = imgY;
	vertCords.push_back(vertPos);
}

void PhysicsImg::setCenterCords(int imgX, int imgY)
{
	centerCords.x = imgX;
	centerCords.y = imgY;
}

b2Vec2 PhysicsImg::centerMeters(float meterX, float meterY)
{
	b2Vec2 meterCenteredCords;
	meterCenteredCords.x = meterX - centerCords.x * (w/bitmapW);
	meterCenteredCords.y = meterY - centerCords.y * (h/bitmapH);
	return meterCenteredCords;
}

PhysicsImg::~PhysicsImg()
{
}
