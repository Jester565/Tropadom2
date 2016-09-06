#pragma once
#include <string>

class Client;
class Pinger;
class Profile;
class PeerManager;

class InternetManager
{
public:
	InternetManager();

	void init(const std::string& ip, const std::string& port);

	void draw();

	Client* getClient()
	{
		return client;
	}

	PeerManager* getPeerManager()
	{
		return peerManager;
	}

	~InternetManager();

protected:
	Client* client;
	Pinger* pinger;
	Profile* userProfile;
	PeerManager* peerManager;
	bool drawPeerManager;
};

