#pragma once
#include <PKeyOwner.h>
#include <Macros.h>

class Client;
class WorldManager;

class Profile : public PKeyOwner
{
public:
	Profile(Client* client, WorldManager* wm, const std::string& name);

	std::string getName()
	{
		return name;
	}

	~Profile();

protected:
	void keyB2(boost::shared_ptr<IPacket> iPack);

	WorldManager* wm;
	Client* client;
	IDType id;
	std::string name;
};

