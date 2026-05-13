#include "tint_ui.h"
#include <commctrl.h>

static HWND TintCreateGroupBox(
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

static HWND TintCreateButton(
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


static HWND TintCreateStatic(
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

static void TintApplyReadOnlySlotStyle(
                                       HWND Control
                                       )
{
    LONG_PTR Style;
    LONG_PTR ExStyle;

    Style = GetWindowLongPtrW(Control, GWL_STYLE);
    ExStyle = GetWindowLongPtrW(Control, GWL_EXSTYLE);

    SetWindowLongPtrW(Control, GWL_STYLE, Style | SS_SUNKEN);
    SetWindowLongPtrW(Control, GWL_EXSTYLE, ExStyle | WS_EX_STATICEDGE);

    SetWindowPos(
                 Control,
                 NULL,
                 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED
                 );
}


void TintCreateMainLayout (
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

    TintApplyReadOnlySlotStyle(State->title_value);

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
    TintApplyReadOnlySlotStyle(State->process_value);

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

    SendMessageW(
                 State->opacity_slider,
                 TBM_SETRANGE,
                 TRUE,
                 MAKELONG(
                          TINT_MIN_OPACITY_PERCENT,
                          TINT_MAX_OPACITY_PERCENT
                          )
                 );

    SendMessageW(
                 State->opacity_slider,
                 TBM_SETPOS,
                 TRUE,
                 TINT_MAX_OPACITY_PERCENT
                 );

}
