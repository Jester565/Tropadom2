#pragma once
#include <FPSLogger.h>
#include <SoundEffect.h>
#include <Music.h>

class PhysicsBody;
class BoxCore;
class PhysicsJoint;
class LightSource;

class GameTest
{
public:
	GameTest(BoxCore* boxCore);

	void draw();

	~GameTest();

protected:
	double vol;
	double pan;
	double idVol;
	BoxCore* boxCore;
	PhysicsBody* boxBody;
	PhysicsBody* boxBody2;
	PhysicsJoint* boxJoint;
	PhysicsBody* groundBody;
	LightSource* lightSource;
	AllegroExt::FPSLogger* fpsLogger;
	AllegroExt::Sound::SoundEffect* grassStep;
	AllegroExt::Sound::Music* monsterSong;
};

