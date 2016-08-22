#include "GameTest.h"
#include "PhysicsSquareLight.h"
#include "PhysicsCircle.h"
#include "PhysicsImgLight.h"
#include "PhysicsJoint.h"
#include "BoxCore.h"
#include "WorldManager.h"
#include <SoundManager.h>
#include <LightSource.h>
#include <ShapeRenderer.h>
#include <InputManager.h>
#include <Box2D/Box2D.h>
#include <iostream>

GameTest::GameTest(BoxCore* boxCore)
	:boxCore(boxCore), boxBody(nullptr), vol(1), pan(0), idVol(1)
{
	groundBody = new PhysicsSquareLight(boxCore->getLightLayer(), boxCore->getWorldManager(), BODY_KINEM, 5, 5, 7, 1, 0, 200, 0, 0);
	groundBody->getBody()->SetAngularVelocity(.2);
	fpsLogger = new AllegroExt::FPSLogger();
	lightSource = new LightSource(boxCore->getLightLayer(), 600);
	lightSource->setDebugMode(false);
	boxCore->getSoundManager()->addIDVol(1.0);
	monsterSong = new AllegroExt::Sound::Music(boxCore->getSoundManager(), "monster.ogg");
	grassStep = new AllegroExt::Sound::SoundEffect(boxCore->getSoundManager(), "grass1.wav", 0);

}

void GameTest::draw()
{
	if (AllegroExt::Input::InputManager::getClicked())
	{
		if (boxBody != nullptr)
		{
			delete boxBody;
			delete boxBody2;
		}
		//boxBody = new PhysicsImgLight(boxCore->getLightLayer(), boxCore->getWorldManager(), BODY_DYNAM, AllegroExt::Input::InputManager::mouseX / boxCore->getWorldManager()->getXScale(), 10);
		//((PhysicsImgLight*)boxBody)->initSquare("C:\\Users\\ajcra\\Desktop\\wdir\\trop2\\grass.png", 1, 1);
		//boxBody2 = new PhysicsSquare(boxCore->getWorldManager(), BODY_DYNAM, AllegroExt::Input::InputManager::mouseX / boxCore->getWorldManager()->getXScale() + 5, 10, 1, 1, 0, 0, 0, 200);
		boxBody = new PhysicsImgLight(boxCore->getLightLayer(), boxCore->getWorldManager(), BODY_DYNAM, AllegroExt::Input::InputManager::mouseX / boxCore->getWorldManager()->getXScale(), 10);
		//boxBody2 = new PhysicsCircle(boxCore->getWorldManager(), BODY_DYNAM, AllegroExt::Input::InputManager::mouseX / boxCore->getWorldManager()->getXScale(), 10, 1);
		((PhysicsImgLight*)boxBody)->addVert(0, 332);
		((PhysicsImgLight*)boxBody)->addVert(300, 0);
		((PhysicsImgLight*)boxBody)->addVert(796, 600);
		((PhysicsImgLight*)boxBody)->setCenterCords(400, 300);
		((PhysicsImgLight*)boxBody)->initPoly("C:\\Users\\ajcra\\Desktop\\wdir\\trop2\\triangle.png", 4, 4);
		//boxJoint = new PhysicsJoint(boxCore->getWorldManager(), boxBody, boxBody2, 400, 300, 50, 50);
	}
	lightSource->setXY(AllegroExt::Input::InputManager::mouseX, AllegroExt::Input::InputManager::mouseY);
	if (boxBody != nullptr)
	{
		boxBody->draw();
		//boxBody2->draw();
	}
	if (AllegroExt::Input::InputManager::keyPressed('o'))
	{
		grassStep->play(vol, pan);
	}
	if (AllegroExt::Input::InputManager::keyPressed('p'))
	{
		vol += .1;
		if (vol > 1)
		{
			vol = 1;
		}
		std::cout << "VOL: " << vol << std::endl;
	}
	if (AllegroExt::Input::InputManager::keyPressed('i'))
	{
		vol -= .1;
		if (vol < 0)
		{
			vol = 0;
		}
		std::cout << "VOL: " << vol << std::endl;
	}
	if (AllegroExt::Input::InputManager::keyPressed('l'))
	{
		pan += .1;
		if (pan > 1)
		{
			pan = 1;
		}
		std::cout << "PAN: " << pan << std::endl;
	}
	if (AllegroExt::Input::InputManager::keyPressed('k'))
	{
		pan -= .1;
		if (pan < -1)
		{
			pan = -1;
		}
	}
	if (AllegroExt::Input::InputManager::keyPressed('m'))
	{
		idVol += .1;
		if (idVol > 1)
		{
			idVol = 1;
		}
		boxCore->getSoundManager()->setIDVol(0, idVol);
		std::cout << "ID_VOL: " << idVol << std::endl;
	}
	if (AllegroExt::Input::InputManager::keyPressed('n'))
	{
		idVol -= .1;
		if (idVol < 0)
		{
			idVol = 0;
		}
		boxCore->getSoundManager()->setIDVol(0, idVol);
		std::cout << "ID_VOL: " << idVol << std::endl;
	}
	if (AllegroExt::Input::InputManager::keyPressed('u'))
	{
		monsterSong->play();
	}
	if (AllegroExt::Input::InputManager::keyPressed('m'))
	{
		monsterSong->stop();
	}
	groundBody->draw();
	fpsLogger->draw(5, 10, 50);
}

GameTest::~GameTest()
{
}
