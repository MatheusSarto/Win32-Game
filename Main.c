#pragma warning(push, 0)
#include <windows.h>
#pragma warning(pop)
#pragma warning(disable: 4668) // Disable Warnings About WIN10, I'm On WIN11 So That's Nothing I Can Do About It :/
#pragma warning(disable: 28251) // Disable Warning About WinMain Inconsistent, I Think It's Also Releated To WIN10.

#include <stdio.h>
#include <stdint.h>
#include "Main.h"

HWND g_GameWindow;
BOOL g_GameIsRunning;
GAMEBITMAP g_FrameDrawer;
GAME_PERF_DATA g_PerformanceData;


INT  WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, PSTR CommandLine, INT CmdShow)
{
    //Specifying Parameters That Are Not Gonna Be Used.
    UNREFERENCED_PARAMETER(Instance);

	UNREFERENCED_PARAMETER(PrevInstance);

	UNREFERENCED_PARAMETER(CommandLine);

	UNREFERENCED_PARAMETER(CmdShow);

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

    QueryPerformanceFrequency(&g_PerformanceData.PerfFrequency);

    //Inicialize The Global Structure Used To Draw Frames.
    g_FrameDrawer.BitMapInfo.bmiHeader.biSize         = sizeof(g_FrameDrawer.BitMapInfo.bmiHeader);
    g_FrameDrawer.BitMapInfo.bmiHeader.biWidth        = GAME_RES_WIDTH;
    g_FrameDrawer.BitMapInfo.bmiHeader.biHeight       = GAME_RES_HEIGHT;
    g_FrameDrawer.BitMapInfo.bmiHeader.biBitCount     = GAME_BPP;
    g_FrameDrawer.BitMapInfo.bmiHeader.biCompression  = BI_RGB;
    g_FrameDrawer.BitMapInfo.bmiHeader.biPlanes       = 1;

    //Allocates Memory To Be Used By The Structer, If It Fails The Pointer Will Be NULL.
    g_FrameDrawer.MemoryBuffer = VirtualAlloc(NULL, GAME_FRAME_MEMORY_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    
    if (!g_FrameDrawer.MemoryBuffer)
    {
        MessageBox(NULL, "Failed to allocate memory!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        goto Exit;
    }

    //Temp Function Used To Change Buffer Default Collor, So It Does Not Hurt My Eyes. :)
    memset(g_FrameDrawer.MemoryBuffer, 0x7F, GAME_FRAME_MEMORY_SIZE);

    MSG Message = { 0 };

    g_GameIsRunning = TRUE;
    

    //Main Loop, Here's Where The Game Makes It's Processes ( Can Think Of It As One In Game Frame ).
    while (g_GameIsRunning)
    {
        QueryPerformanceCounter(&g_PerformanceData.FrameStart);

        //Checks For Inputs.
        while (PeekMessage(&Message, g_GameWindow, 0, 0, PM_REMOVE))
        {
            DispatchMessage(&Message);

        }

        ProcessPlayerInput();

        RenderGameGraphics();
        
        
        QueryPerformanceCounter(&g_PerformanceData.FrameEnd);
        g_PerformanceData.ElapsedMicroSecondsPerFrame.QuadPart = g_PerformanceData.FrameEnd.QuadPart - g_PerformanceData.FrameStart.QuadPart;
        g_PerformanceData.ElapsedMicroSecondsPerFrame.QuadPart *= 1000000;
        g_PerformanceData.ElapsedMicroSecondsPerFrame.QuadPart /= g_PerformanceData.PerfFrequency.QuadPart;

        //Sleep(1); 

        g_PerformanceData.TotalFramesRendered++;

        if ( (g_PerformanceData.TotalFramesRendered % AVG_FPS_EVERY_FRAMES) == 0)
        {
            char str[64] = { 0 };

            _snprintf_s(str, _countof(str), _TRUNCATE, "Elapsed ms: %lli\n", g_PerformanceData.ElapsedMicroSecondsPerFrame.QuadPart);

            OutputDebugString(str);
        }
    }


Exit:
    
    return(0);
}


//Where Messages Sent By Windows Will Be Handled.
LRESULT CALLBACK MainWindowProc(
    _In_ HWND WindowHandle,        // Handle To Window
    _In_ UINT Message,        // Message Identifier
    _In_ WPARAM wParam,    // First Message Parameter
    _In_ LPARAM lParam)    // Second Message Parameter
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

//Creates The Window And Inicialize All Necessary Variables And Structs, As Well As The Global: g_GameWindow.
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
    WindowClass.hbrBackground  = CreateSolidBrush(RGB(0xDC,0x14,0x3C));
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
        0,
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

    g_PerformanceData.MonitorInfo.cbSize = sizeof(MONITORINFO);
 
    if (!GetMonitorInfo(MonitorFromWindow(g_GameWindow, MONITOR_DEFAULTTOPRIMARY), &g_PerformanceData.MonitorInfo))
    {
        Result = ERROR_MONITOR_NO_DESCRIPTOR;

        goto Exit;
    }

    g_PerformanceData.MonitorWidth = g_PerformanceData.MonitorInfo.rcMonitor.right - g_PerformanceData.MonitorInfo.rcMonitor.left;
    g_PerformanceData.MonitorHeight = g_PerformanceData.MonitorInfo.rcMonitor.bottom - g_PerformanceData.MonitorInfo.rcMonitor.top;
     

    if (!SetWindowLongPtr(g_GameWindow, GWL_STYLE, (WS_OVERLAPPEDWINDOW | WS_VISIBLE) & ~WS_OVERLAPPEDWINDOW))
    {
        MessageBox(NULL, "Could Not Apply Corret Styles!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        Result = GetLastError();

        goto Exit;
    }

    if (!SetWindowPos(
        g_GameWindow, HWND_TOP, 
        g_PerformanceData.MonitorInfo.rcMonitor.left, g_PerformanceData.MonitorInfo.rcMonitor.top,
        g_PerformanceData.MonitorWidth, g_PerformanceData.MonitorHeight,
        SWP_FRAMECHANGED | SWP_FRAMECHANGED)
        )
    {
        MessageBox(NULL, "Full Screen Mode Could Not Be Inicialized", "Error!", MB_ICONEXCLAMATION | MB_OK);
        Result = GetLastError();

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
    int16_t ThisKeyIsPressed = GetAsyncKeyState(VK_ESCAPE);
     
    if (ThisKeyIsPressed)
    {
        SendMessage(g_GameWindow, WM_CLOSE, 0, 0);
    }
}

void RenderGameGraphics(void)
{

    PIXEL32 Pixel = { 0 };

    Pixel.Red   = 0x80;
    Pixel.Green = 0x00;
    Pixel.Blue  = 0x80;
    Pixel.Alpha = 0xff;
    

    for (int x = 0; x < GAME_RES_WIDTH * GAME_RES_HEIGHT; x++)
    {
        memcpy_s((PIXEL32*)g_FrameDrawer.MemoryBuffer + x, sizeof(PIXEL32), &Pixel, sizeof(PIXEL32));
    }
     
    HDC DeviceContext = GetDC(g_GameWindow);
    
    StretchDIBits(
        DeviceContext,
        0, 0,           //Coordinates Where The Image Starts Inside Of The Window.
        g_PerformanceData.MonitorWidth, g_PerformanceData.MonitorHeight, //Width And Height Of The Window.
        0, 0,   //Coordinates Where The Rectangle Starts On The Buffer.
        GAME_RES_WIDTH, GAME_RES_HEIGHT, //Width And Height Of The Source Rectangle.
        g_FrameDrawer.MemoryBuffer, //Pointer To Image Bits [ Array Of Bytes ].
        &g_FrameDrawer.BitMapInfo,  //Pointer To a BITMAPINFO.
        DIB_RGB_COLORS,     //Specifies How Colors Will Be Displayed.
        SRCCOPY     //Specifies How The Source Rectangle Will Be Copied To The Destination.
    );

    ReleaseDC(g_GameWindow, DeviceContext);
}