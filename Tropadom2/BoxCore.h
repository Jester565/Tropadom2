#pragma once
#include <Core.h>
#include <FPSLogger.h>
#include <LightLayer.h>
#include "LightV4.h"

using namespace AllegroExt;

class WorldManager;
class MainMenu;
class InternetManager;

class BoxCore : public Core
{
public:
	BoxCore(const std::string& resourcePath);

	WorldManager* getWorldManager()
	{
		return worldManager;
	}

#ifdef USE_LIGHT_V4
	lighting::LightLayer* getLightLayer()
	{
		return lightLayer;
	}
#else
	LightLayer* getLightLayer()
	{
		return lightLayer;
	}
#endif

	~BoxCore();

protected:
	bool init() override;
	void draw() override;
#ifdef USE_LIGHT_V4
	lighting::LightLayer* lightLayer;
#else
	LightLayer* lightLayer;
#endif
	WorldManager* worldManager;
	InternetManager* internetManager;
	MainMenu* mainMenu;
	AllegroExt::FPSLogger* fpsLogger;
};

