#pragma warning(push, 0)
#include <windows.h>
#pragma warning(pop)

#include "FrameRender.h"
#include "WindowConfigs.h"
#include "Player.h"
#include <emmintrin.h>
#include <xmmintrin.h>


extern HWND g_GameWindow;
extern GAMEBITMAP g_FrameDrawer;
extern GAME_PERF_DATA g_PerformanceData;
extern PLAYER g_Player;
uint32_t StartingRenderingPixel;
DWORD LoadFileBMP(_In_ char* FileName, _Inout_ GAMEBITMAP* GameBitmap)
{
    HANDLE FileHandle = CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    DWORD Error = ERROR_SUCCESS;
    DWORD PixelOffSet = 0;
    DWORD BMHcheck = 0;
    DWORD BytesToRead = 2;
    uint32_t NumberOfBytesRead = 0;

    //Checks If The File Could Be Opened.
    if (FileHandle == INVALID_HANDLE_VALUE)
    {
        Error = GetLastError();

        goto Exit;
    }
    
    //Read The First Two Bytes Of The File.
    if (ReadFile(FileHandle, &BMHcheck, BytesToRead, &NumberOfBytesRead, NULL) == FALSE)
    {
        Error = GetLastError();

        goto Exit;
    }

    //Check If The File Is Actually An BM File.
    if (BMHcheck != 0x4D42) //"BM" Backwards In Hex.
    {
        Error = ERROR_FILE_INVALID;

        goto Exit;
    }
    
    if (SetFilePointer(FileHandle, 0x0A, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {
        Error = INVALID_SET_FILE_POINTER;

        goto Exit;
    }

    //Finds Where The Pixel Array Starts.
    if (ReadFile(FileHandle, &PixelOffSet, sizeof(DWORD), &NumberOfBytesRead, NULL) == NULL)
    {
        Error = GetLastError();

        goto Exit;
    }


    if (SetFilePointer(FileHandle, 0x0E, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {
        Error = INVALID_SET_FILE_POINTER;

        goto Exit;
    }
    
    //Fill Up The Bitmap Header.
    if (ReadFile(FileHandle, &GameBitmap->BitMapInfo.bmiHeader, sizeof(BITMAPV5HEADER), &NumberOfBytesRead, NULL) == FALSE)
    {
        Error = GetLastError();

        goto Exit;
    }

    GameBitmap->MemoryBuffer = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, GameBitmap->BitMapInfo.bmiHeader.biSizeImage);
    if (GameBitmap->MemoryBuffer == NULL)
    {
        Error = ERROR_NOT_ENOUGH_MEMORY;
         
        goto Exit;
    }

    //Go To The Start Of The Pixel Array.
    if (SetFilePointer(FileHandle, PixelOffSet, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {
        Error = INVALID_SET_FILE_POINTER;

        goto Exit;
    }

    if (ReadFile(FileHandle, GameBitmap->MemoryBuffer, GameBitmap->BitMapInfo.bmiHeader.biSizeImage, &NumberOfBytesRead, NULL) == NULL)
    {
        Error = GetLastError();

        goto Exit;
    }

Exit:
    if (FileHandle != INVALID_HANDLE_VALUE)
    {

        CloseHandle(FileHandle);
    }

    return(Error);
}

void GetTileCoordinate(uint16_t X, uint16_t Y, uint32_t** TileCoordinate)
{
    **TileCoordinate = ((GAME_RES_WIDTH * TILE_HEIGHT) * (Y - 1)) + (TILE_WIDTH * X);
}


void DrawMainCharacter(uint16_t X, uint16_t Y, uint32_t* CharacterCoordinates, GAMEBITMAP* GameBitmap)
{
    int32_t TileOnStartingPixel = 0;
    GetTileCoordinate(X, Y, &CharacterCoordinates);
    
    uint32_t PixelLine = 0;
    int32_t MemoryOffSet = 0;
    PIXEL32 BitmapPixel = { 0 };

    for (int32_t YPixel = 0; YPixel < TILE_HEIGHT; YPixel++)
    {
        for (int32_t XPixel = 0; XPixel < TILE_WIDTH; XPixel++)
        {
            memcpy(&BitmapPixel, sizeof(PIXEL32), (PIXEL32*)GameBitmap->MemoryBuffer, sizeof(PIXEL32));

            memcpy((PIXEL32*)g_FrameDrawer.MemoryBuffer, sizeof(PIXEL32), &BitmapPixel, sizeof(PIXEL32));
        }
        PixelLine += GAME_RES_WIDTH;
    }

}

void RenderGameGraphics(void)
{

    PIXEL32 Pixel = { 0 };

    Pixel.Red = 0x80;
    Pixel.Green = 0x00;
    Pixel.Blue = 0x80;
    Pixel.Alpha = 0xff;

    //(TILE_WIDTH) * (GAME_RES_WIDTH) = One Row Of Tiles.
    //Gets The Bottom Left Coordinate Of The Tile.
    uint32_t PixelLine = 0;


    //*Gonna Change This Later*
    //Draw The WHOLE Screen.
    //All Functions Related To Drawing A Tile MUST Be Inside These Nested For Loops.
    //The 'x' And 'PixelLine' Variable Are Obligatory On All Functions.
    for (int32_t y = 0; y < TILE_HEIGHT; y++)
    {
        for (int32_t x = 0; x < TILE_WIDTH; x++)
        {
            memset((PIXEL32*)g_FrameDrawer.MemoryBuffer + (0 + x + PixelLine)
                , 0xFF, sizeof(PIXEL32));
        }
        PixelLine += GAME_RES_WIDTH;
    }

   // DrawMainCharacter(g_Player.PositionX, g_Player.PositionY, &StartingRenderingPixel, &g_Player.Sprite);
   
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