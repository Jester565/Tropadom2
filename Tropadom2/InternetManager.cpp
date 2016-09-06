#include "InternetManager.h"
#include "Pinger.h"
#include "PeerManager.h"
#include "Profile.h"
#include <Client.h>

InternetManager::InternetManager()
	:drawPeerManager(false)
{
	client = new Client();
	client->createManagers();
	pinger = new Pinger(this);
	userProfile = new Profile(client, "Doge");
	peerManager = new PeerManager(client, userProfile);
}

void InternetManager::init(const std::string& ip, const std::string& port)
{
	client->initClient(ip, port, false);
}

void InternetManager::draw()
{
	if (AllegroExt::Input::InputManager::keyTyped('p'))
	{
		drawPeerManager = !drawPeerManager;
	}
	if (drawPeerManager)
	{
		peerManager->draw();
	}
}

InternetManager::~InternetManager()
{
	delete pinger;
	pinger = nullptr;
	delete peerManager;
	peerManager = nullptr;
	delete userProfile;
	userProfile = nullptr;
	delete client;
	client = nullptr;
}
