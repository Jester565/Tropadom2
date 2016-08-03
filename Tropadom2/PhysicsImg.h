#pragma once
#include "PhysicsBody.h"
#include <Box2D/Box2D.h>
#include <vector>
#include <string>

class PhysicsImg : public PhysicsBody
{
public:
	PhysicsImg(WorldManager* worldManager, BodyMode bm, float x, float y, float degs = 0);

	void initPoly(const std::string& filePath, float w, float h);

	void initPoly(const std::string& filePath, float w, float h, b2FixtureDef& fixDef);
	
	void initSquare(const std::string& filePath, float w, float h);

	void initSquare(const std::string& filePath, float w, float h, b2FixtureDef& fixDef);

	void initCircle(const std::string& filePath, float radius);

	void initCircle(const std::string& filePath, float radius, b2FixtureDef& fixDef);

	void addVert(int imgX, int imgY);

	void setCenterCords(int imgX, int imgY);

	b2Vec2 centerMeters(float meterX, float meterY) override;

	~PhysicsImg();

protected:
	std::vector <b2Vec2> vertCords;
	b2Vec2 centerCords;
};

