#define UNICODE
#define _UNICODE
#include <windows.h>
#include <commctrl.h>

enum ControlIds
{
    IDC_GROUP_WINDOWS = 2001,
    IDC_WINDOWS_LIST = 2002,
    IDC_REFRESH_BUTTON = 2003,
    IDC_GROUP_SELECTION = 2004,
    IDC_SELECTION_TITLE_LABEL = 2005,
    IDC_SELECTION_TITLE_VALUE = 2006,
    IDC_SELECTION_PROCESS_LABEL = 2007,
    IDC_SELECTION_PROCESS_VALUE = 2008,
    IDC_GROUP_OPACITY = 2009,
    IDC_OPACITY_VALUE = 2010,
    IDC_OPACITY_SLIDER = 2011,
    IDC_GROUP_ACTIONS = 2012,
    IDC_RESTORE_CURRENT_BUTTON = 2013,
    IDC_RESTORE_ALL_BUTTON = 2014,
    IDC_STATUS_BAR = 2015,
};

#define TINT_WINDOW_WIDTH 760
#define TINT_WINDOW_HEIGHT 520
#define TINT_MARGIN 12
#define TINT_GAP 12
#define TINT_LEFT_PANEL_WIDTH 410
#define TINT_RIGHT_PANEL_WIDTH 300
#define TINT_BUTTON_HEIGHT 28
#define TINT_STATUS_HEIGHT 24
#define TINT_MIN_OPACITY_PERCENT 20
#define TINT_MAX_OPACITY_PERCENT 100

typedef struct tint_app_state
{
    HWND main_window;
    HWND windows_group;
    HWND windows_list;
    HWND refresh_button;
    HWND selection_group;
    HWND title_value;
    HWND process_value;
    HWND opacity_group;
    HWND opacity_value;
    HWND opacity_slider;
    HWND actions_group;
    HWND restore_current_button;
    HWND restore_all_button;
    HWND status_bar;
    int current_opacity_percent;
} tint_app_state;

static HWND
TintCreateGroupBox(
                   HWND Parent,
                   HINSTANCE Instance,
                   int Id,
                   LPCWSTR Title,
                   int X,
                   int Y,
                   int Width,
                   int Height
                   )
{
    return CreateWindowExW(
                           0,
                           WC_BUTTONW,
                           Title,
                           WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
                           X, Y, Width, Height,
                           Parent,
                           (HMENU) (INT_PTR)Id,
                           Instance,
                           NULL
                           );
}

static HWND
TintCreateButton(
                 HWND Parent,
                 HINSTANCE Instance,
                 int Id,
                 LPCWSTR Text,
                 int X,
                 int Y,
                 int Width,
                 int Height
                 )
{
    return CreateWindowExW(
                           0,
                           WC_BUTTONW,
                           Text,
                           WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON,
                           X, Y, Width, Height,
                           Parent,
                           (HMENU) (INT_PTR) Id,
                           Instance,
                           NULL
                           );
}

static HWND
TintCreateStatic(
                 HWND Parent,
                 HINSTANCE Instance,
                 int Id,
                 LPCWSTR Text,
                 DWORD Style,
                 int X,
                 int Y,
                 int Width,
                 int Height
                 )
{
    return CreateWindowExW(
                           0,
                           WC_STATICW,
                           Text,
                           WS_CHILD | WS_VISIBLE | Style,
                           X, Y, Width, Height,
                           Parent,
                           (HMENU) (INT_PTR) Id,
                           Instance,
                           NULL
                           );
}

