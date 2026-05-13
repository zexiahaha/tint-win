#define UNICODE
#define _UNICODE
#include <windows.h>
#include <commctrl.h>
#include <wchar.h>
#include "tint_app.h"
#include "tint_ui.h"
#include "tint_view.h"


static void TintSelectWindow(
                             tint_app_state *State,
                             int SelectionIndex
                             )
{
    if (State == NULL)
    {
        return;
    }

    if (SelectionIndex < 0 || SelectionIndex >= State->window_count)
    {
        TintSetSelectionText(State, L"(none)", L"(none)");
        TintSetStatusText(State, L"Status: No selection");
        return;
    }

    if (!IsWindow(State->windows[SelectionIndex].hwnd))
    {
        TintSetSelectionText(State, L"(none)", L"(none)");
        TintSetStatusText(State, L"Status: Selected window no longer exists; refresh the list");
        return;
    }

    wchar_t buffer[256];
    swprintf_s(buffer, 256,  L"Status: Selected %s",  State->windows[SelectionIndex].process_name);

    TintSetSelectionText(State, State->windows[SelectionIndex].title, State->windows[SelectionIndex].process_name);
    TintSetStatusText(State, buffer);

}

static tint_window_item *TintGetSelectedWindow(tint_app_state *State)
{
    LRESULT SelectionIndex;

    if (State == NULL)
    {
        return NULL;
    }

    SelectionIndex = SendMessageW(
                                  State->windows_list,
                                  LB_GETCURSEL,
                                  0,
                                  0
                                  );

    if (SelectionIndex == LB_ERR || SelectionIndex < 0 || SelectionIndex >= State->window_count)
    {
        return NULL;
    }

    if (!IsWindow(State->windows[SelectionIndex].hwnd))
    {
        TintSetStatusText(State, L"Status: Selected window no longer exists; refresh the list");
        return NULL;
    }

    return &State->windows[SelectionIndex];
}


static void TintSetOpacityFromSlider(
                                     tint_app_state *State
                                     )
{
    wchar_t StatusText[64];
    int Percent;
    tint_window_item *SelectedWindow;
    
    if (State == NULL)
    {
        return;
    }
    
    Percent = (int)SendMessageW(
                                State->opacity_slider,
                                TBM_GETPOS,
                                0,
                                0
                                );
    
    TintSetOpacityPercent(State, Percent);

    SelectedWindow = TintGetSelectedWindow(State);
    if (SelectedWindow == NULL)
    {
        TintSetStatusText(State, L"Status: Select a window first");
        return;
    }

    if (!TintApplyOpacityToWindow(
                                  State->modified_windows,
                                  &State->modified_window_count,
                                  SelectedWindow->hwnd,
                                  Percent))
    {
        TintSetStatusText(State, L"Status: Failed to apply opacity");
        return;
    }

    wsprintfW(StatusText, L"Status: Opacity %d%%", Percent);
    TintSetStatusText(State, StatusText);
}


