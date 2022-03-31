#pragma warning(push, 0)
#include <windows.h>
#pragma warning(pop)

#include "ProcessInputs.h"
extern HWND g_GameWindow;

void ProcessPlayerInput(void)
{
    int16_t ThisKeyIsPressed = GetAsyncKeyState(VK_ESCAPE);

    if (ThisKeyIsPressed)
    {
        SendMessage(g_GameWindow, WM_CLOSE, 0, 0);
    }
}