#pragma once
#include <ScreenText.h>
#include <vector>

class DebugBox
{
public:
	static const int FONT_SIZE = 20;
	DebugBox();

	void draw(int x, int y);
	void addField(const std::string& name, const std::string& val)
	{
		info.emplace_back(name, val);
		screenTexts.push_back(new AllegroExt::Graphics::ScreenText());
	}
	void removeField(const std::string& name, const std::string& val)
	{
		for (int i = 0; i < info.size(); i++)
		{
			if (info.at(i).first == name)
			{
				info.erase(info.begin() + i);
				delete screenTexts.at(i);
				screenTexts.erase(screenTexts.begin() + i);
				break;
			}
		}
	}
	void setField(const std::string& name, const std::string& val)
	{
		for (int i = 0; i < info.size(); i++)
		{
			if (info.at(i).first == name)
			{
				info.at(i).second = val;
			}
		}
	}

	~DebugBox();

private:
	std::vector<std::pair<std::string, std::string>> info;
	std::vector<AllegroExt::Graphics::ScreenText*> screenTexts;
};

