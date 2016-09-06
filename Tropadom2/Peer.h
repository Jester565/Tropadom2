#pragma once
#include <Macros.h>
#include <string>
#include <stdint.h>

class Peer
{
public:
	Peer(IDType id, const std::string& name = "NO NAME");

	void setName(const std::string& name)
	{
		this->name = name;
	}

	IDType getID() const
	{
		return id;
	}

	std::string getName() const
	{
		return name;
	}

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
	std::string name;
	IDType id;
};

