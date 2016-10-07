#pragma once
#include <allegro5/bitmap.h>
#include <tuple>
#include <vector>

class WorldManager;
class b2Body;
class b2Vec2;
class PhysicsJoint;

const enum BodyMode { BODY_DYNAM, BODY_STAT, BODY_KINEM };

class PhysicsBody
{
public:
	PhysicsBody(WorldManager* world, BodyMode bm, float x, float y, float degs = 0);
	 
	virtual void draw();

	std::pair<float,float> getXY();
	float getX();
	float getY();
	void setBmp();
	float getW()
	{
		return w;
	}
	float getH()
	{
		return h;
	}

	void setW(float w)
	{
		this->w = w;
	}

	void setH(float h)
	{
		this->h = h;
	}

	void setFixedRotation(bool mode);

	float getDegs();
	b2Body* getBody()
	{
		return body;
	}

	void setDegV(float angularVelocity);

	virtual b2Vec2 pixelToMeters(int pixelX, int pixelY);
	virtual b2Vec2 centerMeters(float meterX, float meterY);

	void addJoint(PhysicsJoint* joint);
	void removeJoint(PhysicsJoint* joint);

	virtual ~PhysicsBody();

protected:
	WorldManager* worldManager;
	b2Body* body;
	ALLEGRO_BITMAP* bodyBitMap;
	float w;
	float h;
	float bitmapW;
	float bitmapH;
	std::vector <PhysicsJoint*> joints;
};

