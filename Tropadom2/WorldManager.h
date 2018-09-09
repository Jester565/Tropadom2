#pragma once
#include <allegro5/transformations.h>
#include <LightLayer.h>
#include <CircleLightSource.h>
#include "LightV4.h"
#ifdef USE_LIGHT_V4
#include <GaussianBlurrer.h>
#include <GaussianKernelData.h>
#endif

class b2World;
class TerrainManager;
class DebugBox;
class InternetManager;
class ChatBox;
class Player;
class WorldDebugDraw;
class WorldContactListener;

#ifdef USE_LIGHT_V4
using namespace lighting;
#endif

static const float B2D_SCALE = 19;

class WorldManager
{
public:
	static const int SPEED = 5;
	static const float DEFAULT_GRAV_Y;
	static const float DEFAULT_GRAV_X;
	static const float METER_W;
	static const float METER_H;
	static const bool LIGHTING_ENABLED = true;

	WorldManager();

	void setGravity(float xGrav, float yGrav);

	bool init(InternetManager* im);

	void draw();

	void setDebugDraw(bool mode);

	b2World* getWorld()
	{
		return world;
	}

	float getPixelWorldX()
	{
		return worldX;
	}

	float getPixelWorldY()
	{
		return worldY;
	}
	
	float getMeterWorldX();

	float getMeterWorldY();

#ifdef USE_LIGHT_V4
	lighting::LightLayer* getLightLayer()
	{
		return lightLayer;
	}
	
	GaussianBlurrer* gausBlurrer;
	GaussianBlurrer* gausBlurrer2;

#else
	LightLayer* getLightLayer()
	{
		return lightLayer;
	}
#endif

	void setZoom(float zoom);

	float getZoom()
	{
		return zoom;
	}

	bool isLoaded();

	DebugBox* debugBox;

	WorldContactListener* getContactListener()
	{
		return worldContactListener;
	}

	TerrainManager* getTerrainManager()
	{
		return terrainManager;
	}

	Player* getPlayer()
	{
		return player;
	}

	~WorldManager();

protected:
	TerrainManager* terrainManager;
	WorldDebugDraw* worldDebugDraw;
	WorldContactListener* worldContactListener;
	InternetManager* internetManager;

	LightLayer* lightLayer;
	LightSource* lightSource;
	LightSource* sun;
	ChatBox* chatBox;
	b2World* world;
	Player* player;
	float xGrav;
	float yGrav;
	double worldX;
	double worldY;
	float zoom;
	bool debugDraw;
	ALLEGRO_TRANSFORM normalTrans;
	ALLEGRO_TRANSFORM zoomTrans;
};

