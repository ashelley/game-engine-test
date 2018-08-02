#ifndef UNICODE
#if 1
#define UNICODE
#endif
#endif

#include <windows.h>
//#include <wingdi.h>
//#include "win32_util.cpp"

typedef unsigned char u8;
typedef unsigned int u32;

static bool Record;
static bool Running = true;

static BITMAPINFO BitmapInfo;
static void* BitmapMemory;
static int BitmapWidth;
static int BitmapHeight;
static int BytesPerPixel = 4;
//static HBITMAP BitmapHandle;
//static HDC DeviceContext;

static void RenderWeirdGradient(int XOffset, int YOffset) {
	u8 *Row = (u8 *)BitmapMemory;
	int Pitch = BytesPerPixel * BitmapWidth;

	for (int Y = 0; Y < BitmapHeight; Y++) {
		u32 *Pixel = (u32 *)Row;
		for (int X = 0; X < BitmapWidth; X++) {

			// BB GG RR xx
			// *Pixel++ = X + XOffset;//(u8)X;
			// *Pixel++ = Y + YOffset;//(u8)Y;
			// *Pixel++ = 0;
			// *Pixel++ = 0;

            u8 R = 0;
            u8 G = (X + XOffset);
            u8 B = (Y + YOffset);
            //u8 A = 255;

            u32 Color = ((B << 0) | (G <<8) | (R << 16)) /* | (A << 24)*/;
            *Pixel++ = Color;
		}
		Row += Pitch;
	}
}

static void Win32UpdateWindow(HDC hdc,RECT *WindowRect, int x, int y, int width, int height) {

    int WindowWidth = WindowRect->right - WindowRect->left;
    int WindowHeight = WindowRect->bottom - WindowRect->top; 

    StretchDIBits(
        hdc,
        // x,  y, width, height,
        // x,  y, width, height,
        0,0,BitmapWidth,BitmapHeight,
        0,0,WindowWidth,WindowHeight,
        BitmapMemory,
        &BitmapInfo,
        DIB_RGB_COLORS,
        SRCCOPY
    );
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{

#if 1  
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"TestClient";

    WNDCLASS wc = {};

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
    while(Running) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {

            if(msg.message == WM_QUIT) {
                Running = false;  
            }
            TranslateMessage(&msg);
            DispatchMessage(& msg);
        }

        RenderWeirdGradient(xOffset, yOffset);

        RECT ClientRect;
        GetClientRect(Window, &ClientRect);        

        int WindowWidth = ClientRect.right - ClientRect.left;
        int WindowHeight = ClientRect.bottom - ClientRect.top;

        HDC DeviceContext = GetDC(Window);
        Win32UpdateWindow(DeviceContext, &ClientRect, 0, 0, WindowWidth, WindowHeight);
        ReleaseDC(Window, DeviceContext);

        xOffset++;
    }

    return 0;
}

static void Win32ResizeDIBSection(int width, int height) {
    
    // if(BitmapHandle) {
    //     DeleteObject(BitmapHandle);
    // }

    // if(!DeviceContext) {
    //     DeviceContext = CreateCompatibleDC(0);    
    // }    

    if(BitmapMemory) {
        VirtualFree(&BitmapMemory, 0, MEM_RELEASE);
    }

    BitmapWidth = width;
    BitmapHeight = height;

#if 1

    BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
    BitmapInfo.bmiHeader.biWidth = BitmapWidth;
    BitmapInfo.bmiHeader.biHeight = -BitmapHeight;
    BitmapInfo.bmiHeader.biPlanes = 1;
    BitmapInfo.bmiHeader.biBitCount = 32;
    BitmapInfo.bmiHeader.biCompression = BI_RGB;
    BitmapInfo.bmiHeader.biSizeImage = 0;
    BitmapInfo.bmiHeader.biXPelsPerMeter = 0;
    BitmapInfo.bmiHeader.biYPelsPerMeter = 0;
    BitmapInfo.bmiHeader.biClrUsed = 0;
    BitmapInfo.bmiHeader.biClrImportant = 0;    

    int BitMapMemorySize = BytesPerPixel * BitmapWidth * BitmapHeight;
    BitmapMemory = VirtualAlloc(0, BitMapMemorySize, MEM_COMMIT, PAGE_READWRITE);

    RenderWeirdGradient(128, 0);
    

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
                RECT rect;
                GetClientRect(hwnd, &rect);
                LONG width = rect.right - rect.left;
                LONG height = rect.bottom - rect.top;                                
                Win32ResizeDIBSection(width, height);
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

            Win32UpdateWindow(hdc,&rect,x,y,width,height);

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