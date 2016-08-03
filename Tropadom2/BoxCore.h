#pragma once
#include <Core.h>
#include <FPSLogger.h>

using namespace AllegroExt;

class WorldManager;
class LightLayer;
class GameTest;

class BoxCore : public Core
{
public:
	BoxCore();

	WorldManager* getWorldManager()
	{
		return worldManager;
	}

	LightLayer* getLightLayer()
	{
		return lightLayer;
	}

	~BoxCore();

protected:
	bool init() override;
	void draw() override;
	LightLayer* lightLayer;
	WorldManager* worldManager;
	GameTest* gameTest;
	AllegroExt::FPSLogger* fpsLogger;
};

