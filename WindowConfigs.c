#pragma warning(push, 0)
#include <windows.h>
#pragma warning(pop)

#include "WindowConfigs.h"

extern GAME_PERF_DATA g_PerformanceData;
extern HWND g_GameWindow;
extern BOOL g_GameIsRunning;


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

    WindowClass.cbSize = sizeof(WNDCLASSEX);
    WindowClass.style = 0;
    WindowClass.lpfnWndProc = MainWindowProc;
    WindowClass.cbClsExtra = 0;
    WindowClass.cbWndExtra = 0;
    WindowClass.hInstance = GetModuleHandle(NULL);
    WindowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WindowClass.hCursor = LoadCursor(NULL, IDI_APPLICATION);
    WindowClass.hbrBackground = CreateSolidBrush(RGB(0xDC, 0x14, 0x3C));
    WindowClass.lpszMenuName = NULL;
    WindowClass.lpszClassName = GAME_NAME "_WINDOWCLASS";
    WindowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);


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

    Mutex = CreateMutex(NULL, FALSE, GAME_NAME "_M");

    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}