#include "WorldManager.h"
#include "TerrainManager.h"
#include <Core.h>
#include <Box2D/Box2D.h>

const float WorldManager::DEFAULT_GRAV_Y = -9.8;
const float WorldManager::DEFAULT_GRAV_X = 0;
const float WorldManager::DEFAULT_SCALE_X = 100;
const float WorldManager::DEFAULT_SCALE_Y = 100;

WorldManager::WorldManager()
	:xGrav(DEFAULT_GRAV_X), yGrav(DEFAULT_GRAV_Y), xScale(DEFAULT_SCALE_X), yScale(DEFAULT_SCALE_Y), world(nullptr)
{
	terrainManager = new TerrainManager(this);
}

void WorldManager::setGravity(float xGrav, float yGrav)
{
	this->xGrav = xGrav;
	this->yGrav = yGrav;
	if (world != nullptr)
	{
		b2Vec2 gravVec(xGrav, yGrav);
		world->SetGravity(gravVec);
	}
}

bool WorldManager::init()
{
	b2Vec2 gravVec(xGrav, yGrav);
	world = new b2World(gravVec);
	return true;
}

void WorldManager::draw()
{
	world->Step(AllegroExt::Core::rate / 60, 6, 2);
	world->ClearForces();
	terrainManager->draw();
	if (AllegroExt::Input::InputManager::keyPressed('d'))
	{
		terrainManager->translate(10, 0);
		worldX += 10;
	}
	if (AllegroExt::Input::InputManager::keyPressed('a'))
	{
		terrainManager->translate(-10 * AllegroExt::Core::rate, 0);
		worldX += -10 * AllegroExt::Core::rate;
	}
	if (AllegroExt::Input::InputManager::keyPressed('w'))
	{
		terrainManager->translate(0, -40 * AllegroExt::Core::rate, 0);
	}
}

WorldManager::~WorldManager()
{
	delete world;
	world = nullptr;
}
