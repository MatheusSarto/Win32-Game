#include <stdio.h>

#pragma warning(push, 3)
#include <windows.h>
#pragma warning(pop)


LRESULT CALLBACK MainWindowProc(
    HWND WindowHandle,
    UINT Message,
    WPARAM wParam,
    LPARAM lParam);


DWORD CreateMainGameWindow();

INT  WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, PSTR CommandLine, INT CmdShow)
{
	UNREFERENCED_PARAMETER(PrevInstance);

	UNREFERENCED_PARAMETER(CommandLine);

	UNREFERENCED_PARAMETER(CmdShow);

    CreateMainGameWindow();

    MSG Message = { 0 };

    while (GetMessage(&Message , NULL, 0, 0) > 0)
    {
        TranslateMessage(&Message);

        DispatchMessage(&Message);
    }

	return(0);
}

LRESULT CALLBACK MainWindowProc(
    HWND WindowHandle,        // handle to window
    UINT Message,        // message identifier
    WPARAM wParam,    // first message parameter
    LPARAM lParam)    // second message parameter
{
    LRESULT Result = NULL;

    switch (Message)
    {

        default:
        {
            Result = DefWindowProc(WindowHandle, Message, wParam, lParam);
        }

    }
    return (Result);
}

DWORD CreateMainGameWindow()
{
    DWORD Result = ERROR_SUCCESS;
    
    WNDCLASSEX WindowClass = { 0 };

    HWND WindowHandle = 0;

    WindowClass.cbSize = sizeof(WNDCLASSEX);
    WindowClass.style = 0;
    WindowClass.lpfnWndProc = MainWindowProc;
    WindowClass.cbClsExtra = 0;
    WindowClass.cbWndExtra = 0;
    WindowClass.hInstance = GetModuleHandle(NULL);
    WindowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WindowClass.hCursor = LoadCursor(NULL, IDI_APPLICATION);
    WindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    WindowClass.lpszMenuName = NULL;
    WindowClass.lpszClassName = "GAME_WINDOWCLASS";
    WindowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&WindowClass))
    {
        Result = GetLastError();

        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        
        goto Exit;
    }

    WindowHandle = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        WindowClass.lpszClassName,
        "COOL LITTLE SILLY GAME",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
        NULL, NULL, GetModuleHandle(NULL), NULL);

    if (WindowHandle == NULL)
    {
        Result = GetLastError();

        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);

        goto Exit;
    }

Exit:

    return(Result);
}