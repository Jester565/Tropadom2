#include "PerlinManager.h"
#include "BlockColumn.h"

PerlinManager::PerlinManager(int64_t seed)
	:seed(seed)
{
}

bool PerlinManager::isSolid(int bX, int bY, int solidRange, int minSolidY)
{
	int solidBY = BlockColumn::MAX_BY - getNoise(bX, solidRange) - minSolidY;
	if (bY >= solidBY)
	{
		//return true;
	}
	//return false;
	return (rand() % 100 < 50);
}

int PerlinManager::getNoise(int bX, int range)
{
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
	noiseBY *= ((double)range / (double)maxRange);
	return (int)noiseBY;
}


PerlinManager::~PerlinManager()
{
}

double PerlinManager::genPerlinNoise(int bX, int freq, int range)
{
	int bY0 = genPseudoRand(bX - (bX%freq) - freq, range);
	int bY1 = genPseudoRand(bX - (bX%freq), range);
	int bY2 = genPseudoRand(bX - (bX%freq) + freq, range);
	int bY3 = genPseudoRand(bX - (bX%freq) + freq * 2, range);
	double betweenPercent = (bX%freq)/(double)freq;
	return cubicInterpolate(bY0, bY1, bY2, bY3, betweenPercent);
}
