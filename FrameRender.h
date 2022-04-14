#pragma once
#include <stdint.h> 
#include "WindowConfigs.h"

#define TILE_WIDTH 16
#define TILE_HEIGHT 16
#define GAME_FRAME_MEMORY_SIZE  (GAME_RES_WIDTH * GAME_RES_HEIGHT * (GAME_BPP / 8))
#define TILE_MEMORY_SIZE (TILE_WIDTH * TILE_HEIGHT) * (GAME_BPP / 8)
#define BASIC_ARMOR 0

typedef struct GAMEBITMAP
{
    BITMAPINFO BitMapInfo;
    void* MemoryBuffer;

} GAMEBITMAP;
 
typedef struct PIXEL32
{
    uint8_t Blue;
    uint8_t Green;
    uint8_t Red;
    uint8_t Alpha;

} PIXEL32;

DWORD LoadFileBMP(_In_ char* FileName, _Inout_ GAMEBITMAP* GameBitmap);

void GetTileCoordinate(uint16_t X,  uint16_t Y,  uint32_t** TileCoordinate);

void DrawMainCharacter(uint16_t X, uint16_t Y, uint32_t* CharacterCoordinates);

void RenderGameGraphics(void);