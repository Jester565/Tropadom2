#include "WorldManager.h"
#include "TerrainManager.h"
#include "TerrainConstants.h"
#include "InternetManager.h"
#include "DebugBox.h"
#include "ChatBox.h"
#include "Player.h"
#include "WorldDebugDraw.h"
#include "WorldContactListener.h"
#include "PeerManager.h"
#include <AboveLightSource.h>
#include <DisplayManager.h>
#include <Core.h>
#include <Box2D/Box2D.h>

const float WorldManager::DEFAULT_GRAV_Y = -40;
const float WorldManager::DEFAULT_GRAV_X = 0;
const float WorldManager::METER_W = STANDARD_WIDTH / B2D_SCALE;
const float WorldManager::METER_H = STANDARD_HEIGHT / B2D_SCALE;

#ifdef USE_LIGHT_V4
using namespace lighting;
#endif

WorldManager::WorldManager()
	:xGrav(DEFAULT_GRAV_X), yGrav(DEFAULT_GRAV_Y), world(nullptr), zoom(1), debugDraw(false), worldDebugDraw(nullptr), terrainManager(nullptr), player(nullptr)
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

bool WorldManager::init(InternetManager* im)
{
	this->internetManager = im;
	b2Vec2 gravVec(xGrav, yGrav);
	b2MixRestitution(0, 0);
	world = new b2World(gravVec);
	worldContactListener = new WorldContactListener();
	world->SetContactListener(worldContactListener);
	al_identity_transform(&normalTrans);
	if (LIGHTING_ENABLED)
	{
#ifdef USE_LIGHT_V4
		lightLayer = new LightLayer(STANDARD_WIDTH, STANDARD_HEIGHT, .25);
		lightSource = new CircleLightSource(lightLayer, 500, 200, 200, 200);
		sun = new AboveLightSource(lightLayer, 30);
		GaussianKernelData kernData(36, 9);
		gausBlurrer = new GaussianBlurrer(lightLayer, kernData, "VertShader.hlsl", "XFragShader.hlsl", "YFragShader.hlsl");
		gausBlurrer2 = new GaussianBlurrer(lightLayer, kernData, "VertShader.hlsl", "XFragShader.hlsl", "YFragShader.hlsl");
#else
		lightLayer = new LightLayer(.25);
		lightSource = new CircleLightSource(lightLayer, 500, 200, 200, 200);
#endif
		debugBox = new DebugBox();
		debugBox->addField("# of LightSource", "?");
		debugBox->addField("# of LightBlockers", "?");
		debugBox->addField("# of AboveLightBlockers", "?");
		debugBox->addField("# of LightRunnables", "?");
	}
	terrainManager = new TerrainManager(this, im->getClient());
	chatBox = new ChatBox(im, 400, 200);
	return true;
}

void WorldManager::draw()
{
	if (AllegroExt::Input::InputManager::keyTyped('k'))
	{
		std::cout << "TOTAL POINTS PROCESSED: " << CircleLightSource::ShadePointsProcessed << std::endl;
		std::cout << "TOTAL CAST POINTS PROCESSED: " << CircleLightSource::CastPointsProcessed << std::endl;
		std::cout << "CAST POINTS / POINTS: " << ((float)CircleLightSource::CastPointsProcessed) / CircleLightSource::ShadePointsProcessed << std::endl;
	}
	if (zoom != 1)
	{
		al_use_transform(&zoomTrans);
	}
	if (AllegroExt::Input::InputManager::keyTyped('y'))
	{
		setDebugDraw(!debugDraw);
	}
	if (AllegroExt::Input::InputManager::keyPressed('u'))
	{
		((CircleLightSource*)lightSource)->setXY(AllegroExt::Input::InputManager::mouseX, AllegroExt::Input::InputManager::mouseY);
	}
	terrainManager->draw();
	if (player != nullptr)
	{
		player->draw();
		internetManager->getPeerManager()->draw();
		terrainManager->translate((player->getTransX() * B2D_SCALE), -(player->getTransY() * B2D_SCALE));
		worldX += player->getTransX() * B2D_SCALE;
		worldY -= player->getTransY() * B2D_SCALE;
	}
	else
	{
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
	}
	if (LIGHTING_ENABLED)
	{
#ifdef USE_LIGHT_V4
		lightLayer->detach();
#else
		lightLayer->dispatch();
#endif
		lightLayer->draw();
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
	if (AllegroExt::Input::InputManager::getClicked() > 0)
	{
		std::pair <int, int> blockII = terrainManager->getIndexsAtPixels(AllegroExt::Input::InputManager::mouseX, AllegroExt::Input::InputManager::mouseY);
		if (blockII.first != -1 && blockII.second != -1)
		{
			terrainManager->replaceBlock(blockII, 1);
		}
	}
	if (zoom != 1)
	{
		al_use_transform(&normalTrans);
	}
	if (AllegroExt::Input::InputManager::keyTyped('i'))
	{
		if (player != nullptr)
		{
			delete player;
		}
		player = new Player(this, internetManager->getClient());
	}
	if (AllegroExt::Input::InputManager::keyTyped('o'))
	{
		if (player != nullptr)
		{
			delete player;
			player = nullptr;
		}
	}
	if (debugDraw)
	{
		world->DrawDebugData();
	}
	world->Step(AllegroExt::Core::rate / 60, 6, 2);
	world->ClearForces();
	/*
	debugBox->setField("# of LightSource", std::to_string(lightLayer->getLightSourceSize()));
	debugBox->setField("# of LightBlockers", std::to_string(lightLayer->getLightBlockersSize()));
	debugBox->setField("# of AboveLightBlockers", std::to_string(lightLayer->getAboveBlockerSize()));
	debugBox->setField("# of LightRunnables", std::to_string(lightLayer->getLightRunnablesSize()));
	*/
	debugBox->draw(0, 100);
	chatBox->draw(STANDARD_WIDTH - chatBox->getW(), STANDARD_HEIGHT - chatBox->getH());
}

void WorldManager::setDebugDraw(bool mode)
{
	this->debugDraw = mode;
	if (mode == true && worldDebugDraw == nullptr)
	{
		worldDebugDraw = new WorldDebugDraw(this);
		worldDebugDraw->SetFlags(DebugDraw::e_shapeBit);
		world->SetDebugDraw(worldDebugDraw);
	}
}

float WorldManager::getMeterWorldX()
{
	return (worldX + BLOCK_WORLD_OFF_PIXELS_W + (BLOCK_DRAW_OFF - 1) * BLOCK_WIDTH) / B2D_SCALE;
}

float WorldManager::getMeterWorldY()
{
	return (-worldY - INITIAL_BY * BLOCK_WIDTH - BLOCK_WORLD_OFF_PIXELS_H - (BLOCK_DRAW_OFF - 1) * BLOCK_WIDTH) / B2D_SCALE;
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

bool WorldManager::isLoaded()
{
	return (terrainManager != nullptr && terrainManager->isLoaded());
}

WorldManager::~WorldManager()
{
	delete terrainManager;
	terrainManager = nullptr;
	//delete lightSource;
	//lightSource = nullptr;
	delete world;
	world = nullptr;
	delete lightLayer;
	lightLayer = nullptr;
	std::cout << "REE" << std::endl;
}
