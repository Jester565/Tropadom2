#include "BoxCore.h"
#include "WorldManager.h"
#include "InternetManager.h"
#include "MainMenu.h"
#include <ShapeRenderer.h>
#include <LightLayer.h>
#include <Box2D/Box2D.h>

BoxCore::BoxCore(const std::string& resourcePath)
	:Core(resourcePath), worldManager(nullptr)
{
}

bool BoxCore::init()
{
	if (Core::init())
	{
		lightLayer = new LightLayer();
		worldManager = new WorldManager();
		internetManager = new InternetManager(worldManager);
		mainMenu = new MainMenu(internetManager, worldManager);
		fpsLogger = new AllegroExt::FPSLogger();
		return true;
	}
	return false;
}

void BoxCore::draw()
{
	if (!worldManager->isLoaded())
	{
		mainMenu->draw();
	}
	else
	{
		worldManager->draw();
	}
	internetManager->drawOverlay();
	fpsLogger->draw(5, 10, 50);
}

BoxCore::~BoxCore()
{
	if (worldManager != nullptr)
	{
		delete worldManager;
		worldManager = nullptr;
	}
}
