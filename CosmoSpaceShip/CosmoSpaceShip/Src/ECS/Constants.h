#pragma once

#define PERSON_SIZE 128
#define MAP_TILE_SCALE 2
#define PLAYER_SCALE 5
#define WIDTH_IN_TILES 20
#define HEIGHT_IN_TILES 16
#define WINDOW_WIDTH WIDTH_IN_TILES * 32 * MAP_TILE_SCALE
#define WINDOW_HEIGHT HEIGHT_IN_TILES * 32 *MAP_TILE_SCALE

#define PLAY_WIDTH WINDOW_WIDTH - 32 * MAP_TILE_SCALE * 2
#define PLAY_HEIGHT WINDOW_HEIGHT - 32 * MAP_TILE_SCALE * 2
#define PlAY_HD_WIDTH 1920
#define PLAY_HD_HEIGHT 1080
//#define WINDOW_WIDTH 1200
//#define WINDOW_HEIGHT 900

static constexpr int defaultPlayerScale   = 3;
static constexpr int defaultMapTileScale  = 2;
static constexpr int defaultWidthInTiles  = 20;
static constexpr int defaultHeightInTiles = 16;
static constexpr int defaultWindowWidth   = 32 * 2 * defaultWidthInTiles;
static constexpr int defaultWindowHeight  = 32 * 2 * defaultHeightInTiles;
static constexpr int defaultPlayWidth     = defaultWindowWidth  - 32 * defaultMapTileScale * 2;
static constexpr int defaultPlayHeight    = defaultWindowHeight - 32 * defaultMapTileScale * 2;

static constexpr int mediumShipSizeHard = 32;
static constexpr int heavyShipSize  = 64;
static constexpr int backgroundTileSize = 32;

static constexpr int ScreenShipRatioW = 9; //1152/128
static constexpr int ScreenShipRatioH = 6; //896/128

static constexpr float shipDelta = 1.5;

static constexpr float mediumShipScreenCoef = 0.06f;
static constexpr float heavyShipScreenCoef = 0.27f;
static constexpr float ricochetHandlingCoef = 76.8f;
