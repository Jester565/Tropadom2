#include "InternetManager.h"
#include "Pinger.h"
#include "PeerManager.h"
#include "Profile.h"
#include <Client.h>

InternetManager::InternetManager(WorldManager* worldManager)
	:showPeers(false)
{
	client = new Client();
	client->createManagers();
	pinger = new Pinger(this);
	userProfile = new Profile(client, worldManager, "Doge");
	peerManager = new PeerManager(client, userProfile, worldManager);
}

void InternetManager::init(const std::string& ip, const std::string& port)
{
	client->initClient(ip, port, false);
}

void InternetManager::drawOverlay()
{
	if (AllegroExt::Input::InputManager::keyTyped('p'))
	{
		showPeers = !showPeers;
	}
	if (showPeers)
	{
		peerManager->showPeers();
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
