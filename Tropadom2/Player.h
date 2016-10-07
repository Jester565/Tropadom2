#pragma once
#include <PKeyOwner.h>
#include "PhysicsCircle.h"
#include "PhysicsSquare.h"

class WorldManager;
class Client;
class AniPart;

class Player : public PKeyOwner
{
public:
	static const float PLAYER_SCALE;
	static const double POS_SEND_TIME;
	static const float PLAYER_W;
	static const float PLAYER_H;
	static const int FOOT_CONTACT_ID = 1;
	static const int JUMP_MULT = 18;

	static PhysicsBody* CreatePlayerBody(WorldManager* wm, float x, float y);

	Player(WorldManager* worldManager, Client* client);

	void draw();

	float getTransX()
	{
		return transX;
	}

	float getTransY()
	{
		return transY;
	}

	float getPlayerX()
	{
		return playerBody->getX();
	}

	float getPlayerY()
	{
		return playerBody->getY();
	}

	~Player();

private:
	AniPart* torso;
	AniPart* topLeg;
	AniPart* topArm;
	AniPart* topForearm;
	AniPart* topHand;
	AniPart* head;
	PhysicsBody* playerBody;
	WorldManager* worldManager;
	Client* client;
	float transX;
	float transY;
	float lastX;
	float lastY;
	double posSendTimer;
};

