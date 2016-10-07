#pragma once
class AniPart;

class AniJoint
{
public:
	AniJoint(AniPart* originPart, AniPart* movePart, float oriBindX, float oriBindY);
	~AniJoint();

	float length;
	double radOff;
	AniPart* originPart;
	AniPart* movePart;
};

