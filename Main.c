#pragma warning(push, 0)
#include <windows.h>
#pragma warning(pop)

#include <stdio.h>
#include <stdint.h>
#include "WindowConfigs.h"
#include "FrameRender.h"
#include "Player.h"

#pragma warning(disable: 4668) // Disable Warnings About WIN10, I'm On WIN11 So That's Nothing I Can Do About It :/
#pragma warning(disable: 28251) // Disable Warning About WinMain Inconsistent, I Think It's Also Releated To WIN10.

HWND g_GameWindow;
BOOL g_GameIsRunning;
GAMEBITMAP g_FrameDrawer;
GAME_PERF_DATA g_PerformanceData;
PLAYER g_Player;


INT  WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, PSTR CommandLine, INT CmdShow)
{
    //Specifying Parameters That Are Not Gonna Be Used.
    UNREFERENCED_PARAMETER(Instance);

	UNREFERENCED_PARAMETER(PrevInstance);

	UNREFERENCED_PARAMETER(CommandLine);
     
	UNREFERENCED_PARAMETER(CmdShow);

    MSG Message = { 0 };
    HANDLE ProcessHandle = GetCurrentProcess();

    //Check If Some Instance Of The Program Is Alredy Running.
    if (GameIsAlreadyRunning() == TRUE)
    {
        MessageBox(NULL, "Another instance of this program is alredy running!", "Error!", MB_ICONEXCLAMATION | MB_OK);

        goto Exit;
    }

    //Ends The Program If It Cannot Be Opend.
    if (CreateGameWindow() != ERROR_SUCCESS)
    {
        goto Exit;
    }

    if (!SetPriorityClass(ProcessHandle, HIGH_PRIORITY_CLASS))
    {
        MessageBox(NULL, "Failed To Set Process Priority!", "Error!", MB_ICONEXCLAMATION | MB_OK);

        goto Exit;
    }
    
    if (!SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST))
    {
        MessageBox(NULL, "Failed To Set Thread Priority!", "Error!", MB_ICONEXCLAMATION | MB_OK);

        goto Exit;
    }

    ShowCursor(FALSE);

    //Inicialize The Global Structure Used To Draw Frames.
    g_FrameDrawer.BitMapInfo.bmiHeader.biSize          = sizeof(g_FrameDrawer.BitMapInfo.bmiHeader);
    g_FrameDrawer.BitMapInfo.bmiHeader.biWidth         = GAME_RES_WIDTH;
    g_FrameDrawer.BitMapInfo.bmiHeader.biHeight        = GAME_RES_HEIGHT;
    g_FrameDrawer.BitMapInfo.bmiHeader.biBitCount      = GAME_BPP;
    g_FrameDrawer.BitMapInfo.bmiHeader.biCompression   = BI_RGB;
    g_FrameDrawer.BitMapInfo.bmiHeader.biPlanes        = 1;
    
    //Allocates Memory To Be Used By The Structer, If It Fails The Pointer Will Be NULL.
    g_FrameDrawer.MemoryBuffer = VirtualAlloc(NULL, GAME_FRAME_MEMORY_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    LoadFileBMP("C:\\Users\\Matheus\\source\\repos\\Project1\\Assets\\Sprites\\Vergil(1).bmp", &g_Player.Sprite);
    InitializePlayer();

    if (!g_FrameDrawer.MemoryBuffer)
    {
        MessageBox(NULL, "Failed to allocate memory!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    //Temp Function Used To Change Buffer Default Collor, So It Does Not Hurt My Eyes. :)
    memset(g_FrameDrawer.MemoryBuffer, 0x7F, GAME_FRAME_MEMORY_SIZE);

    g_GameIsRunning = TRUE;

    
    

    //Main Loop, Here's Where The Game Makes It's Processes ( Can Think Of It As One In Game Frame ).
    while (g_GameIsRunning)
    {

        //Checks For Inputs.
        while (PeekMessage(&Message, g_GameWindow, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&Message);

        }

        ProcessPlayerInput();
        memset(g_FrameDrawer.MemoryBuffer, 0x7F, GAME_FRAME_MEMORY_SIZE);
        RenderGameGraphics();
      
    }

Exit:
    
    return(0);
}