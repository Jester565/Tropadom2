#pragma once
#include <Box2D/Box2D.h>
#include <map>
#include <iostream>

class WorldContactListener : public b2ContactListener
{
public:
	WorldContactListener();

	void BeginContact(b2Contact* contact) override;

	void EndContact(b2Contact* contact) override;

	void addIDCounter(int id)
	{
		if (idCounters.find(id) == idCounters.end())
		{
			std::cerr << "The idCounter " << id << " already exists" << std::endl;
		}
		idCounters.emplace(std::pair <int, int>(id, 0));
	}

	void removeIDCounter(int id)
	{
		idCounters.erase(id);
	}

	int getCount(int id)
	{
		auto idIt = idCounters.find(id);
		if (idIt != idCounters.end())
		{
			return idIt->second;
		}
	}

	~WorldContactListener();

private:
	std::map <int, int> idCounters;
};

