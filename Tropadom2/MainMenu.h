#pragma once
#include <ScreenText.h>
#include <TextField.h>
#include <Image.h>

class InternetManager;
class WorldManager;

class MainMenu
{
public:
	MainMenu(InternetManager* im, WorldManager* worldManager);

	void draw();

	~MainMenu();

private:
	static const int DC_WIDTH = STANDARD_WIDTH / 2;
	static const int DC_HEIGHT = STANDARD_HEIGHT / 2;
	static const int FONT_HEIGHT = DC_HEIGHT / 10;
	void drawDirectConnect(int x, int y);
	void drawConnectLoad();
	WorldManager* worldManager;
	InternetManager* internetManager;
	AllegroExt::Graphics::ScreenText dcTitle;
	AllegroExt::Graphics::ScreenText connectionStatus;
	AllegroExt::Input::TextField ipField;
	AllegroExt::Input::TextField portField;
	bool connecting;
	bool loading;
};

