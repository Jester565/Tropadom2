#pragma once
#include <stdint.h>
#include <math.h>
class PerlinManager
{
public:
	static const int FREQ_MAX = 50;
	static const int FREQ_MIN = 4;
	static const int FREQ_DIV = 2;
	static const int RANGE_DIV = 2;
	PerlinManager(int64_t seed);

	bool isSolid(int bX, int bY, int solidRange, int minSolidY);

	int getNoise(int bX, int range);

	~PerlinManager();

private:
	double genPerlinNoise(int bX, int freq, int range);

	int genPseudoRand(int bX, int range)
	{
		return (bX + seed) ^ 5 % range;
	}
	double cubicInterpolate(int bY0, int bY1, int bY2, int bY3, double betweenPercent)
	{
		double a0 = bY3 - bY2 - bY0 + bY1;
		double a1 = bY0 - bY1 - a0;
		double a2 = bY2 - bY0;
		double a3 = bY1;
		return (a0 * pow(betweenPercent, 3) + a1 * pow(betweenPercent, 2) + a2 * betweenPercent + a3);
	}

	int64_t seed;
};