static void TintLoadRealWindows(tint_app_state *State)
{
    int Index = 0;
    int SelectionIndex = 0;
    int SelectionToUse = 0;
    HWND PreviousSelectedWindow = NULL;

    if (State == NULL)
    {
        return;
    }

    SelectionIndex = (int)SendMessageW(
                                       State->windows_list,
                                       LB_GETCURSEL,
                                       0,
                                       0
                                       );

    if (SelectionIndex != LB_ERR &&
        SelectionIndex >= 0 &&
        SelectionIndex < State->window_count)
    {
        PreviousSelectedWindow = State->windows[SelectionIndex].hwnd;
    }

    SendMessageW(
                 State->windows_list,
                 LB_RESETCONTENT,
                 0,
                 0
                 );

    ZeroMemory(State->windows, sizeof(State->windows));
    State->window_count = TintLoadWindowItems(
                                              State->main_window,
                                              State->windows,
                                              TINT_MAX_WINDOWS
                                              );

    for (Index = 0; Index < State->window_count; ++Index)
    {
        wchar_t Line[600];
        wsprintfW(
                  Line,
                  L"%s - %s",
                  State->windows[Index].title,
                  State->windows[Index].process_name
                  );

        SendMessageW(
                     State->windows_list,
                     LB_ADDSTRING,
                     0,
                     (LPARAM)Line
                     );
    }

    SelectionToUse = 0;

    if (PreviousSelectedWindow != NULL)
    {
        for (Index = 0; Index < State->window_count; Index++)
        {
            if (State->windows[Index].hwnd == PreviousSelectedWindow)
            {
                SelectionToUse = Index;
                break;
            }
        }
    }

    if (State->window_count > 0)
    {
        SendMessageW(State->windows_list, LB_SETCURSEL, SelectionToUse, 0);
        TintSelectWindow(State, SelectionToUse);
        TintSetStatusText(State, L"Status: Window list refreshed");
    }
    else
    {
        TintSetSelectionText(State, L"(none)", L"(none)");
        TintSetStatusText(State, L"Status: No windows found");
    }
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
            tint_app_state *State = (tint_app_state *)GetWindowLongPtrW(Window, GWLP_USERDATA);

            if (State != NULL)
            {
                TintRestoreAllWindows(State->modified_windows, &State->modified_window_count);

                SetWindowLongPtrW(Window, GWLP_USERDATA, 0);

                HeapFree(
                         GetProcessHeap(),
                         0,
                         State
                         );
            }
            
            PostQuitMessage(0);
            break;
        }
        case WM_COMMAND:
        {
            tint_app_state *State = (tint_app_state *)GetWindowLongPtrW(Window, GWLP_USERDATA);
            if (LOWORD(WParam) == IDC_WINDOWS_LIST && HIWORD(WParam) == LBN_SELCHANGE)
            {
                
                if (State != NULL)
                {
                    
                    int SelectionIndex = (int)SendMessageW(
                                                           State->windows_list,
                                                           LB_GETCURSEL,
                                                           0,
                                                           0
                                                           );
                    TintSelectWindow(State, SelectionIndex);
                }
            }
            else if (LOWORD(WParam) == IDC_REFRESH_BUTTON)
            {
                
                if (State != NULL)
                {
                    TintLoadRealWindows(State);
                }
            }
            else if (LOWORD(WParam) == IDC_RESTORE_CURRENT_BUTTON)
            {
                if (State != NULL)
                {
                    tint_window_item *SelectedWindow = TintGetSelectedWindow(State);
                    if (SelectedWindow == NULL)
                    {
                        TintSetStatusText(State, L"Status: No window selected");
                        break;
                    }

                    BOOL RestoreStatus = TintRestoreWindow(
                                                           State->modified_windows,
                                                           &State->modified_window_count,
                                                           SelectedWindow->hwnd
                                                           );
                    if (RestoreStatus)
                    {
                        TintSetStatusText(State, L"Status: Restored selected window");
                    }
                    else
                    {
                        TintSetStatusText(State, L"Status: Nothing to restore for selected window");
                    }
                }
            }
            else if (LOWORD(WParam) == IDC_RESTORE_ALL_BUTTON)
            {
                if (State != NULL)
                {
                    TintRestoreAllWindows(State->modified_windows, &State->modified_window_count);
                    TintSetStatusText(State, L"Status: Restored all modified windows");
                }
            }
            break;
        }
        case WM_HSCROLL:
        {
            tint_app_state *State = (tint_app_state *)GetWindowLongPtrW(Window, GWLP_USERDATA);
            if (State != NULL && (HWND)LParam == State->opacity_slider)
            {
                TintSetOpacityFromSlider(State);
            }
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

            
            TintSetSelectionText(
                                 State,
                                 L"(none)",
                                 L"(none)"
                                 );
            TintSetOpacityPercent(
                                  State,
                                  TINT_MAX_OPACITY_PERCENT
                                  );
            TintSetStatusText(State, L"Status: Ready");

            TintLoadRealWindows(State);
            
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

int WINAPI wWinMain(
                    HINSTANCE MainHInstance,
                    HINSTANCE MainHInstancePrev,
                    PWSTR cmdline,
                    int cmdshow
                    )
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
