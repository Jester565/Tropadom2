#include "PhysicsImgLight.h"
#include "WorldManager.h"
#include <LightBlockerContainer.h>
#include <allegro5/allegro.h>
#include <iostream>	//temp include

PhysicsImgLight::PhysicsImgLight(LightLayer* lightLayer, WorldManager* worldManager, BodyMode bm, float x, float y, float degs)
	:PhysicsBodyLight(lightLayer, worldManager, bm, x, y, degs)
{
}

void PhysicsImgLight::initPoly(const std::string & filePath, float w, float h)
{
	b2FixtureDef fixDef;
	fixDef.density = 1;
	initPoly(filePath, w, h, fixDef);
}

void PhysicsImgLight::initPoly(const std::string & filePath, float w, float h, b2FixtureDef & fixDef)
{
	bodyBitMap = al_load_bitmap(filePath.c_str());
	if (bodyBitMap != nullptr)
	{
		this->w = w;
		this->h = h;
		bitmapW = al_get_bitmap_width(bodyBitMap);
		bitmapH = al_get_bitmap_height(bodyBitMap);
		lbc->setCXY(toLightX(centerCords.x), toLightY(centerCords.y));
		float lX0;
		float lY0;
		if (vertCords.size() > 0)
		{
			lX0 = toLightX(vertCords.at(0).x);
			lY0 = toLightY(vertCords.at(0).y);
		}
		for (int i = 0; i < vertCords.size(); i++)
		{
			if (i == vertCords.size() - 1)
			{
				float x1 = toLightX(vertCords.at(i).x);
				float y1 = toLightY(vertCords.at(i).y);
				lbc->addLine(x1, y1, lX0, lY0);
			}
			else
			{
				float x1 = toLightX(vertCords.at(i).x);
				float y1 = toLightY(vertCords.at(i).y);
				float x2 = toLightX(vertCords.at(i + 1).x);
				float y2 = toLightY(vertCords.at(i + 1).y);
				lbc->addLine(x1, y1, x2, y2);
			}
			vertCords.at(i).x = (vertCords.at(i).x - centerCords.x) * ((float)w / bitmapW);
			vertCords.at(i).y = (vertCords.at(i).y - centerCords.y) * ((float)h / bitmapH);
		}
		b2PolygonShape polyShape;
		polyShape.Set(vertCords.data(), vertCords.size());
		fixDef.shape = &polyShape;
		body->CreateFixture(&fixDef);
	}
}

void PhysicsImgLight::initSquare(const std::string& filePath, float w, float h)
{
	b2FixtureDef fixDef;
	fixDef.density = 1;
	initSquare(filePath, w, h, fixDef);
}

void PhysicsImgLight::initSquare(const std::string& filePath, float w, float h, b2FixtureDef& fixDef)
{
	bodyBitMap = al_load_bitmap(filePath.c_str());
	if (bodyBitMap != nullptr)
	{
		bitmapW = al_get_bitmap_width(bodyBitMap);
		bitmapH = al_get_bitmap_height(bodyBitMap);
		this->w = w;
		this->h = h;
		lbc->initSquare(w * B2D_SCALE, h * B2D_SCALE);
		lbc->setRotateLXYToCenter();
		b2PolygonShape boxShape;
		boxShape.SetAsBox(w / 2.0f, h / 2.0f);
		fixDef.shape = &boxShape;
		body->CreateFixture(&fixDef);
	}
}

void PhysicsImgLight::addVert(int imgX, int imgY)
{
	b2Vec2 vertPos;
	vertPos.x = imgX;
	vertPos.y = imgY;
	vertCords.push_back(vertPos);
}

void PhysicsImgLight::setCenterCords(int imgX, int imgY)
{
	centerCords.x = imgX;
	centerCords.y = imgY;
}

b2Vec2 PhysicsImgLight::centerMeters(float meterX, float meterY)
{
	b2Vec2 meterCenteredCords;
	meterCenteredCords.x = meterX - centerCords.x * (w / bitmapW);
	meterCenteredCords.y = meterY - centerCords.y * (h / bitmapH);
	return meterCenteredCords;
}

PhysicsImgLight::~PhysicsImgLight()
{
}

float PhysicsImgLight::toLightX(float x)
{
	std::cout << "IX: " << x << " ";
	float result = (x/bitmapW) * (B2D_SCALE * w);
	std::cout << "X: " << result << "  ";
	return result;
}

float PhysicsImgLight::toLightY(float y)
{
	std::cout << "IY: " << y << " ";
	float result = ((bitmapH - y) / bitmapH) * (B2D_SCALE * h);
	std::cout << "Y: " << result << std::endl;
	return result;
}
