#pragma once
#include <Testbed/Framework/DebugDraw.h>
#include <Testbed/Framework/Test.h>
#include <Box2D/Box2D.h>

class WorldManager;

class WorldDebugDraw : public b2Draw
{
public:
	static const int LINE_THICK = 4;
	WorldDebugDraw(WorldManager* wm);

	void DrawPolygon(const b2Vec2* verts, int32 vertCount, const b2Color& color) override;

	void DrawSolidPolygon(const b2Vec2* verts, int32 vertCount, const b2Color& color) override;

	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override;

	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override;

	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;

	void DrawTransform(const b2Transform& xf) override;

	void DrawPoint(const b2Vec2& point, float32 size, const b2Color& color) override;

	~WorldDebugDraw();

private:
	WorldManager* wm;
};

