#pragma once
#include <stdint.h> 

#define GAME_FRAME_MEMORY_SIZE  (GAME_RES_WIDTH * GAME_RES_HEIGHT * (GAME_BPP / 8))
#define AVG_FPS_EVERY_FRAMES 100

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

void RenderGameGraphics(void);
