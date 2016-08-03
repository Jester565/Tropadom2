#include "PhysicsSquare.h"
#include "WorldManager.h"
#include <allegro5/allegro.h>

PhysicsSquare::PhysicsSquare(WorldManager* world, BodyMode bm, float x, float y, float w, float h, float degs, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	:PhysicsBody(world, bm, x, y, degs)
{
	b2FixtureDef boxFixDef;
	boxFixDef.density = 1;
	init(boxFixDef, w, h, r, g, b, a);
}

PhysicsSquare::PhysicsSquare(WorldManager* world, BodyMode bm, b2FixtureDef & boxFixDef, float x, float y, float w, float h, float degs, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	: PhysicsBody(world, bm, x, y, degs)
{
	init(boxFixDef, w, h, r, g, b, a);
}

void PhysicsSquare::init(b2FixtureDef& boxFixDef, float w, float h, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	this->w = w;
	this->h = h;
	this->bitmapW = w * worldManager->getXScale();
	this->bitmapH = h * worldManager->getYScale();
	b2PolygonShape boxShape;
	boxShape.SetAsBox(w / 2.0f, h / 2.0f);
	boxFixDef.shape = &boxShape;
	body->CreateFixture(&boxFixDef);
	bodyBitMap = al_create_bitmap(w * worldManager->getXScale(), h * worldManager->getYScale());
	ALLEGRO_BITMAP* previousBitmap = al_get_target_bitmap();
	al_set_target_bitmap(bodyBitMap);
	al_clear_to_color(al_map_rgba(r, g, b, a));
	al_set_target_bitmap(previousBitmap);
}

PhysicsSquare::~PhysicsSquare()
{
}
