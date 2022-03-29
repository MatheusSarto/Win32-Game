#pragma once

#define GAME_NAME   "COOL LITTLE SILLY GAME"
#define GAME_RES_WIDTH  384
#define GAME_RES_HEIGHT 240
#define GAME_BPP    32
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

typedef struct GAME_PERF_DATA
{
    uint64_t TotalFramesRendered;
    uint32_t RawFramesPerSecondAverage;
    uint32_t VirtualFramesPerSecondAverage;
    LARGE_INTEGER PerfFrequency;
    LARGE_INTEGER FrameStart;
    LARGE_INTEGER FrameEnd;
    LARGE_INTEGER ElapsedMicroSecondsPerFrame;
    int32_t MonitorWidth;
    int32_t MonitorHeight;
    MONITORINFO MonitorInfo;

} GAME_PERF_DATA;

LRESULT CALLBACK MainWindowProc(
    _In_ HWND WindowHandle,
    _In_ UINT Message,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam);


DWORD CreateGameWindow(void);

BOOL GameIsAlreadyRunning(void);

void ProcessPlayerInput(void);

void RenderGameGraphics(void);