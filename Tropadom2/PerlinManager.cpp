#include "PerlinManager.h"
#include "BlockColumn.h"

PerlinManager::PerlinManager(int64_t seed)
	:seed(seed)
{
}

int PerlinManager::getPerlinVal(int bX, int range, int minY)
{
	int solidBY = getNoise(bX, range) + minY;
	return solidBY;
}

int PerlinManager::getNoise(int bX, int range)
{
	int iRange = range;
	int freq = FREQ_MAX;
	int maxRange = 0;
	double noiseBY = 0;
	while (freq >= FREQ_MIN)
	{
		noiseBY += genPerlinNoise(bX, freq, range);
		freq /= FREQ_DIV;
		range /= RANGE_DIV;
		maxRange += range;
	 }
	noiseBY *= ((double)iRange / (double)maxRange);
	return (int)noiseBY;
}


PerlinManager::~PerlinManager()
{
}

double PerlinManager::genPerlinNoise(int bX, int freq, int range)
{
	int bXFreqFit = bX - (bX%freq);
	double betweenPercent = (bX%freq) / (double)freq;
	if (bX < 0)
	{
		bXFreqFit -= freq;
		betweenPercent = (freq + (bX%freq)) / (double)freq;
	}
	int bY0 = genPseudoRand(bXFreqFit - freq, range);
	int bY1 = genPseudoRand(bXFreqFit, range);
	int bY2 = genPseudoRand(bXFreqFit + freq, range);
	int bY3 = genPseudoRand(bXFreqFit + freq * 2, range);
	return cubicInterpolate(bY0, bY1, bY2, bY3, betweenPercent);
}
