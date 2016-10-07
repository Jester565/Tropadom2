#pragma once
#include <Image.h>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

class AniJoint;

class AniPart
{
public:
	AniPart(const std::string& imgPath, float imgW, float imgH, float oriX, float oriY);
	
	void translate(float dX, float dY);
	void setXY(float x, float y);
	void rotateDegs(float degs)
	{
		rotate(degs * (M_PI / 180.0));
	}
	void changeDegs(float dDegs)
	{
		changeRads(dDegs * (M_PI / 180.0));
	}
	void rotate(float rads);
	void changeRads(float dRads)
	{
		rotate(rads + dRads);
	}
	void draw();
	AniJoint* createMoveJoint(AniPart* movePart, float moveOriX, float moveOriY);
	void removeMoveJoint(AniJoint* aniJoint);
	void removeOriJoint(AniJoint* aniJoint);

	~AniPart();

	float originX;
	float originY;

private:
	std::vector <AniJoint*> moveJoints;
	std::vector <AniJoint*> oriJoints;
	AllegroExt::Graphics::Image* img;
	float imgW;
	float imgH;
	double sX;
	double sY;
	float rads;
	float x;
	float y;
};

