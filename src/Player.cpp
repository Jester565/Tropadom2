#include "Player.h"
#include "AniPart.h"
#include "WorldManager.h"
#include "WorldContactListener.h"
#include "PhysicsJoint.h"
#include "TerrainManager.h"
#include "Trop.pb.h"
#include <UDPConnection.h>
#include <Client.h>
#include <OPacket.h>
#include <Core.h>
#include <Box2D/Box2D.h>
#include <DisplayManager.h>
#include <InputManager.h>
#include <iostream>

const float Player::PLAYER_SCALE = .9;
const float Player::PLAYER_W = 1.2;
const float Player::PLAYER_H = 4.6;
const double Player::POS_SEND_TIME = 3;

PhysicsBody * Player::CreatePlayerBody(WorldManager * wm, float x, float y)
{
	PhysicsBody* playerBody = new PhysicsBody(wm, BODY_DYNAM, x, y, 0);
	b2FixtureDef playerFixDef;
	b2PolygonShape boxShape;
	boxShape.SetAsBox(PLAYER_W / 2.0, PLAYER_H / 2.0);
	playerFixDef.shape = &boxShape;
	playerFixDef.density = 1020;
	playerFixDef.restitution = 0;
	playerFixDef.filter.groupIndex = -1;
	playerBody->setFixedRotation(true);
	playerBody->getBody()->CreateFixture(&playerFixDef);
	playerBody->setW(PLAYER_W);
	playerBody->setH(PLAYER_H);
	playerBody->setBmp();
	playerFixDef.density = 1;
	playerFixDef.friction = 0;
	boxShape.SetAsBox(PLAYER_W / 20.0, PLAYER_H / 2.0, b2Vec2(-PLAYER_W / 2, 0), 0);
	playerBody->getBody()->CreateFixture(&playerFixDef);
	boxShape.SetAsBox(PLAYER_W / 20.0, PLAYER_H / 2.0, b2Vec2(PLAYER_W / 2, 0), 0);
	playerBody->getBody()->CreateFixture(&playerFixDef);
	return playerBody;
}

Player::Player(WorldManager* worldManager, Client* client)
	:PKeyOwner(client->getPacketManager()), worldManager(worldManager), client(client), posSendTimer(0)
{
	worldManager->getContactListener()->addIDCounter(FOOT_CONTACT_ID);
	lastX = worldManager->getMeterWorldX() + (STANDARD_WIDTH / 2.0) / B2D_SCALE;
	lastY = worldManager->getMeterWorldY() - (STANDARD_HEIGHT / 2.0) / B2D_SCALE;
	playerBody = CreatePlayerBody(worldManager, lastX, lastY);
	b2FixtureDef sensorFixDef;
	b2PolygonShape sensorShape;
	sensorFixDef.density = 1;
	sensorFixDef.filter.groupIndex = -1;
	sensorFixDef.isSensor = true;
	sensorShape.SetAsBox(PLAYER_W / 2.0, PLAYER_H / 20.0, b2Vec2(0, -PLAYER_H/2), 0);
	sensorFixDef.shape = &sensorShape;
	b2Fixture* groundSensor = playerBody->getBody()->CreateFixture(&sensorFixDef);
	groundSensor->SetUserData((void*)FOOT_CONTACT_ID);
	torso = new AniPart("chestReal.png", 22 * PLAYER_SCALE, 40 * PLAYER_SCALE, 11 * PLAYER_SCALE, 40 * PLAYER_SCALE);
	topLeg = new AniPart("legFill.png", 20 * PLAYER_SCALE, 44 * PLAYER_SCALE, 11 * PLAYER_SCALE, 3 * PLAYER_SCALE);
	topArm = new AniPart("realArm.png", 14 * PLAYER_SCALE, 25 * PLAYER_SCALE, 5 * PLAYER_SCALE, 3 * PLAYER_SCALE);
	topForearm = new AniPart("forArmReal.png", 8 * PLAYER_SCALE, 17 * PLAYER_SCALE, 4 * PLAYER_SCALE, 1 * PLAYER_SCALE);
	topHand = new AniPart("realHand.png", 6 * PLAYER_SCALE, 10 * PLAYER_SCALE, 3 * PLAYER_SCALE, 1 * PLAYER_SCALE);
	head = new AniPart("realHelm.png", 22 * PLAYER_SCALE, 22 * PLAYER_SCALE, 12 * PLAYER_SCALE, 22 * PLAYER_SCALE);
	torso->createMoveJoint(topLeg, 13 * PLAYER_SCALE, 40 * PLAYER_SCALE);
	torso->createMoveJoint(topArm, 9 * PLAYER_SCALE, 5 * PLAYER_SCALE);
	torso->createMoveJoint(head, 9 * PLAYER_SCALE, 1 * PLAYER_SCALE);
	topArm->createMoveJoint(topForearm, 6 * PLAYER_SCALE, 23.5 * PLAYER_SCALE);
	topForearm->createMoveJoint(topHand, 4 * PLAYER_SCALE, 13 * PLAYER_SCALE);
	torso->setXY(STANDARD_WIDTH / 2, STANDARD_HEIGHT / 2 + 5 / PLAYER_SCALE + 1);
}

