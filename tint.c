#include <windows.h>
#include <commctrl.h>


LRESULT CALLBACK Win32MainWindowCallback(
                                HWND Window,
                                UINT Message,
                                WPARAM WParam,
                                LPARAM LParam
                                )
{
    LRESULT Result = 0;
    switch (Message)
    {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }
        default: {
            return DefWindowProcW(Window, Message, WParam, LParam);
        }
    }
    return Result;
}

int WINAPI wWinMain(HINSTANCE MainHInstance, HINSTANCE MainHInstancePrev, PWSTR cmdline, int cmdshow)
{
    (void)MainHInstancePrev;
    (void)cmdline;
    WNDCLASSEXW WindowClass = {0};
    WindowClass.cbSize = sizeof(WindowClass);
    WindowClass.lpfnWndProc = Win32MainWindowCallback;
    WindowClass.hInstance = MainHInstance;
    WindowClass.lpszClassName = L"TintWindowClass";
    WindowClass.hCursor = LoadCursorW(NULL, IDC_ARROW);
    WindowClass.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);

    if (!RegisterClassExW(&WindowClass))
    {
        return 0;
    }
    HWND HWindow = CreateWindowExW(
                                   0,
                                   L"TintWindowClass",
                                   L"tint-win",
                                   WS_OVERLAPPEDWINDOW,
                                   CW_USEDEFAULT,
                                   CW_USEDEFAULT,
                                   800,
                                   500,
                                   NULL,
                                   NULL,
                                   MainHInstance,
                                   NULL
                                   );

    if (HWindow == NULL)
    {
        return 0;
    }

    ShowWindow(HWindow, cmdshow);
    UpdateWindow(HWindow);

    MSG Message;

    while(GetMessageW(&Message, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Message);
        DispatchMessageW(&Message);
    }
    
    return (int)Message.wParam;
}
