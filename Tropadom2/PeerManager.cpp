#include "PeerManager.h"
#include "Packets/Trop.pb.h"
#include "Profile.h"
#include <Client.h>
#include <TCPConnection.h>
#include <ShapeRenderer.h>
#include <IPacket.h>
#include <OPacket.h>
#include <boost/thread.hpp>

PeerManager::PeerManager(Client* client, Profile* userProfile)
	:PKeyOwner(client->getPacketManager()), userProfile(userProfile), client(client)
{
	addKey(new PKey("B0", this, &PeerManager::keyB0));
	addKey(new PKey("B1", this, &PeerManager::keyB1));
	addKey(new PKey("B3", this, &PeerManager::keyB3));
	peersInfo.setMaxWidth(STANDARD_WIDTH / 2 - 20);
}

void PeerManager::draw()
{
	boost::shared_lock <boost::shared_mutex> lock(peersMutex);
	AllegroExt::Graphics::ShapeRenderer::drawRectangle(STANDARD_WIDTH / 2, 0, STANDARD_WIDTH / 2, STANDARD_HEIGHT / 2, 40, 40, 40, 200);
	std::string total = "Peers\n";
	for (auto it = peers.begin(); it != peers.end(); it++)
	{
		total += it->second->getValSummary();
		total += "\n\n";
	}
	peersInfo.drawText(total, STANDARD_WIDTH/2 + 10, 10, 30, 255, 255, 255, 255);
}

PeerManager::~PeerManager()
{
}

void PeerManager::keyB0(boost::shared_ptr<IPacket> iPack)
{
	TropPackets::PackB0 packB0;
	packB0.ParseFromString(*iPack->getData());
	boost::unique_lock <boost::shared_mutex> lock(peersMutex);
	peers.emplace(std::make_pair(iPack->getSenderID(), new Peer(iPack->getSenderID(), packB0.name())));
	TropPackets::PackB3 packB3;
	packB3.set_name(userProfile->getName());
	boost::shared_ptr <OPacket> opB3(new OPacket("B3", false));
	opB3->addSendToID(iPack->getSenderID());
	opB3->setData(boost::make_shared <std::string>(packB3.SerializeAsString()));
	client->getTCPConnection()->send(opB3);
}

void PeerManager::keyB1(boost::shared_ptr<IPacket> iPack)
{
	TropPackets::PackB1 packB1;
	packB1.ParseFromString(*iPack->getData());
	boost::unique_lock <boost::shared_mutex> lock(peersMutex);
	peers.erase(packB1.id());
}

void PeerManager::keyB3(boost::shared_ptr<IPacket> iPack)
{
	TropPackets::PackB3 packB3;
	packB3.ParseFromString(*iPack->getData());
	boost::unique_lock <boost::shared_mutex> lock(peersMutex);
	peers.emplace(std::make_pair(iPack->getSenderID(), new Peer(iPack->getSenderID(), packB3.name())));
}