void Player::draw()
{
	topLeg->draw();
	head->draw();
	torso->draw();
	topHand->draw();
	topForearm->draw();
	topArm->draw();
	if (AllegroExt::Input::InputManager::keyPressed('e'))
	{
		torso->changeDegs(10000000 * AllegroExt::Core::rate);
		head->changeDegs(3 * AllegroExt::Core::rate);
		topLeg->changeDegs(9 * AllegroExt::Core::rate);
		topHand->changeDegs(18 * AllegroExt::Core::rate);
		topForearm->changeDegs(24 * AllegroExt::Core::rate);
		topArm->changeDegs(36 * AllegroExt::Core::rate);
	}
	if (AllegroExt::Input::InputManager::keyPressed('q'))
	{
		torso->changeDegs(-1 * AllegroExt::Core::rate);
	}
	transX = playerBody->getX() - lastX;
	transY = playerBody->getY() - lastY;
	lastX = playerBody->getX();
	lastY = playerBody->getY();
	b2Vec2 vel = playerBody->getBody()->GetLinearVelocity();
	float desiredXV = 0;
	if (AllegroExt::Input::InputManager::keyPressed('a') != AllegroExt::Input::InputManager::keyPressed('d'))
	{
		if (AllegroExt::Input::InputManager::keyPressed('a'))
		{
			desiredXV = -7.5;
		}
		if (AllegroExt::Input::InputManager::keyPressed('d'))
		{
			desiredXV = 7.5;
		}
	}
	float deltaXV = desiredXV - vel.x;
	float xImpulse = playerBody->getBody()->GetMass() * deltaXV;
	float yImpulse = 0;
	if (AllegroExt::Input::InputManager::keyPressed('w'))
	{
		playerBody->getBody()->SetAwake(true);
		if (worldManager->getContactListener()->getCount(FOOT_CONTACT_ID) > 0 && vel.y == 0)
		{
			yImpulse = playerBody->getBody()->GetMass() * JUMP_MULT;
		}
	}
	if (xImpulse != 0 || yImpulse != 0)
	{
		playerBody->getBody()->ApplyLinearImpulse(b2Vec2(xImpulse, yImpulse), playerBody->getBody()->GetWorldCenter(), true);
	}
	if (posSendTimer > POS_SEND_TIME)
	{
		posSendTimer = 0;
		TropPackets::PackG0 packG0;
		packG0.set_mx(playerBody->getX());
		packG0.set_my(playerBody->getY());
		vel = playerBody->getBody()->GetLinearVelocity();
		packG0.set_mvx(vel.x);
		packG0.set_mvy(vel.y);
		boost::shared_ptr <OPacket> oPack(new OPacket("G0", true));
		oPack->addSendToID(BROADCAST_ID);
		oPack->setData(boost::make_shared <std::string>(packG0.SerializeAsString()));
		client->getUDPConnection()->send(oPack);
	}
	posSendTimer += AllegroExt::Core::rate;
}

Player::~Player()
{
	delete playerBody;
	playerBody = nullptr;
	worldManager->getContactListener()->removeIDCounter(FOOT_CONTACT_ID);
}
