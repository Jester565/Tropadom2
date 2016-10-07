#pragma once
#include "Block.h"

class LightBlockerContainer;

class ShadeBlock : public Block
{
public:
	virtual void draw(float pX, float pY) override;

	virtual void initSurround(Block* n, Block* e, Block* s, Block* w, int64_t bX, int64_t bY) override;

	virtual void updateSurround(float pX, float pY);

	virtual void destroySurround() override;

	virtual bool isTransparent() override
	{
		return false;
	}

	LightBlockerContainer* lbc;

	virtual ~ShadeBlock();

protected:
	ShadeBlock(WorldManager* wm, TerrainManager* tm);

	ShadeBlock(const ShadeBlock& original);
};

