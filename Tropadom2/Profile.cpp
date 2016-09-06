#include "Profile.h"
#include "Packets/Trop.pb.h"
#include <Macros.h>
#include <Client.h>
#include <TCPConnection.h>
#include <IPacket.h>
#include <OPacket.h>

Profile::Profile(Client* client, const std::string& name)
	:PKeyOwner(client->getPacketManager()), client(client), id(0), name(name)
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
	boost::shared_ptr<OPacket> opB0(new OPacket("B0", false));
	opB0->setData(boost::make_shared<std::string>(packB0.SerializeAsString()));
	opB0->addSendToID(BROADCAST_ID);
	client->getTCPConnection()->send(opB0);
}
