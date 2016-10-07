#include "WorldDebugDraw.h"
#include "WorldManager.h"
#include <allegro5/allegro_primitives.h>
#include <vector>
#include <iostream>

WorldDebugDraw::WorldDebugDraw(WorldManager * wm)
	:wm(wm)
{
}

void WorldDebugDraw::DrawPolygon(const b2Vec2 * verts, int32 vertCount, const b2Color & color)
{
	std::vector <float> drawVerts(vertCount * 2);
	for (int i = 0; i < vertCount; i++)
	{
		drawVerts.at(i * 2) = (verts[i].x - wm->getMeterWorldX()) * B2D_SCALE;;
		drawVerts.at(i * 2 + 1) = -(verts[i].y - wm->getMeterWorldY()) * B2D_SCALE;
	}
	al_draw_polygon(drawVerts.data(), vertCount, ALLEGRO_LINE_JOIN_BEVEL, al_map_rgba_f(color.r, color.g, color.b, color.a), LINE_THICK, NULL);
	std::cout << "POLY draw called: " << vertCount << std::endl;
}

void WorldDebugDraw::DrawSolidPolygon(const b2Vec2 * verts, int32 vertCount, const b2Color & color)
{
	std::vector <float> drawVerts(vertCount * 2);
	for (int i = 0; i < vertCount; i++)
	{
		drawVerts.at(i * 2) = (verts[i].x - wm->getMeterWorldX()) * B2D_SCALE;;
		drawVerts.at(i * 2 + 1) = -(verts[i].y - wm->getMeterWorldY()) * B2D_SCALE;
	}
	al_draw_filled_polygon(drawVerts.data(), vertCount, al_map_rgba_f(color.r, color.g, color.b, color.a));
}

void WorldDebugDraw::DrawCircle(const b2Vec2 & center, float32 radius, const b2Color & color)
{
	float correctedX = (center.x - wm->getMeterWorldX()) * B2D_SCALE;
	float correctedY = -(center.y - wm->getMeterWorldY()) * B2D_SCALE;
	float correctedRadius = radius * B2D_SCALE;
	al_draw_circle(correctedX, correctedY, correctedRadius, al_map_rgba_f(color.r, color.g, color.b, color.a), LINE_THICK);
}

void WorldDebugDraw::DrawSolidCircle(const b2Vec2 & center, float32 radius, const b2Vec2 & axis, const b2Color & color)
{
	float correctedX = (center.x - wm->getMeterWorldX()) * B2D_SCALE;
	float correctedY = -(center.y - wm->getMeterWorldY()) * B2D_SCALE;
	float correctedRadius = radius * B2D_SCALE;
	al_draw_filled_circle(correctedX, correctedY, correctedRadius, al_map_rgba_f(color.r, color.g, color.b, color.a));
}

void WorldDebugDraw::DrawSegment(const b2Vec2 & p1, const b2Vec2 & p2, const b2Color & color)
{
	float correctedX1 = (p1.x - wm->getMeterWorldX()) * B2D_SCALE;
	float correctedY1 = -(p1.y - wm->getMeterWorldY()) * B2D_SCALE;
	float correctedX2 = (p2.x - wm->getMeterWorldX()) * B2D_SCALE;
	float correctedY2 = -(p2.y - wm->getMeterWorldY()) * B2D_SCALE;
	al_draw_line(correctedX1, correctedY1, correctedX2, correctedY2, al_map_rgba_f(color.r, color.g, color.b, color.a), LINE_THICK);
}

void WorldDebugDraw::DrawTransform(const b2Transform & xf)
{
	
}

void WorldDebugDraw::DrawPoint(const b2Vec2 & point, float32 size, const b2Color & color)
{
	
}

WorldDebugDraw::~WorldDebugDraw()
{
}
