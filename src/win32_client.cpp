#ifndef UNICODE
#if 1
#define UNICODE
#endif
#endif

#include <windows.h>
#include <stdio.h> 
#include <intrin.h> 
//#include <wingdi.h>
//#include "win32_util.cpp"

struct win32_offscreen_buffer {
    BITMAPINFO Info;
    void* Memory;
    int Width;
    int Height;
    int BytesPerPixel = 4;
};

static bool Record;
static bool Running = true;

static win32_offscreen_buffer GlobalBackBuffer;

#include "defines.h"
#include "game.cpp"

//static HBITMAP BitmapHandle;
//static HDC DeviceContext;

static void Win32UpdateWindow(win32_offscreen_buffer *Buffer, HDC hdc,RECT *WindowRect, int x, int y, int width, int height) {

    int WindowWidth = WindowRect->right - WindowRect->left;
    int WindowHeight = WindowRect->bottom - WindowRect->top; 

    StretchDIBits(
        hdc,
        // x,  y, width, height,
        // x,  y, width, height,
        0,0,WindowWidth,WindowHeight,
        0,0,Buffer->Width,Buffer->Height,        
        Buffer->Memory,
        &Buffer->Info,
        DIB_RGB_COLORS,
        SRCCOPY
    );
}

static void Win32ResizeDIBSection(win32_offscreen_buffer *Buffer, int width, int height) {
    
    // if(BitmapHandle) {
    //     DeleteObject(BitmapHandle);
    // }

    // if(!DeviceContext) {
    //     DeviceContext = CreateCompatibleDC(0);    
    // }    

    if(Buffer->Memory) {
        VirtualFree(&Buffer->Memory, 0, MEM_RELEASE);
    }

    Buffer->Width = width;
    Buffer->Height = height;

#if 1

    Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
    Buffer->Info.bmiHeader.biWidth = Buffer->Width;
    Buffer->Info.bmiHeader.biHeight = -Buffer->Height;
    Buffer->Info.bmiHeader.biPlanes = 1;
    Buffer->Info.bmiHeader.biBitCount = 32;
    Buffer->Info.bmiHeader.biCompression = BI_RGB;
    Buffer->Info.bmiHeader.biSizeImage = 0;
    Buffer->Info.bmiHeader.biXPelsPerMeter = 0;
    Buffer->Info.bmiHeader.biYPelsPerMeter = 0;
    Buffer->Info.bmiHeader.biClrUsed = 0;
    Buffer->Info.bmiHeader.biClrImportant = 0;    

    int BitMapMemorySize = Buffer->BytesPerPixel * Buffer->Width * Buffer->Height;
    Buffer->Memory = VirtualAlloc(0, BitMapMemorySize, MEM_COMMIT, PAGE_READWRITE);

    //RenderWeirdGradient(&GlobalBackBuffer, 128, 0);
    

#endif
#if 0
    HDC hScreen = GetDC(GetDesktopWindow());
    int ScreenX = GetDeviceCaps(hScreen, HORZRES);
    int ScreenY = GetDeviceCaps(hScreen, VERTRES);

    HDC hdcMem = CreateCompatibleDC (hScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, ScreenX, ScreenY);
    HGDIOBJ hOld = SelectObject(hdcMem, hBitmap);
    BitBlt(hdcMem, 0, 0, ScreenX, ScreenY, hScreen, 0, 0, SRCCOPY);
    SelectObject(hdcMem, hOld);



    int ScreenX = BitmapWidth;
    int ScreenY = BitmapHeight;

    //int ScreenX = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    //int ScreenY = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    // HDC hdc = GetDC(NULL); //get desktop window context
    // HDC hDest = CreateCompatibleDC(hdc);
    // HBITMAP hbDesktop = CreateCompatibleBitmap(hdc, ScreenX, ScreenY);
    // HGDIOBJ hOldBitmap = SelectObject(hDest, hbDesktop);
    // BitBlt(hDest, 0, 0, ScreenX, ScreenY, hdc, 0, 0, SRCCOPY);
#endif    

#if I_STOP_SUCKING_AT_WINAPI

    if(BitmapMemory) {
        VirtualFree(&BitmapMemory, 0, MEM_RELEASE);
    }

    //HDC hScreen = GetDC(GetDesktopWindow());
    HDC hScreen = GetDC(NULL);

    int ScreenWidth = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    int ScreenHeight = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    //BitmapWidth = GetDeviceCaps(hScreen, HORZRES);
    //BitmapHeight = GetDeviceCaps(hScreen, VERTRES);    
    HDC hDest = CreateCompatibleDC(hScreen);
    HBITMAP hbDesktop = CreateCompatibleBitmap(hScreen, BitmapWidth, BitmapHeight);  

    BitmapInfo.bmiHeader.biWidth = ScreenWidth;
    BitmapInfo.bmiHeader.biHeight = -ScreenWidth;

    BitMapMemorySize = BytesPerPixel * ScreenWidth * ScreenHeight;
    BitmapMemory = VirtualAlloc(0, BitMapMemorySize, MEM_COMMIT, PAGE_READWRITE);    
  
    HGDIOBJ hOld = SelectObject(hDest, hbDesktop);
    BOOL result1 = BitBlt(hDest, 0, 0, ScreenWidth, ScreenHeight, hScreen, 0, 0, SRCCOPY);    
    bool result2 = GetDIBits(hScreen, hbDesktop, 0, BitmapHeight, BitmapMemory, &BitmapInfo, DIB_RGB_COLORS);
    SelectObject(hScreen, hOld);   


    //ReleaseDC(GetDesktopWindow(),hScreen);
    ReleaseDC(NULL,hScreen);
    DeleteDC(hDest);
    DeleteObject(hbDesktop);

#endif


}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{

#if 1  
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"TestClient";

    // RECT rect;
    // GetClientRect(0, &rect);
    // LONG width = rect.right - rect.left;
    // LONG height = rect.bottom - rect.top;                                
    Win32ResizeDIBSection(&GlobalBackBuffer, 1280, 720);    

    WNDCLASS wc = {};

    wc.style = CS_HREDRAW | CS_VREDRAW; // when we get wm_paint we want to redraw whole window.
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND Window = CreateWindowEx(
        0,                           // Optional window styles.
        CLASS_NAME,                  // Window class
        L"Test Client", // Window text
        WS_OVERLAPPEDWINDOW,         // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,      // Parent window
        NULL,      // Menu
        hInstance, // Instance handle
        NULL       // Additional application data
    );

    if (Window == NULL)
    {
        return 0;
    }

    ShowWindow(Window, nCmdShow);
#if 0
        OpenFileOpenDialog();
#endif
#else

    MainWindow win;

    if (!win.Create(L"Circle", WS_OVERLAPPEDWINDOW))
    {
        return 0;
    }

    ShowWindow(win.Window(), nCmdShow);

#endif


    // Run the message loop.

    MSG msg = {};
    int xOffset = 0;
    int yOffset = 0;

    //while (GetMessage(&msg, NULL, 0, 0))

    LARGE_INTEGER PerformanceFrequency;
    QueryPerformanceFrequency(&PerformanceFrequency);

    LARGE_INTEGER LastCounter;
    QueryPerformanceCounter(&LastCounter);        

    u64 LastCycles = __rdtsc();

    while(Running) {


        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {

            if(msg.message == WM_QUIT) {
                Running = false;  
            }
            TranslateMessage(&msg);
            DispatchMessage(& msg);
        }

        offscreen_buffer Buffer;
        Buffer.Memory = GlobalBackBuffer.Memory;
        Buffer.Width = GlobalBackBuffer.Width;
        Buffer.Height = GlobalBackBuffer.Height;
        Buffer.BytesPerPixel = GlobalBackBuffer.BytesPerPixel;

        GameUpdateAndRender(&Buffer);

        //RenderWeirdGradient(&GlobalBackBuffer, xOffset, yOffset);

        RECT ClientRect;
        GetClientRect(Window, &ClientRect);        

        int WindowWidth = ClientRect.right - ClientRect.left;
        int WindowHeight = ClientRect.bottom - ClientRect.top;

        HDC DeviceContext = GetDC(Window);
        Win32UpdateWindow(&GlobalBackBuffer, DeviceContext, &ClientRect, 0, 0, WindowWidth, WindowHeight);
        ReleaseDC(Window, DeviceContext);

        xOffset++;

        LARGE_INTEGER EndCounter;
        QueryPerformanceCounter(&EndCounter);        

        u64 EndCycles = __rdtsc();

        float TotalClockTimeForFrame = EndCounter.QuadPart - LastCounter.QuadPart;
        u64 TotalCyclesForFrame = EndCycles - LastCycles;
        float ElapsedMSPerFrame = (float)(TotalClockTimeForFrame) / (float)PerformanceFrequency.QuadPart;
        u64 CyclesPerSecond = TotalCyclesForFrame / ElapsedMSPerFrame;


		wchar_t buffer1[256];
		swprintf(buffer1, 256, L"%fms %lldcycles %lldcycles/sec\n", ElapsedMSPerFrame * 1000, TotalCyclesForFrame, CyclesPerSecond);
		OutputDebugStringW(buffer1);
        
        LastCounter = EndCounter;
        LastCycles = EndCycles;

    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_QUIT:
            Running = false;
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;        
        case WM_KEYDOWN:
            if(wParam == 'R') {                
                OutputDebugStringW(L"R was pressed\n");
            }
            break;
        case WM_SIZE:
            {

            }
            break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            //FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));            
            LONG x = ps.rcPaint.left;
            LONG y = ps.rcPaint.top;
            LONG width = ps.rcPaint.right - ps.rcPaint.left;
            LONG height = ps.rcPaint.bottom - ps.rcPaint.top;

            RECT rect;
            GetClientRect(hwnd, &rect);

            Win32UpdateWindow(&GlobalBackBuffer, hdc,&rect,x,y,width,height);

#if 0            
            static DWORD operation = WHITENESS;

            if(operation == WHITENESS) {
                operation = BLACKNESS;
            }
            else {
                operation = WHITENESS;
            }
            PatBlt(hdc, x, y, width, height, operation);

#endif            
            EndPaint(hwnd, &ps);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}