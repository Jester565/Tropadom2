#include "PhysicsCircle.h"
#include "WorldManager.h"
#include <allegro5/allegro_primitives.h>

PhysicsCircle::PhysicsCircle(WorldManager * worldManager, BodyMode bm, float x, float y, float radius, float degs, int r, int g, int b, int a)
	:PhysicsBody(worldManager, bm, x, y, degs)
{
	b2FixtureDef fixDef;
	fixDef.density = 1;
	init(fixDef, radius, r, g, b, a);
}

PhysicsCircle::PhysicsCircle(WorldManager* worldManager, BodyMode bm, b2FixtureDef & fixDef, float x, float y, float radius, float degs, int r, int g, int b, int a)
	: PhysicsBody(worldManager, bm, x, y, degs)
{
	init(fixDef, radius, r, g, b, a);
}

void PhysicsCircle::init(b2FixtureDef& fixDef, float radius, int r, int g, int b, int a)
{
	this->w = radius * 2;
	this->h = radius * 2;
	this->bitmapW = w * B2D_SCALE;
	this->bitmapH = h * B2D_SCALE;
	b2CircleShape circleShape;
	circleShape.m_p.SetZero();
	circleShape.m_radius = radius;
	fixDef.shape = &circleShape;
	body->CreateFixture(&fixDef);
	bodyBitMap = al_create_bitmap(bitmapW, bitmapH);
	ALLEGRO_BITMAP* previousBitmap = al_get_target_bitmap();
	al_set_target_bitmap(bodyBitMap);
	al_draw_filled_circle(bitmapW/2.0, bitmapH/2.0, bitmapW/2.0, al_map_rgba(r, g, b, a));
	al_set_target_bitmap(previousBitmap);
}

PhysicsCircle::~PhysicsCircle()
{
}
