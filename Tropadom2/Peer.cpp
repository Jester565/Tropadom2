#include "Peer.h"
#include "Player.h"
#include "WorldManager.h"
#include "PhysicsBody.h"
#include <ShapeRenderer.h>

Peer::Peer(WorldManager* worldManager, IDType id, const std::string& name, float mX, float mY)
	:worldManager(worldManager), id(id), name(name), mVX(0), mVY(0), mX(mX), mY(mY), visible(false)
{
	peerBody = Player::CreatePlayerBody(worldManager, mX, mY);
} 

void Peer::draw()
{
	posMutex.lock();
	if (visible)
	{
		if (posUpdated)
		{
			peerBody->getBody()->SetTransform(b2Vec2(mX, mY), 0);
			b2Vec2 vel = peerBody->getBody()->GetLinearVelocity();
			peerBody->getBody()->ApplyLinearImpulse(b2Vec2((mVX - vel.x) * peerBody->getBody()->GetMass(), (mVY - vel.y) * peerBody->getBody()->GetMass()), peerBody->getBody()->GetWorldCenter(), true);
			posUpdated = false;
		}
		peerBody->draw();
	}
	posMutex.unlock();
}

void Peer::setPos(bool visible, float mX, float mY, float mVX, float mVY)
{
	posMutex.lock();
	this->posUpdated = true;
	this->visible = visible;
	this->mX = mX;
	this->mY = mY;
	this->mVX = mVX;
	this->mVY = mVY;
	posMutex.unlock();
}

Peer::~Peer()
{
	
} 
