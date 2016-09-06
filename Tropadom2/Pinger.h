#pragma once
#include <PKeyOwner.h>
#include <boost/shared_ptr.hpp>

class InternetManager;
class TCPConnection;
class IPacket;

class Pinger : public PKeyOwner
{
public:
	Pinger(InternetManager* im);

	void draw();

	~Pinger();

private:
	void keyA0(boost::shared_ptr<IPacket> iPack);

	InternetManager* im;
};

