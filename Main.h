#pragma once

#define GAME_NAME   "COOL LITTLE SILLY GAME"
#define GAME_WIDTH  384
#define GAME_HEIGHT 240
#define GAME_BPP    32
#define GAME_FRAME_MEMORY_SIZE  (GAME_WIDTH * GAME_HEIGHT * (GAME_BPP / 8))

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


LRESULT CALLBACK MainWindowProc(
    _In_ HWND WindowHandle,
    _In_ UINT Message,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam);


DWORD CreateGameWindow(void);

BOOL GameIsAlreadyRunning(void);

void ProcessPlayerInput(void);

void RenderGameGraphics(void);