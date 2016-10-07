#pragma once
#include "WorldManager.h"
#include <DisplayManager.h>

static const int BLOCK_WIDTH = 30;

static const int SEED = 2938457264624365;
static const int BLOCK_DRAW_OFF = 1;
static const int BLOCK_LIGHT_OFF = 8;

static const int BLOCK_LOAD_BW = 100;
static const int BLOCK_DRAW_BW = AllegroExt::Graphics::WINDOW_X / BLOCK_WIDTH + BLOCK_DRAW_OFF * 2;
static const int BLOCK_OFF_BW = (BLOCK_LOAD_BW - BLOCK_DRAW_BW) / 2;

static const int BLOCK_LOAD_BH = 68;
static const int BLOCK_DRAW_BH = AllegroExt::Graphics::WINDOW_Y / BLOCK_WIDTH + BLOCK_DRAW_OFF * 2;
static const int BLOCK_OFF_BH = (BLOCK_LOAD_BH - BLOCK_DRAW_BH) / 2;
static const int MAX_BY = 500;
static const int INITIAL_BY = 200;

static const int BLOCK_LIGHT_OFF_BW = BLOCK_OFF_BW - BLOCK_LIGHT_OFF;
static const int BLOCK_LIGHT_OFF_BH = BLOCK_OFF_BH - BLOCK_LIGHT_OFF;

static const int BLOCK_WORLD_OFF_PIXELS_W = BLOCK_WIDTH * BLOCK_OFF_BW;
static const int BLOCK_WORLD_OFF_PIXELS_H = BLOCK_WIDTH * BLOCK_OFF_BH;
static const float BLOCK_WORLD_OFF_METERS = (((float)BLOCK_WIDTH)/ B2D_SCALE) * (BLOCK_LIGHT_OFF - BLOCK_DRAW_OFF + 1);