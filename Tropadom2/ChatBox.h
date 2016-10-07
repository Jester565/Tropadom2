#pragma once
#include <PKeyOwner.h>
#include <MLScreenText.h>
#include <TextField.h>
#include <allegro5/color.h>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

struct ChatMsg
{
	ChatMsg(const std::string& msg, ALLEGRO_COLOR textColor);

	float draw(float x, float y, float w, float fontSize);

	AllegroExt::Graphics::MLScreenText* msgScreenText;

	std::string msg;

	ALLEGRO_COLOR textColor;

	~ChatMsg();
};

class Profile;
class Client;
class PeerManager;
class InternetManager;
class IPacket;
struct ALLEGRO_BITMAP;

class ChatBox : public PKeyOwner
{
public:
	static const float DEFAULT_FONT_SIZE;
	static const float TEXT_X_OFF;
	static const float TF_OFF;
	ChatBox(InternetManager* internetManager, float w, float h, float fontSize = DEFAULT_FONT_SIZE);

	void write(const std::string& msg);
	
	void update();

	void draw(float x, float y);

	void setBackColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

	int getW()
	{
		return w;
	}

	int getH()
	{
		return h;
	}

	~ChatBox();

protected:
	std::list <ChatMsg> chatMsgs;
	AllegroExt::Input::TextField chatField;
	Client* client;
	Profile* userProfile;
	PeerManager* peerManager;
	void keyF0(boost::shared_ptr <IPacket> iPack);
	ALLEGRO_BITMAP* chatBmp;
	ALLEGRO_COLOR backColor;
	ALLEGRO_COLOR fieldSurroundColor;
	boost::mutex chatMsgsMutex;
	boost::mutex updateMutex;
	float w;
	float h;
	float typeH;
	float fontSize;
	bool updated = false;
};

