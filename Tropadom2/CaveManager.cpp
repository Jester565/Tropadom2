#include "CaveManager.h"
#include "PerlinManager.h"


CaveManager::CaveManager(int worldSeed, int caveMinY, int caveMaxY)
	:worldSeed(worldSeed), caveMinY(caveMinY), caveMaxY(caveMaxY)
{
	caveYNoise = new PerlinManager(0);
	caveRNoise = new PerlinManager(0);
}

bool CaveManager::isAir(int bX, int bY)
{
	for (int i = 0; i < MAX_CAVE_LENGTH / CAVE_CREATE_X; i++)
	{
		int caveBX = bX - bX % CAVE_CREATE_X - CAVE_CREATE_X * i;
		if (bX < 0)
		{
			caveBX = bX - (CAVE_CREATE_X + bX % CAVE_CREATE_X) - CAVE_CREATE_X * i;
		}
		int64_t caveSeed = (int)((worldSeed * 2.0) / (caveBX + .75));
		srand(caveSeed * 3);
		int caveLength = rand() % (MAX_CAVE_LENGTH - MIN_CAVE_LENGTH) + MIN_CAVE_LENGTH;
		if (bX - caveBX > caveLength)
		{
			continue;
		}
		caveYNoise->setSeed(caveSeed);
		int perlinY = caveYNoise->getPerlinVal(bX - caveBX, caveMaxY - caveMinY, caveMinY);
		int lastPerlinY = caveYNoise->getPerlinVal(bX - caveBX - 1, caveMaxY - caveMinY, caveMinY);
		if (perlinY < lastPerlinY)
		{
			if (bY >= perlinY - CAVE_MAX_R && bY <= lastPerlinY + CAVE_MAX_R)
			{
				return true;
			}
		}
		else
		{
			if (bY >= lastPerlinY - CAVE_MAX_R && bY <= perlinY + CAVE_MAX_R)
			{
				return true;
			}
		}
	}
	return false;
}

CaveManager::~CaveManager()
{
}
