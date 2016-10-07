#pragma once
#include <PKeyOwner.h>
#include "Peer.h"
#include <MLScreenText.h>
#include <boost/shared_ptr.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <map>

class Client;
class IPacket;
class Profile;
class WorldManager;

class PeerManager : public PKeyOwner
{
public:
	static const float PEER_VISIBLE_OFF_METERS;
	PeerManager(Client* client, Profile* userProfile, WorldManager* worldManager);

	void draw();

	void showPeers();

	Peer* getPeer(IDType id)
	{
		return peers.at(id);
	}

	std::map <IDType, Peer*>* getPeers()
	{
		return &peers;
	}

	~PeerManager();

protected:
	void keyB0(boost::shared_ptr<IPacket> iPack);
	void keyB1(boost::shared_ptr<IPacket> iPack);
	void keyB3(boost::shared_ptr<IPacket> iPack);
	void keyG0(boost::shared_ptr<IPacket> iPack);
	std::map <IDType, Peer*> peers;
	AllegroExt::Graphics::MLScreenText peersInfo;
	boost::shared_mutex peersMutex;
	Profile* userProfile;
	Client* client;
	WorldManager* worldManager;
};

