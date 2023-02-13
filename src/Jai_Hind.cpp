#include <Windows.h> 
#include <gdiplus.h>
#include <mmsystem.h>

#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "Gdiplus.lib")
#pragma comment(lib, "winmm.lib")

#define VK_F    0x46 
#define FLAG    101

using namespace Gdiplus;

#define EXIT_IF(cond, lpszMsg, lpszCaption)     do\
                                                {\
                                                    if((cond))\
                                                    {\
                                                        MessageBox((HWND)NULL,\
                                                                    TEXT(lpszMsg),\
                                                                    TEXT(lpszCaption),\
                                                                    MB_ICONERROR);\
                                                        ExitProcess(EXIT_FAILURE);\
                                                    }\
                                                }while(0) 

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 
void flag(PAINTSTRUCT ps, HWND hWnd, HDC hdc, int flag_increment, int ashok_chakra_increament);
void ToggleFullScreen(void);
void Ashok_Chakra(HDC hdc);

/* Global variables */ 
HWND ghWnd = NULL; 
BOOL gbFullScreen = FALSE; 
DWORD dwWindowStyle; 
int cxsize, cysize;
int flag_inc = 0,  ashok_chakra_inc = 0;
int is_audio_first = 0;

WINDOWPLACEMENT gpPreviousWindowPlacement; 

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) 
{
    static TCHAR szAppName[] = TEXT("Jai Hind"); 
    static TCHAR szWindowClassName[] = TEXT("Independence Day"); 

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);


    HBRUSH hBrush = NULL; 
    HCURSOR hCursor = NULL; 
    HICON hIcon = NULL; 
    HICON hIconSm = NULL; 
    HWND hWnd = NULL; 

    ATOM bClassRegistrationStatus = 0; 

    WNDCLASSEX wnd; 
    MSG msg; 

    ZeroMemory((void*)&wnd, sizeof(WNDCLASSEX)); 
    ZeroMemory((void*)&msg, sizeof(MSG)); 

    hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH); 
    EXIT_IF(hBrush == NULL, "Error in loading brush", "GetStockObject"); 

    hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW); 
    EXIT_IF(hCursor == NULL, "Error in Loading cursor", "LoadCursor"); 

    hIcon = LoadIcon((HINSTANCE)NULL, IDI_APPLICATION); 
    EXIT_IF(hIcon == NULL, "Error in loading icon", "LoadIcon"); 

    hIconSm = LoadIcon((HINSTANCE)NULL, IDI_APPLICATION); 
    EXIT_IF(hIconSm == NULL, "Error in loading icon", "LoadIcon"); 

    wnd.cbSize = sizeof(WNDCLASSEX); 
    wnd.cbClsExtra = 0; 
    wnd.cbWndExtra = 0; 
    wnd.hbrBackground = hBrush; 
    wnd.hCursor = hCursor; 
    wnd.hIcon = hIcon; 
    wnd.hInstance = hInstance; 
    wnd.hIconSm = hIconSm; 
    wnd.lpfnWndProc = WndProc; 
    wnd.lpszClassName = szWindowClassName; 
    wnd.lpszMenuName = NULL; 
    wnd.style = CS_HREDRAW | CS_VREDRAW; 

    bClassRegistrationStatus = RegisterClassEx(&wnd); 
    EXIT_IF(bClassRegistrationStatus == 0, "Failed to register a window class",
            "RegisterClassEx"); 
    
    hWnd = CreateWindowEx(WS_EX_APPWINDOW, szWindowClassName, szAppName, WS_OVERLAPPEDWINDOW, 
                            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
                            (HWND)NULL, (HMENU)NULL, hInstance, NULL); 
    EXIT_IF(hWnd == NULL, "Failed to create a window in memory", "CreateWindowEx"); 

    ShowWindow(hWnd, nShowCmd); 
    UpdateWindow(hWnd); 

    ghWnd = hWnd; 

    while(GetMessage(&msg, (HWND)NULL, 0U, 0U))
    {
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
    }

    Gdiplus::GdiplusShutdown(gdiplusToken);

    return (msg.wParam); 
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    void ToggleFullScreen(void); 
    char buffer[32];
    HDC hdc;
    PAINTSTRUCT ps;

    switch(uMsg)
    {
        case WM_TIMER:
            switch(wParam)
            {
                case FLAG:
                    KillTimer(hWnd, FLAG);
                    flag_inc += 5;
                    ashok_chakra_inc += 5;
                    if(flag_inc > cxsize)
                    {
                        KillTimer(hWnd, FLAG);
                        InvalidateRect(hWnd, NULL, TRUE);
                        break;
                    }
                    InvalidateRect(hWnd, NULL, TRUE);
                    SetTimer(hWnd, FLAG, 100, NULL);
                    break;
            }
            break;

        case WM_SIZE:
            cysize = HIWORD(lParam);
            cxsize = LOWORD(lParam);
            break;

        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            if(flag_inc > cxsize)
                Ashok_Chakra(hdc);
            flag(ps, hWnd, hdc, flag_inc, ashok_chakra_inc);
            EndPaint(hWnd, &ps);
            break;

        case WM_KEYDOWN: 
            switch(wParam)
            {
                case VK_ESCAPE: 
                    DestroyWindow(hWnd); 
                    break; 

                case VK_F: 
                    SetTimer(hWnd, FLAG, 500, NULL);
                    is_audio_first++;
                    if(is_audio_first == 1)
                    {
                        PlaySound((LPCSTR)"Trimmed.wav", NULL, SND_LOOP | SND_ASYNC);
                    }
                    if(gbFullScreen == FALSE)
                    {
                        ToggleFullScreen(); 
                        gbFullScreen = TRUE; 
                    }
                    else 
                    {
                        ToggleFullScreen(); 
                        gbFullScreen = FALSE; 
                    }
                    break; 
            }
            break; 

        case WM_DESTROY: 
            PostQuitMessage(EXIT_SUCCESS); 
            break; 
    }

    return (DefWindowProc(hWnd, uMsg, wParam, lParam)); 
}

