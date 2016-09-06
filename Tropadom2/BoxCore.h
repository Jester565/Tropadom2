#pragma once
#include <Core.h>
#include <FPSLogger.h>

using namespace AllegroExt;

class WorldManager;
class LightLayer;
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
	InternetManager* internetManager;
	MainMenu* mainMenu;
	AllegroExt::FPSLogger* fpsLogger;
};

