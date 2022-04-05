#pragma warning(push, 0)
#include <windows.h>
#pragma warning(pop)

#include "FrameRender.h"
#include "WindowConfigs.h"

extern HWND g_GameWindow;
extern GAMEBITMAP g_FrameDrawer;
extern GAME_PERF_DATA g_PerformanceData;

void RenderGameGraphics(void)
{

    PIXEL32 Pixel = { 0 };

    Pixel.Red = 0x80;
    Pixel.Green = 0x00;
    Pixel.Blue = 0x80;
    Pixel.Alpha = 0xff;

    //These Are The Top Left Coordinates Of The Tile.
    // X 0 <= 0 < 24.
    // Y 0 <= 0 < 15.
    uint16_t XTileCoordinate = 23;
    uint16_t YTileCoordinate = 14;

    //(TILE_WIDTH) * (GAME_RES_WIDTH) = Uma Linha Completa De Tiles.
    //Gets The Bottom Left Coordinate Of The Tile.
    int32_t StartReenderingCoordinateX = TILE_WIDTH * XTileCoordinate;
    int32_t StartReenderingCoordinateY = (TILE_HEIGHT * GAME_RES_WIDTH) * YTileCoordinate;
    int32_t PixelLine = 0;

    for (int32_t y = 0; y < TILE_HEIGHT; y++)
    {
        for (int32_t x = 0; x < TILE_WIDTH; x++)
        {

            memset((PIXEL32*)g_FrameDrawer.MemoryBuffer + ((StartReenderingCoordinateY + StartReenderingCoordinateX) + (PixelLine + x))
                , 0XFF, sizeof(PIXEL32));
        }

        PixelLine += GAME_RES_WIDTH;
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