static void
TintCreateMainLayout (
                      HWND Window,
                      HINSTANCE Instance,
                      tint_app_state *State
                      )
{
    int client_left = TINT_MARGIN;
    int client_top = TINT_MARGIN;
    int left_x = client_left;
    int right_x = left_x + TINT_LEFT_PANEL_WIDTH + TINT_GAP;
    int top_y = client_top;

    State->windows_group = TintCreateGroupBox(
                                              Window,
                                              Instance,
                                              IDC_GROUP_WINDOWS,
                                              L"Windows",
                                              left_x,
                                              top_y,
                                              TINT_LEFT_PANEL_WIDTH,
                                              430
                                              );

    State->windows_list = CreateWindowExW(
                                          WS_EX_CLIENTEDGE,
                                          WC_LISTBOXW,
                                          L"",
                                          WS_CHILD | WS_VISIBLE | WS_TABSTOP | LBS_NOTIFY | WS_VSCROLL | WS_BORDER,
                                          left_x + 12,
                                          top_y + 22,
                                          TINT_LEFT_PANEL_WIDTH - 24,
                                          350,
                                          Window,
                                          (HMENU) (INT_PTR)IDC_WINDOWS_LIST,
                                          Instance,
                                          NULL
                                          );

    State->refresh_button = TintCreateButton(
                                             Window,
                                             Instance,
                                             IDC_REFRESH_BUTTON,
                                             L"Refresh List",
                                             left_x + 12,
                                             top_y + 382,
                                             120,
                                             TINT_BUTTON_HEIGHT
                                             );

    State->selection_group = TintCreateGroupBox(
                                                Window,
                                                Instance,
                                                IDC_GROUP_SELECTION,
                                                L"Selection",
                                                right_x,
                                                top_y,
                                                TINT_RIGHT_PANEL_WIDTH,
                                                110
                                                );

    TintCreateStatic(
                     Window,
                     Instance,
                     IDC_SELECTION_TITLE_LABEL,
                     L"Title",
                     0,
                     right_x + 12,
                     top_y + 28,
                     48,
                     20
                     );

    State->title_value = TintCreateStatic(
                                          Window,
                                          Instance,
                                          IDC_SELECTION_TITLE_VALUE,
                                          L"(none)",
                                          SS_LEFTNOWORDWRAP,
                                          right_x + 70,
                                          top_y + 28,
                                          206,
                                          20
                                          );

    TintCreateStatic(
                     Window,
                     Instance,
                     IDC_SELECTION_PROCESS_LABEL,
                     L"Process",
                     0,
                     right_x + 12,
                     top_y + 58,
                     48,
                     20
                     );

    State->process_value = TintCreateStatic(
                                            Window,
                                            Instance,
                                            IDC_SELECTION_PROCESS_VALUE,
                                            L"(none)",
                                            SS_LEFTNOWORDWRAP,
                                            right_x + 70,
                                            top_y + 58,
                                            206,
                                            20
                                            );

    State->opacity_group = TintCreateGroupBox(
                                              Window,
                                              Instance,
                                              IDC_GROUP_OPACITY,
                                              L"Opacity",
                                              right_x,
                                              top_y + 122,
                                              TINT_RIGHT_PANEL_WIDTH,
                                              110
                                              );

    TintCreateStatic(
                     Window,
                     Instance,
                     0,
                     L"Current Opacity",
                     0,
                     right_x + 12,
                     top_y + 150,
                     100,
                     20
                     );

    State->opacity_value = TintCreateStatic(
                                            Window,
                                            Instance,
                                            IDC_OPACITY_VALUE,
                                            L"100%",
                                            SS_CENTER,
                                            right_x + 214,
                                            top_y + 146,
                                            58,
                                            22
                                            );

    State->opacity_slider = CreateWindowExW(
                                            0,
                                            TRACKBAR_CLASSW,
                                            L"",
                                            WS_CHILD | WS_VISIBLE | WS_TABSTOP | TBS_AUTOTICKS,
                                            right_x + 12,
                                            top_y + 178,
                                            264,
                                            30,
                                            Window,
                                            (HMENU) (INT_PTR) IDC_OPACITY_SLIDER,
                                            Instance,
                                            NULL
                                            );

    State->actions_group = TintCreateGroupBox(
                                              Window,
                                              Instance,
                                              IDC_GROUP_ACTIONS,
                                              L"Actions",
                                              right_x,
                                              top_y + 244,
                                              TINT_RIGHT_PANEL_WIDTH,
                                              88
                                              );

    State->restore_current_button = TintCreateButton(
                                                     Window,
                                                     Instance,
                                                     IDC_RESTORE_CURRENT_BUTTON,
                                                     L"Restore Current",
                                                     right_x + 12,
                                                     top_y + 272,
                                                     128,
                                                     TINT_BUTTON_HEIGHT
                                                     );

    State->restore_all_button = TintCreateButton(
                                                 Window,
                                                 Instance,
                                                 IDC_RESTORE_ALL_BUTTON,
                                                 L"Restore All",
                                                 right_x + 148,
                                                 top_y + 272,
                                                 128,
                                                 TINT_BUTTON_HEIGHT
                                                 );

    State->status_bar = CreateWindowExW(
                                       0,
                                       STATUSCLASSNAMEW,
                                       L"Status: Ready",
                                       WS_CHILD | WS_VISIBLE,
                                       0, 0, 0, 0,
                                       Window,
                                       (HMENU) (INT_PTR)IDC_STATUS_BAR,
                                       Instance,
                                       NULL
                                       );
}


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
        case WM_CREATE:
        {
            CREATESTRUCTW *Create = (CREATESTRUCTW *)LParam;
            HINSTANCE Instance = Create->hInstance;
            tint_app_state *State = (tint_app_state *)HeapAlloc(
                                                                GetProcessHeap(),
                                                                HEAP_ZERO_MEMORY,
                                                                sizeof(*State)
                                                                );

            if (State == NULL)
            {
                return -1;
            }

            State->main_window = Window;
            State->current_opacity_percent = TINT_MAX_OPACITY_PERCENT;
            SetWindowLongPtrW(Window, GWLP_USERDATA, (LONG_PTR)State);

            TintCreateMainLayout(Window, Instance, State);
            
            break;
        }
        default: {
            return DefWindowProcW(Window, Message, WParam, LParam);
        }
    }
    return Result;
}

BOOL InitCC(void)
{
    INITCOMMONCONTROLSEX CommonControlClass = {0};
    CommonControlClass.dwSize = sizeof(CommonControlClass);
    CommonControlClass.dwICC = ICC_WIN95_CLASSES;
    return InitCommonControlsEx(&CommonControlClass);
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

    
    BOOL InitCCRes = InitCC();

    if (!InitCCRes)
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
                                   TINT_WINDOW_WIDTH,
                                   TINT_WINDOW_HEIGHT,
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
