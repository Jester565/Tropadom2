#include "Profile.h"
#include "Trop.pb.h"
#include "WorldManager.h"
#include "Player.h"
#include <Macros.h>
#include <Client.h>
#include <TCPConnection.h>
#include <IPacket.h>
#include <OPacket.h>

Profile::Profile(Client* client, WorldManager* wm, const std::string& name)
	:PKeyOwner(client->getPacketManager()), client(client), wm(wm), id(0), name(name)
{
	addKey(new PKey("B2", this, &Profile::keyB2));
}


Profile::~Profile()
{
}

void Profile::keyB2(boost::shared_ptr<IPacket> iPack)
{
	TropPackets::PackB2 packB2;
	packB2.ParseFromString(*iPack->getData());
	this->id = packB2.id();
	TropPackets::PackB0 packB0;
	packB0.set_name(name);
	packB0.set_mx(0);
	packB0.set_my(0);
	boost::shared_ptr<OPacket> opB0(new OPacket("B0", false));
	opB0->setData(boost::make_shared<std::string>(packB0.SerializeAsString()));
	opB0->addSendToID(BROADCAST_ID);
	client->getTCPConnection()->send(opB0);
}
