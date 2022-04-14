#pragma warning(push, 0)
#include <windows.h>
#pragma warning(pop)

#include "Player.h"
#include "WindowConfigs.h"
#include "FrameRender.h"
extern HWND g_GameWindow;
extern PLAYER g_Player;

void ProcessPlayerInput(void)
{
    int16_t CloseGame = GetAsyncKeyState(VK_ESCAPE);

    int16_t RightKeyIsDown = GetAsyncKeyState(VK_RIGHT) | GetAsyncKeyState('D');
    static int16_t RightKeyWasDown;

    int16_t LeftKeyIsDown = GetAsyncKeyState(VK_LEFT) | GetAsyncKeyState('A');
    static int16_t LeftKeyWasDown;

    int16_t UpKeyIsDown = GetAsyncKeyState(VK_UP) | GetAsyncKeyState('W');
    static int16_t UpKeyWasDown;

    int16_t BottomKeyIsDown = GetAsyncKeyState(VK_DOWN) | GetAsyncKeyState('S');
    static int16_t BottomKeyWasDown;

    if (CloseGame)
    {
        SendMessage(g_GameWindow, WM_CLOSE, 0, 0);
    }
    if (RightKeyIsDown && !RightKeyWasDown && g_Player.PositionX < (GAME_RES_WIDTH - TILE_WIDTH) / TILE_WIDTH)
    {
        g_Player.PositionX++;
    }
    if (LeftKeyIsDown && !LeftKeyWasDown && g_Player.PositionX != 0)
    {
        g_Player.PositionX--;
    }
    if (UpKeyIsDown && !UpKeyWasDown && g_Player.PositionY < GAME_RES_HEIGHT / TILE_HEIGHT)
    {
        g_Player.PositionY++;
    }
    if (BottomKeyIsDown && !BottomKeyWasDown && g_Player.PositionY != 1)
    {
        g_Player.PositionY--;
    }

    RightKeyWasDown = RightKeyIsDown;
    LeftKeyWasDown = LeftKeyIsDown;
    UpKeyWasDown = UpKeyIsDown;
    BottomKeyWasDown = BottomKeyIsDown;
 
}

void InitializePlayer(void)
{
    DWORD Error = ERROR_SUCCESS;
    g_Player.PositionX = 0;
    g_Player.PositionY = 1;
    g_Player.Health = 1000;

    
    int a;
    int b;

Exit:
    return Error;
}