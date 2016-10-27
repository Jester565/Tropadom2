#pragma once
#include "PhysicsBodyLight.h"
#include <Box2D/Box2D.h>
#include "LightV4.h"

class PhysicsImgLight : public PhysicsBodyLight
{
public:
	PhysicsImgLight(LightLayer* lightLayer, WorldManager* worldManager, BodyMode bm, float x, float y, float degs = 0);

	void initPoly(const std::string& filePath, float w, float h);

	void initPoly(const std::string& filePath, float w, float h, b2FixtureDef& fixDef);

	void initSquare(const std::string& filePath, float w, float h);

	void initSquare(const std::string& filePath, float w, float h, b2FixtureDef& fixDef);

	void initCircle(const std::string& filePath, float radius);

	void initCircle(const std::string& filePath, float radius, b2FixtureDef& fixDef);

	void addVert(int imgX, int imgY);

	void setCenterCords(int imgX, int imgY);

	b2Vec2 centerMeters(float meterX, float meterY) override;

	~PhysicsImgLight();

protected:
	float toLightX(float x);
	float toLightY(float y);
	std::vector <b2Vec2> vertCords;
	b2Vec2 centerCords;
};

