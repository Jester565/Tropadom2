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
	:xGrav(DEFAULT_GRAV_X), yGrav(DEFAULT_GRAV_Y), xScale(DEFAULT_SCALE_X), yScale(DEFAULT_SCALE_Y), world(nullptr), zoom(1)
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
	al_identity_transform(&normalTrans);
	if (LIGHTING_ENABLED)
	{
		lightLayer = new LightLayer(.25);
		//lightLayer = new LightLayer(1);
		//lightSource = new DirectionalLightSource(lightLayer, 1000, 60, 0, 255, 0);
		lightSource2 = new CircleLightSource(lightLayer, 500, 200, 200, 200);
		new AboveLightSource(lightLayer);
		debugBox = new DebugBox();
		debugBox->addField("# of LightSource", "?");
		debugBox->addField("# of LightBlockers", "?");
		debugBox->addField("# of AboveLightBlockers", "?");
		debugBox->addField("# of LightRunnables", "?");
	}
	terrainManager = new TerrainManager(this);
	return true;
}

void WorldManager::draw()
{
	if (zoom != 1)
	{
		al_use_transform(&zoomTrans);
	}
	if (AllegroExt::Input::InputManager::keyPressed('u'))
	{
		((CircleLightSource*)lightSource2)->setXY(AllegroExt::Input::InputManager::mouseX, AllegroExt::Input::InputManager::mouseY);
	}
	/*
	((DirectionalLightSource*)lightSource)->setXY(AllegroExt::Input::InputManager::mouseX, AllegroExt::Input::InputManager::mouseY);
	if (AllegroExt::Input::InputManager::keyPressed('r'))
	{
		((DirectionalLightSource*)lightSource)->changeDegs(2 * AllegroExt::Core::rate);
	}
	if (AllegroExt::Input::InputManager::keyPressed('f'))
	{
		((DirectionalLightSource*)lightSource)->changeDegs(-2 * AllegroExt::Core::rate);
	}
	*/
	world->Step(AllegroExt::Core::rate / 60, 6, 2);
	world->ClearForces();
	terrainManager->draw();
	if (LIGHTING_ENABLED)
	{
		lightLayer->dispatch();
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
	if (AllegroExt::Input::InputManager::keyPressed('z'))
	{
		setZoom(getZoom() - .01 * AllegroExt::Core::rate);
	}
	if (AllegroExt::Input::InputManager::keyPressed('x'))
	{
		setZoom(getZoom() + .01 * AllegroExt::Core::rate);
	}
	if (AllegroExt::Input::InputManager::keyPressed('c'))
	{
		setZoom(1);
	}
	if (zoom != 1)
	{
		al_use_transform(&normalTrans);
	}
	debugBox->setField("# of LightSource", std::to_string(lightLayer->getLightSourceSize()));
	debugBox->setField("# of LightBlockers", std::to_string(lightLayer->getLightBlockersSize()));
	debugBox->setField("# of AboveLightBlockers", std::to_string(lightLayer->getAboveBlockerSize()));
	//debugBox->setField("# of LightRunnables", std::to_string(lightLayer->getLightRunnablesSize()));
	debugBox->draw(0, 100);
}

void WorldManager::setZoom(float zoom)
{
	if (this->zoom != zoom)
	{
		this->zoom = zoom;
		float cordOff = (zoom - 1.0) / 2.0;
		if (zoom != 1)
		{
			al_build_transform(&zoomTrans, -STANDARD_WIDTH * cordOff, -STANDARD_HEIGHT * cordOff, zoom, zoom, 0);
		}
		else
		{
			al_use_transform(&normalTrans);
		}
	}
}

WorldManager::~WorldManager()
{
	delete terrainManager;
	terrainManager = nullptr;
	delete lightSource;
	lightSource = nullptr;
	delete lightSource2;
	lightSource2 = nullptr;
	delete world;
	world = nullptr;
	delete lightLayer;
	lightLayer = nullptr;
}
