#include <windows.h>


LRESULT Win32MainWindowCallback(
                                HWND Window,
                                UINT MEssage,
                                WPARAM WParam,
                                LPARAM LParam,
                                ) {
                                    LRESULT Result = 0;
                                    return Result;
                                }

int WinMain(HINSTANCE MainHInstance, HINSTANCE MainHInstancePrev, PSTR cmdline, int cmdshow)
{
    WNDCLASS WndClass = {};
    WndClass.lpfnWndProc = WindowProc;
    WndClass.hInstance = MainHInstance;
    WndClass.lpszClassName = "TintWindowClass";
    
    return 0;
}
