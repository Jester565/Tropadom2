#pragma once
#include <FPSLogger.h>
#include <SoundEffect.h>
#include <Music.h>
#include <CircleLightSource.h>

class PhysicsBody;
class BoxCore;
class PhysicsJoint;

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
	lighting::CircleLightSource* lightSource;
	AllegroExt::FPSLogger* fpsLogger;
	AllegroExt::Sound::SoundEffect* grassStep;
	AllegroExt::Sound::Music* monsterSong;
};