void ToggleFullScreen(void)
{
    MONITORINFO mi; 
    HMONITOR hMonitor; 
    BOOL bVar; 

    if(gbFullScreen == FALSE)
    {
        dwWindowStyle = GetWindowLong(ghWnd, GWL_STYLE); 
        if(dwWindowStyle & WS_OVERLAPPEDWINDOW)
        { 
            gpPreviousWindowPlacement.length = sizeof(WINDOWPLACEMENT); 
            bVar = GetWindowPlacement(ghWnd, &gpPreviousWindowPlacement); 
            mi.cbSize = sizeof(MONITORINFO); 
            hMonitor = MonitorFromWindow(ghWnd, MONITORINFOF_PRIMARY); 
            GetMonitorInfo(hMonitor, &mi); 
            if(bVar && hMonitor) 
            {
                SetWindowLong(ghWnd, GWL_STYLE, dwWindowStyle & ~WS_OVERLAPPEDWINDOW); 
                SetWindowPos(ghWnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, 
                                mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, 
                                SWP_NOZORDER | SWP_FRAMECHANGED); 
            }
        }
        ShowCursor(FALSE); 
    }
    else
    {
        SetWindowLong(ghWnd, GWL_STYLE, dwWindowStyle & WS_OVERLAPPEDWINDOW); 
        gpPreviousWindowPlacement.length = sizeof(WINDOWPLACEMENT); 
        SetWindowPlacement(ghWnd, &gpPreviousWindowPlacement); 
        SetWindowPos(ghWnd, HWND_TOP, 0, 0, 0, 0, 
                        SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED); 
        ShowCursor(TRUE); 
    }
}

void flag(PAINTSTRUCT ps, HWND hWnd, HDC hdc, int flag_increment, int ashok_chakra_increament)
{
    Graphics graphics(hdc);

    SetDCBrushColor(hdc, RGB(255, 153, 51));
    HBRUSH brush = (HBRUSH)GetStockObject(DC_BRUSH);

    RECT rc = {0, 0, 0 + flag_increment, cysize/3};
    FillRect(hdc, &rc, brush);

    SetDCBrushColor(hdc, RGB(19, 136, 8));
    brush = (HBRUSH)GetStockObject(DC_BRUSH);

    RECT rc2 = {cxsize - flag_increment, cysize - cysize/3, cxsize, cysize};
    FillRect(hdc, &rc2, brush);

    DeleteObject(brush);
}

void Ashok_Chakra(HDC hdc)
{
    Graphics graphics(hdc);

    Image image(L"image1.jpg");

    RectF rc_img(cxsize/2 - ((cysize/3)/2) - 50, cysize/3 - 55, 400, 400);

    graphics.DrawImage(&image, rc_img);
}