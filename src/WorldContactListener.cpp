#include "WorldContactListener.h"

WorldContactListener::WorldContactListener()
	:b2ContactListener()
{

}

void WorldContactListener::BeginContact(b2Contact * contact)
{
	void* userFixtureData = contact->GetFixtureA()->GetUserData();
	if ((int)userFixtureData != 0)
	{
		auto idIt = idCounters.find((int)userFixtureData);
		if (idIt != idCounters.end())
		{
			idIt->second++;
		}
	}
	userFixtureData = contact->GetFixtureB()->GetUserData();
	if ((int)userFixtureData != 0)
	{
		auto idIt = idCounters.find((int)userFixtureData);
		if (idIt != idCounters.end())
		{
			idIt->second++;
		}
	}
}

void WorldContactListener::EndContact(b2Contact * contact)
{
	void* userFixtureData = contact->GetFixtureA()->GetUserData();
	if ((int)userFixtureData != 0)
	{
		auto idIt = idCounters.find((int)userFixtureData);
		if (idIt != idCounters.end())
		{
			idIt->second--;
		}
	}
	userFixtureData = contact->GetFixtureB()->GetUserData();
	if ((int)userFixtureData != 0)
	{
		auto idIt = idCounters.find((int)userFixtureData);
		if (idIt != idCounters.end())
		{
			idIt->second--;
		}
	}
}

WorldContactListener::~WorldContactListener()
{
}
