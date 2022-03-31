#pragma once
#include <stdint.h>

#define GAME_NAME   "COOL LITTLE SILLY GAME"
#define GAME_RES_WIDTH  384
#define GAME_RES_HEIGHT 240
#define GAME_BPP    32

typedef struct GAME_PERF_DATA
{
    int32_t MonitorWidth;
    int32_t MonitorHeight;
    MONITORINFO MonitorInfo;

} GAME_PERF_DATA;

DWORD CreateGameWindow(void);

BOOL GameIsAlreadyRunning(void);
