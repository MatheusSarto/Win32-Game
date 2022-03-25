#include <stdio.h>

#pragma warning(push, 0)
#include <windows.h>
#pragma warning(pop)

#include "Main.h"

HWND g_GameWindow;

BOOL g_GameIsRunning;

GAMEBITMAP g_FrameDrawer;

INT  WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, PSTR CommandLine, INT CmdShow)
{
    //Specifying parameters that are not gonna be used.
    UNREFERENCED_PARAMETER(Instance);

	UNREFERENCED_PARAMETER(PrevInstance);

	UNREFERENCED_PARAMETER(CommandLine);

	UNREFERENCED_PARAMETER(CmdShow);

    //Check if some instance of the program is alredy running.
    if (GameIsAlreadyRunning() == TRUE)
    {
        MessageBox(NULL, "Another instance of this program is alredy running!", "Error!", MB_ICONEXCLAMATION | MB_OK);

        goto Exit;
    }

    //Ends the program if it cannot be opend.
    if (CreateGameWindow() != ERROR_SUCCESS)
    {
        goto Exit;
    }


    //Inicialize the global structure used to draw frames.
    g_FrameDrawer.BitMapInfo.bmiHeader.biSize         = sizeof(g_FrameDrawer.BitMapInfo.bmiHeader);
    g_FrameDrawer.BitMapInfo.bmiHeader.biWidth        = GAME_WIDTH;
    g_FrameDrawer.BitMapInfo.bmiHeader.biHeight       = GAME_HEIGHT;
    g_FrameDrawer.BitMapInfo.bmiHeader.biBitCount     = GAME_BPP;
    g_FrameDrawer.BitMapInfo.bmiHeader.biCompression  = BI_RGB;
    g_FrameDrawer.BitMapInfo.bmiHeader.biPlanes       = 1;

    //Allocates memory to be used by the structer, if it fails the pointer will be NULL.
    g_FrameDrawer.MemoryBuffer = VirtualAlloc(NULL, GAME_FRAME_MEMORY_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    
    if (!g_FrameDrawer.MemoryBuffer)
    {
        MessageBox(NULL, "Failed to allocate memory!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    MSG Message = { 0 };

    g_GameIsRunning = TRUE;
    

    //Main loop, here's where the game makes it's processes ( can think of it as one in game frame ).
    while (g_GameIsRunning)
    {
        //Checks for inputs.
        while (PeekMessage(&Message, g_GameWindow, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&Message);

        }

        ProcessPlayerInput();

        RenderGameGraphics();

        Sleep(1);
    }


Exit:
    
    return(0);
}

//Where messages sent by windows will be handled.
LRESULT CALLBACK MainWindowProc(
    _In_ HWND WindowHandle,        // handle to window
    _In_ UINT Message,        // message identifier
    _In_ WPARAM wParam,    // first message parameter
    _In_ LPARAM lParam)    // second message parameter
{
    LRESULT Result = 0;  

    switch (Message)
    {
        case WM_CLOSE:
        {
            g_GameIsRunning = FALSE;

            DestroyWindow(WindowHandle);
            PostQuitMessage(0);
            break;
        }

        default:
        {
            Result = DefWindowProc(WindowHandle, Message, wParam, lParam);
        }

    }
    return (Result);
}

//Creates the window and inicialize all necessary variables and structs, as well as the global: g_GameWindow.
DWORD CreateGameWindow(void)
{
    DWORD Result = ERROR_SUCCESS;
    
    WNDCLASSEX WindowClass = { 0 };

    WindowClass.cbSize         = sizeof(WNDCLASSEX);
    WindowClass.style          = 0;
    WindowClass.lpfnWndProc    = MainWindowProc;
    WindowClass.cbClsExtra     = 0;
    WindowClass.cbWndExtra     = 0;
    WindowClass.hInstance      = GetModuleHandle(NULL);
    WindowClass.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
    WindowClass.hCursor        = LoadCursor(NULL, IDI_APPLICATION);
    WindowClass.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    WindowClass.lpszMenuName   = NULL;
    WindowClass.lpszClassName  = GAME_NAME "_WINDOWCLASS";
    WindowClass.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&WindowClass))
    {
        Result = GetLastError();

        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        
        goto Exit;
    }

    g_GameWindow = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        WindowClass.lpszClassName,
        GAME_NAME,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
        NULL, NULL, GetModuleHandle(NULL), NULL
    );

    if (g_GameWindow == NULL)
    {
        Result = GetLastError();

        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);

        goto Exit;
    }

Exit:

    return(Result);
}

BOOL GameIsAlreadyRunning(void)
{
    HANDLE Mutex = NULL;

    Mutex = CreateMutex(NULL, FALSE,GAME_NAME "_M");

    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}


void ProcessPlayerInput(void)
{
    SHORT ThisKeyIsPressed = GetAsyncKeyState(VK_ESCAPE);

    if (ThisKeyIsPressed)
    {
        SendMessage(g_GameWindow, WM_CLOSE, 0, 0);
    }
}

void RenderGameGraphics(void)
{
    
}