#include "AniJoint.h"
#include "AniPart.h"
#define _USE_MATH_DEFINES
#include <math.h>

AniJoint::AniJoint(AniPart * originPart, AniPart * movePart, float oriBindX, float oriBindY)
	:originPart(originPart), movePart(movePart)
{
	length = sqrt(pow(oriBindX - originPart->originX, 2) + pow(oriBindY - originPart->originY, 2));
	radOff = atan2(oriBindY - originPart->originY, oriBindX - originPart->originX);
	if (radOff < 0)
	{
		radOff += M_PI * 2;
	}
}

AniJoint::~AniJoint()
{
}
