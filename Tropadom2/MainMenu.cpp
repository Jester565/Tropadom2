#include "MainMenu.h"
#include "InternetManager.h"
#include "WorldManager.h"
#include <Client.h>
#include <ShapeRenderer.h>
#include <Button.h>

MainMenu::MainMenu(InternetManager* im, WorldManager* wm)
	:internetManager(im), worldManager(wm), connecting(false), worldReady(false)
{
	ipField.set((7 * DC_WIDTH) / 8, DC_HEIGHT / 6);
	ipField.setText("IP");
	portField.set(DC_WIDTH / 2, DC_HEIGHT / 6);
	portField.setText("Port");
}


void MainMenu::draw()
{
	if (!connecting)
	{
		drawDirectConnect(STANDARD_WIDTH / 4, STANDARD_HEIGHT / 4);
	}
	else
	{
		drawConnectLoad();
	}
}

MainMenu::~MainMenu()
{
}

void MainMenu::drawDirectConnect(int x, int y)
{
	AllegroExt::Graphics::ShapeRenderer::drawRectangle(x, y, DC_WIDTH, DC_HEIGHT, 100, 100, 100, 200);
	dcTitle.drawCenteredText("Direct Connect", x + DC_WIDTH / 2, y +  DC_HEIGHT / 8, FONT_HEIGHT, 0, 0, 0, 255);
	ipField.draw(x + DC_WIDTH/16, y + DC_HEIGHT/3);
	portField.draw(x + DC_WIDTH / 4, y + (1.7 * DC_HEIGHT) / 3);
	if (AllegroExt::Input::Button::clickButton(x + DC_WIDTH / 4, y + .8 * DC_HEIGHT, DC_WIDTH / 2, DC_HEIGHT * .17, 0, 255, 0, 255))
	{
		internetManager->init(ipField.getText(), portField.getText());
		ipField.clear();
		portField.clear();
		connecting = true;
	}
}

void MainMenu::drawConnectLoad()
{
	if (internetManager->getClient()->getConnectionState() == false)
	{
		connectionStatus.drawCenteredText("Connection failed >:|", STANDARD_WIDTH / 2, STANDARD_HEIGHT / 4, FONT_HEIGHT, 0, 0, 0, 255);
		if (AllegroExt::Input::Button::clickButton(STANDARD_WIDTH/2 - 100, STANDARD_HEIGHT / 2, 200, 100, 0, 255, 0, 255))
		{
			connecting = false;
		}
	}
	else if (internetManager->getClient()->getConnectionState() == true)
	{
		connectionStatus.drawCenteredText("Connected :)", STANDARD_WIDTH / 2, STANDARD_HEIGHT / 4, FONT_HEIGHT, 0, 0, 0, 255);
		
	}
	else
	{
		connectionStatus.drawCenteredText("Connecting...", STANDARD_WIDTH / 2, STANDARD_HEIGHT / 4, FONT_HEIGHT, 0, 0, 0, 255);
	}
}
