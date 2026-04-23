#include <windows.h>


LRESULT Win32MainWindowCallback(
                                HWND Window,
                                UINT Message,
                                WPARAM WParam,
                                LPARAM LParam
                                )
{
    LRESULT Result = 0;
    switch (Message)
    {
        case WM_CLOSE:
        {
            return 0;
            break;
        }
        case WM_SIZE:
        {
            return 0;
            break;
        }
        case WM_DESTROY:
        {
            return 0;
            break;
        }
        default: {
            return DefWindowProc(Window, Message, WParam, LParam);
        }
    }
    return Result;
}

int WinMain(HINSTANCE MainHInstance, HINSTANCE MainHInstancePrev, PSTR cmdline, int cmdshow)
{
    WNDCLASS WindowClass = {};
    WindowClass.lpfnWndProc = Win32MainWindowCallback;
    WindowClass.hInstance = MainHInstance;
    WindowClass.lpszClassName = "TintWindowClass";

    if (!RegisterClass(&WindowClass))
    {
        HWND HWindow = CreateWindowEx(
                                      0,
                                      "TintWindowClass",
                                      "tint-win",
                                      0,
                                      0,
                                      0,
                                      50,
                                      50,
                                      NULL,
                                      NULL,
                                      MainHInstance,
                                      NULL
                                      );

        if (HWindow != NULL)
        {
            
        }
    }
    
    return 0;
}
