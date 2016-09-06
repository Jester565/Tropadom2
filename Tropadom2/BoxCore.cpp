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
		internetManager = new InternetManager();
		worldManager = new WorldManager();
		mainMenu = new MainMenu(internetManager, worldManager);
		if (worldManager->init())
		{
			fpsLogger = new AllegroExt::FPSLogger();
			return true;
		}
	}
	return false;
}

void BoxCore::draw()
{
	//mainMenu->draw();
	worldManager->draw();
	//internetManager->draw();
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
