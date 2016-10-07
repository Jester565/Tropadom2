#pragma once
#include <Macros.h>
#include <string>
#include <stdint.h>
#include <boost/thread/mutex.hpp>

class WorldManager;
class PhysicsBody;

class Peer
{
public:
	Peer(WorldManager* worldManager, IDType id, const std::string& name, float mX, float mY);

	void setName(const std::string& name)
	{
		this->name = name;
	}

	void draw();

	IDType getID() const
	{
		return id;
	}

	std::string getName() const
	{
		return name;
	}
	
	void setPos(bool visible, float mX, float mY, float mVX, float mVY);

	std::string getValSummary()
	{
		std::string summary = "ID: ";
		summary += std::to_string(id);
		summary += " ";
		summary += "Name: ";
		summary += name;
		return summary;
	}

	~Peer();

protected:
	float mX;
	float mY;
	float mVX;
	float mVY;
	bool visible;
	bool posUpdated;
	PhysicsBody* peerBody;
	std::string name;
	IDType id;
	WorldManager* worldManager;
	boost::mutex posMutex;
};

