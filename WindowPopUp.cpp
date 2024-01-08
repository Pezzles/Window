// you_been_bhaved.cpp : Defines the entry point for the application.
// animate it, hide taskbar icons, add a time limit
// We're down to line 232

#include "framework.h"
#include "you_been_bhaved.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // the title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
BITMAP bmp;                                     // our bitmap
HBITMAP hBitmap;                                // handle for our bitmap
NOTIFYICONDATA notifyIconData;                  // Data structure to handle the programs taskbar icon 
BOOL programRunning = true;
const int programDuration = 10000;              // 10 seconds in milliseconds

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
// Forward declarations of my functions:
HBITMAP             LoadImageFromFile(LPCWSTR filename);
void                DisplayImage(HDC hDC, HBITMAP hBitmap, RECT clientRect);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    hBitmap = LoadImageFromFile((LPCWSTR)L"Bhav.bmp");
    if (hBitmap == NULL) {
        MessageBox(NULL, L"Failed to load image", L"Error", MB_OK | MB_ICONERROR);
    }

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_YOUBEENBHAVED, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg;
    DWORD startTime = GetTickCount64();
    // Main message loop:
    while (programRunning) {

        if (GetTickCount64() - startTime >= programDuration) {
            programRunning = false;
            break;
        }

        {

            TranslateMessage(&msg);
            DispatchMessage(&msg);

        }
    }
    return (int) msg.wParam;
}


//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex{};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_YOUBEENBHAVED));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   // Initialising XY variables
   int desktopWidth;
   int desktopHeight;

   int windowXPosition;
   int windowYPosition;

   int windowWidth = 280;
   int windowHeight = 300;
   
   // Handle for the desktop window
   RECT desktop;
   HWND hDesktop = GetDesktopWindow();

   GetWindowRect(hDesktop, &desktop);

   // Top left co-ordinates will be 0, 0
   // Bottom right co-ordinates will be desktopWidth, desktopHeight
   desktopWidth = desktop.right;
   desktopHeight = desktop.bottom;

   // Position image in the bottom right corner:
   windowXPosition = desktop.right - windowWidth;
   windowYPosition = desktop.bottom - windowHeight;


   // Create window with specified dimensions
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_POPUP,
       windowXPosition, windowYPosition, windowWidth, windowHeight, nullptr, nullptr, hInstance, nullptr);

   //Set the window to always be on top
   SetWindowPos(hWnd, HWND_TOPMOST, windowXPosition, windowYPosition, windowWidth , windowHeight, SWP_SHOWWINDOW);
   
   SetWindowLongPtr(hWnd, GWL_EXSTYLE, GetWindowLongPtr(hWnd, GWL_EXSTYLE) | WS_EX_TOOLWINDOW);

   notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
   notifyIconData.hWnd = hWnd;
   notifyIconData.uID = 1;
   notifyIconData.uFlags = NIF_ICON;

   Shell_NotifyIcon(NIM_ADD, &notifyIconData);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);
            DisplayImage(hdc, hBitmap, clientRect);
            
            // Check if the image is being rendered within the boundaries of the window
            RECT imageRect = { 0, 0, bmp.bmWidth, bmp.bmHeight };
            if (!IsRectEmpty(&imageRect) && 
                !IsRectEmpty(&clientRect) &&
                !IntersectRect(&imageRect, &imageRect, &clientRect)) {
                MessageBox(NULL, L"Image is outside the window boundaries", L"Error", MB_OK | MB_ICONERROR);
                return FALSE;
            }
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        DeleteObject(hBitmap);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


// Function to load image:
HBITMAP LoadImageFromFile(LPCWSTR filename) {
    return (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

// Function to display the image:
void DisplayImage(HDC hdc, HBITMAP hBitmap, RECT clientRect) {
    HDC hMemDC = CreateCompatibleDC(hdc);
    SelectObject(hMemDC, hBitmap);
    GetObject(hBitmap, sizeof(BITMAP), &bmp);

    int imageWidth = bmp.bmWidth;
    int imageHeight = bmp.bmHeight;

    // New width & hight to the fit the window
    int newWidth = clientRect.right - clientRect.left;
    int newHeight = clientRect.bottom - clientRect.top;

    double aspectRatio = (double)imageWidth / (double)imageHeight;
    
    // New width & height while maintaining the aspect ratio calculated above
    if (newWidth / aspectRatio > newHeight) {
        newWidth = (int)(newHeight * aspectRatio);
    }
    else {
        newHeight = (int)(newWidth / aspectRatio);
    }

    HBITMAP hResizedBitmap = CreateCompatibleBitmap(hdc, newWidth, newHeight);
    HDC hResizedDC = CreateCompatibleDC(hdc);

    SelectObject(hResizedDC, hResizedBitmap);

    // Temporarily change the viewport to match the new dimensions
    SetViewportExtEx(hResizedDC, newWidth, newHeight, NULL);
    SetWindowExtEx(hResizedDC, imageWidth, imageHeight, NULL);
    

    StretchBlt(hResizedDC, 0, 0, newWidth, newHeight,
        hMemDC, 0, 0, imageWidth, imageHeight, SRCCOPY);

    SetStretchBltMode(hdc, HALFTONE);


    // Position to centre the image within the window
    int x = (clientRect.right - clientRect.left - newWidth) / 2;
    int y = (clientRect.bottom - clientRect.top - newHeight) / 2;


    BitBlt(hdc, x, y, newWidth, newHeight, hResizedDC, 0, 0, SRCCOPY);

    DeleteDC(hResizedDC);
    DeleteObject(hResizedBitmap);
    DeleteDC(hMemDC);
}
