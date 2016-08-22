#pragma once

class b2World;
class TerrainManager;
class LightLayer;
class LightSource;
class DebugBox;

class WorldManager
{
public:
	static const int SPEED = 5;
	static const float DEFAULT_GRAV_Y;
	static const float DEFAULT_GRAV_X;
	static const float DEFAULT_SCALE_X;
	static const float DEFAULT_SCALE_Y;
	static const bool LIGHTING_ENABLED = true;

	WorldManager();

	void setGravity(float xGrav, float yGrav);

	bool init();

	void draw();

	b2World* getWorld()
	{
		return world;
	}

	float getXScale()
	{
		return xScale;
	}

	float getYScale()
	{
		return yScale;
	}

	float getPixelWorldX()
	{
		return worldX;
	}

	float getPixelWorldY()
	{
		return worldY;
	}

	float getMeterWorldX()
	{
		return xScale * worldX;
	}

	float getMeterWorldY()
	{
		return yScale * worldY; 
	}

	LightLayer* getLightLayer()
	{
		return lightLayer;
	}

	DebugBox* debugBox;

	~WorldManager();

protected:
	TerrainManager* terrainManager;
	LightLayer* lightLayer;
	LightSource* lightSource;
	LightSource* lightSource2;
	b2World* world;
	float xGrav;
	float yGrav;
	float xScale;
	float yScale;
	float worldX;
	float worldY;
};

