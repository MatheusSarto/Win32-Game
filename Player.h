#pragma 
#include <stdint.h>
#include "FrameRender.h"

typedef struct PLAYER
{
	GAMEBITMAP Sprite;
	int16_t PositionX;
	int16_t PositionY;
	uint16_t Health;
} PLAYER;

void InitializePlayer(void);

void ProcessPlayerInput(void);
