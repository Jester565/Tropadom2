#include "BoxCore.h"
#include "WorldManager.h"
#include <LightLayer.h>
#include <Box2D/Box2D.h>

BoxCore::BoxCore()
	:worldManager(nullptr)
{
}

bool BoxCore::init()
{
	if (Core::init())
	{
		lightLayer = new LightLayer();
		worldManager = new WorldManager();
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
	//gameTest->draw();
	worldManager->draw();
	fpsLogger->draw(5, 10, 50);
	//lightLayer->draw();
}

BoxCore::~BoxCore()
{
	if (worldManager != nullptr)
	{
		delete worldManager;
		worldManager = nullptr;
	}
}
