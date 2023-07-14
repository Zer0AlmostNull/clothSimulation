#include "types.h"
#include "math.h"

#include "macros.h"

#pragma once

// ---------- SCREEN ---------

const u16 WND_WIDTH = 1280;
const u16 WND_HEIGHT = 720;

const u32 PIXEL_COUNT = WND_WIDTH * WND_HEIGHT;

const u16 WND_WIDTH_HALF = WND_WIDTH / 2;
const u16 WND_HEIGHT_HALF = WND_HEIGHT / 2;

const f32 GRAVITY = 180.0f;


// -------- GRID --------
const u8 GRID_WIDTH = 60;
const u8 GRID_HEIGHT = 40;

const u8 POINTS_DIST = 15;
