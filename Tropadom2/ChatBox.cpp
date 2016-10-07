#include "ChatBox.h"
#include "InternetManager.h"
#include "PeerManager.h"
#include "Profile.h"
#include "Packets/Trop.pb.h"
#include <Client.h>
#include <TCPConnection.h>
#include <IPacket.h>
#include <OPacket.h>
#include <ShapeRenderer.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/bitmap.h>

const float ChatBox::DEFAULT_FONT_SIZE = 20.0f;

const float ChatBox::TEXT_X_OFF = 4.0f;

const float ChatBox::TF_OFF = 3.0f;

ChatBox::ChatBox(InternetManager* internetManager, float w, float h, float fontSize)
	:PKeyOwner(internetManager->getClient()->getPacketManager()), client(internetManager->getClient()), peerManager(internetManager->getPeerManager()), userProfile(internetManager->getUserProfile()), w(w), h(h), fontSize(fontSize)
{
	addKey(new PKey("F0", this, &ChatBox::keyF0));
	chatBmp = al_create_bitmap(w, h - fontSize * 1.3);
	chatField.set(w - TF_OFF * 2, fontSize * 1.3);
	chatField.setMouseActiveFieldSetting(false);
	chatField.setBackColor(al_map_rgba(0, 0, 0, 230));
	chatField.setTextColor(al_map_rgba(200, 200, 200, 200));
	backColor = al_map_rgba(0, 0, 0, 130);
	fieldSurroundColor = al_map_rgba(255, 200, 200, 255);
	update();
}

void ChatBox::write(const std::string & msg)
{
	chatMsgsMutex.lock();
	chatMsgs.emplace_front(msg, al_map_rgb(255, 255, 255));
	chatMsgsMutex.unlock();
	update();
}

void ChatBox::update()
{
	ALLEGRO_BITMAP* prevBitmap = al_get_target_bitmap();
	al_set_target_bitmap(chatBmp);
	al_clear_to_color(backColor);
	float y = h - fontSize * 1.4 - TF_OFF * 2;
	chatMsgsMutex.lock();
	auto it = chatMsgs.begin();
	while (y > -fontSize && it != chatMsgs.end())
	{
		y -= it->draw(TEXT_X_OFF, y, w - TEXT_X_OFF * 2, fontSize);
		y -= fontSize * .15;
		it++;
	}
	if (it != chatMsgs.end())
	{
		chatMsgs.erase(it, chatMsgs.end());
	}
	chatMsgsMutex.unlock();
	al_set_target_bitmap(prevBitmap);
}

void ChatBox::draw(float x, float y)
{
	updateMutex.lock();
	if (updated)
	{
		update();
		updated = false;
	}
	updateMutex.unlock();
	if (AllegroExt::Input::InputManager::keyTyped('t'))
	{
		chatField.setAsActiveField();
	}
	if (chatField.isActiveField())
	{
		chatField.draw(x + TF_OFF, y + h - fontSize * 1.3 - TF_OFF);
		al_draw_bitmap(chatBmp, x, y - TF_OFF * 2, NULL);
		al_draw_rectangle(x, y + h - fontSize * 1.3 - TF_OFF * 2, x + w, y + h, fieldSurroundColor, TF_OFF);
	}
	else
	{
		al_draw_bitmap(chatBmp, x, y, NULL);
	}
	if (chatField.isEnterPressed())
	{
		std::string msg = chatField.reset();
		if (msg.size() > 0)
		{
			TropPackets::PackF0 packF0;
			packF0.set_msg(msg);
			packF0.set_chattype(0);
			boost::shared_ptr <OPacket> opF0(new OPacket("F0", false));
			opF0->addSendToID(BROADCAST_ID);
			opF0->setData(boost::make_shared <std::string>(packF0.SerializeAsString()));
			client->getTCPConnection()->send(opF0);
			write(userProfile->getName() + ": " + msg);
		}
	}
}

void ChatBox::setBackColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	backColor = al_map_rgba(r, g, b, a);
}

ChatBox::~ChatBox()
{

}

void ChatBox::keyF0(boost::shared_ptr<IPacket> iPack)
{
	TropPackets::PackF0 packF0;
	packF0.ParseFromString(*iPack->getData());
	Peer* sender = peerManager->getPeer(iPack->getSenderID());
	std::string totalMsg = sender->getName() + ": " + packF0.msg();
	ALLEGRO_COLOR textColor;
	if (packF0.chattype() == 0)
	{
		textColor = al_map_rgb(255, 0, 0);
	}
	else
	{
		textColor = al_map_rgb(255, 255, 255);
	}
	chatMsgsMutex.lock();
	chatMsgs.emplace_front(totalMsg, textColor);
	chatMsgsMutex.unlock();
	updateMutex.lock();
	updated = true;
	updateMutex.unlock();
}

ChatMsg::ChatMsg(const std::string & msg, ALLEGRO_COLOR textColor)
	:msg(msg), textColor(textColor)
{
	msgScreenText = new AllegroExt::Graphics::MLScreenText();
}

float ChatMsg::draw(float x, float y, float w, float fontSize)
{
	float textH = msgScreenText->getTextHeight(msg, w, fontSize);
	msgScreenText->setMaxWidth(w);
	msgScreenText->drawText(msg, x, y - textH, fontSize, (uint8_t)(textColor.r * 255), (uint8_t)(textColor.g * 255), (uint8_t)(textColor.b * 255), ((uint8_t)textColor.a * 255));
	return textH;
}

ChatMsg::~ChatMsg()
{
	delete msgScreenText;
	msgScreenText = nullptr;
}
