#include "AniPart.h"
#include "AniJoint.h"
#include <allegro5/bitmap.h>

AniPart::AniPart(const std::string & imgPath, float imgW, float imgH, float oriX, float oriY)
	:originX(oriX), originY(oriY), x(0), y(0)
{
	img = new AllegroExt::Graphics::Image(imgPath);
	this->imgW = imgW;
	this->imgH = imgH;
	sX = al_get_bitmap_width(img->imgMap) / imgW;
	sY = al_get_bitmap_height(img->imgMap) / imgH;
	img->setImageOrigin(oriX * sX, oriY * sY);
}

void AniPart::translate(float dX, float dY)
{
	this->x += dX;
	this->y += dY;
	for (int i = 0; i < moveJoints.size(); i++)
	{
		moveJoints.at(i)->movePart->translate(dX, dY);
	}
}

void AniPart::setXY(float x, float y)
{
	float xDif = x - this->x;
	float yDif = y - this->y;
	translate(xDif, yDif);
}

void AniPart::rotate(float rads)
{
	this->rads = rads;
	for (int i = 0; i < moveJoints.size(); i++)
	{
		float partX = x + cos(rads + moveJoints.at(i)->radOff) * moveJoints.at(i)->length;
		float partY = y + sin(rads + moveJoints.at(i)->radOff) * moveJoints.at(i)->length;
		moveJoints.at(i)->movePart->setXY(partX, partY);
	}
}

void AniPart::draw()
{
	img->setRads(rads);
	img->draw(x, y, imgW, imgH);
}

AniJoint * AniPart::createMoveJoint(AniPart * movePart, float moveOriX, float moveOriY)
{
	AniJoint* joint = new AniJoint(this, movePart, moveOriX, moveOriY);
	moveJoints.push_back(joint);
	movePart->oriJoints.push_back(joint);
	movePart->x = x + (moveOriX - originX);
	movePart->y = y + (moveOriY - originY);
	return joint;
}

void AniPart::removeMoveJoint(AniJoint * aniJoint)
{
	for (int i = 0; i < moveJoints.size(); i++)
	{
		if (moveJoints.at(i) == aniJoint)
		{
			moveJoints.erase(moveJoints.begin() + i);
			return;
		}
	}
}

void AniPart::removeOriJoint(AniJoint * aniJoint)
{
	for (int i = 0; i < oriJoints.size(); i++)
	{
		if (oriJoints.at(i) == aniJoint)
		{
			oriJoints.erase(oriJoints.begin() + i);
			return;
		}
	}
}

AniPart::~AniPart()
{
	delete img;
	img = nullptr;
	for (int i = 0; i < moveJoints.size(); i++)
	{
		moveJoints.at(i)->movePart->removeOriJoint(moveJoints.at(i));
		delete moveJoints.at(i);
		moveJoints.at(i) = nullptr;
	}
	moveJoints.clear();
	for (int i = 0; i < oriJoints.size(); i++)
	{
		oriJoints.at(i)->originPart->removeMoveJoint(oriJoints.at(i));
		delete oriJoints.at(i);
		oriJoints.at(i) = nullptr;
	}
	oriJoints.clear();
}
