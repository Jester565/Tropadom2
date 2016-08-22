#include "WorldManager.h"
#include "TerrainManager.h"
#include "DebugBox.h"
#include <LightLayer.h>
#include <CircleLightSource.h>
#include <AboveLightSource.h>
#include <DirectionalLightSource.h>
#include <Core.h>
#include <Box2D/Box2D.h>

const float WorldManager::DEFAULT_GRAV_Y = -9.8;
const float WorldManager::DEFAULT_GRAV_X = 0;
const float WorldManager::DEFAULT_SCALE_X = 100;
const float WorldManager::DEFAULT_SCALE_Y = 100;

WorldManager::WorldManager()
	:xGrav(DEFAULT_GRAV_X), yGrav(DEFAULT_GRAV_Y), xScale(DEFAULT_SCALE_X), yScale(DEFAULT_SCALE_Y), world(nullptr)
{

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
	if (LIGHTING_ENABLED)
	{
		lightLayer = new LightLayer(.25);
		lightSource = new DirectionalLightSource(lightLayer, 1000, 60, 0, 255, 0);
		lightSource2 = new CircleLightSource(lightLayer, 500, 255, 0, 0);
		((CircleLightSource*)lightSource2)->setXY(1000, 500);
		new AboveLightSource(lightLayer);
		debugBox = new DebugBox();
		debugBox->addField("# of LightSource", "?");
		debugBox->addField("# of LightBlockers", "?");
		debugBox->addField("# of AboveLightBlockers", "?");
		//lightSource2 = new CircleLightSource(lightLayer, 800);
		//((CircleLightSource*)lightSource)->setXY(1000, 500);
		//((CircleLightSource*)lightSource2)->setXY(0, 0);
	}
	terrainManager = new TerrainManager(this);
	return true;
}

void WorldManager::draw()
{
	((DirectionalLightSource*)lightSource)->setXY(AllegroExt::Input::InputManager::mouseX, AllegroExt::Input::InputManager::mouseY);
	if (AllegroExt::Input::InputManager::keyPressed('r'))
	{
		((DirectionalLightSource*)lightSource)->changeDegs(2 * AllegroExt::Core::rate);
	}
	if (AllegroExt::Input::InputManager::keyPressed('f'))
	{
		((DirectionalLightSource*)lightSource)->changeDegs(-2 * AllegroExt::Core::rate);
	}
	
	world->Step(AllegroExt::Core::rate / 60, 6, 2);
	world->ClearForces();
	terrainManager->draw();
	if (LIGHTING_ENABLED)
	{
		lightLayer->draw();
	}
	if (AllegroExt::Input::InputManager::keyPressed('d'))
	{
		terrainManager->translate(SPEED * AllegroExt::Core::rate, 0);
		worldX += SPEED * AllegroExt::Core::rate;
	}
	if (AllegroExt::Input::InputManager::keyPressed('a'))
	{
		terrainManager->translate(-SPEED * AllegroExt::Core::rate, 0);
		worldX += -SPEED * AllegroExt::Core::rate;
	}
	if (AllegroExt::Input::InputManager::keyPressed('w'))
	{
		terrainManager->translate(0, -SPEED * AllegroExt::Core::rate);
		worldY += -SPEED * AllegroExt::Core::rate;
	}
	if (AllegroExt::Input::InputManager::keyPressed('s'))
	{ 
		terrainManager->translate(0, SPEED * AllegroExt::Core::rate);
		worldY += SPEED * AllegroExt::Core::rate;
	}
	debugBox->setField("# of LightSource", std::to_string(lightLayer->getLightSourceSize()));
	debugBox->setField("# of LightBlockers", std::to_string(lightLayer->blockerCount));
	debugBox->setField("# of AboveLightBlockers", std::to_string(lightLayer->getAboveBlockerSize()));
	debugBox->draw(0, 100);
}

WorldManager::~WorldManager()
{
	delete world;
	world = nullptr;
}
