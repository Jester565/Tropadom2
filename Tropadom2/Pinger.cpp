#include "Pinger.h"
#include "InternetManager.h"
#include "Packets/Trop.pb.h"
#include <Client.h>
#include <OPacket.h>
#include <IPacket.h>
#include <TCPConnection.h>


Pinger::Pinger(InternetManager* im)
	:PKeyOwner(im->getClient()->getPacketManager()), im(im)
{
	addKey(new PKey("A0", this, &Pinger::keyA0));
}

void Pinger::draw()
{
}


Pinger::~Pinger()
{
}

void Pinger::keyA0(boost::shared_ptr<IPacket> iPack)
{
	boost::shared_ptr<OPacket> opA0(new OPacket("A0", 0, true));
	opA0->setData(iPack->getData());
	im->getClient()->getTCPConnection()->send(opA0);
}
