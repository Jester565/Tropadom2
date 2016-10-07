#pragma once
#include <string>

class Client;
class Pinger;
class Profile;
class PeerManager;
class WorldManager;

class InternetManager
{
public:
	InternetManager(WorldManager* worldManager);

	void init(const std::string& ip, const std::string& port);

	void drawOverlay();

	Client* getClient()
	{
		return client;
	}

	PeerManager* getPeerManager()
	{
		return peerManager;
	}

	Profile* getUserProfile()
	{
		return userProfile;
	}

	~InternetManager();

protected:
	Client* client;
	Pinger* pinger;
	Profile* userProfile;
	PeerManager* peerManager;
	bool showPeers;
};

