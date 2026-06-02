#include "tint_opacity.h"

static tint_modified_window *TintFindModifiedWindow(
                                                    tint_modified_window *ModifiedWindows,
                                                    HWND Window
                                                    )
{
    if (ModifiedWindows == NULL || Window == NULL)
    {
        return NULL;
    }

    for (int Index = 0; Index < TINT_MAX_MODIFIED_WINDOWS; Index++)
    {
        tint_modified_window *Current = &ModifiedWindows[Index];
        if (Current->in_use && Current->hwnd == Window)
        {
            return Current;
        }
    }

    return NULL;
}


static tint_modified_window *TintFindOrCreateModifiedWindow(
                                                            tint_modified_window *ModifiedWindows,
                                                            int *ModifiedWindowCount,
                                                            HWND Window
                                                            )
{
    if (ModifiedWindows == NULL || ModifiedWindowCount == NULL || Window == NULL)
    {
        return NULL;
    }
    
    tint_modified_window *Current = TintFindModifiedWindow(ModifiedWindows, Window);

    if (Current != NULL)
    {
        return Current;
    }
    else {
        for (int Index = 0; Index < TINT_MAX_MODIFIED_WINDOWS; Index++)
        {
            tint_modified_window *Current = &ModifiedWindows[Index];
            if (!Current->in_use)
            {
                ZeroMemory(Current, sizeof(*Current));
                Current->hwnd = Window;
                Current->in_use = TRUE;
                *ModifiedWindowCount += 1;
                return Current;
            }
        }
    }

    return NULL;
}

static BOOL TintRememberModifiedWindow(
                                       tint_modified_window *ModifiedWindows,
                                       int *ModifiedWindowCount,
                                       HWND Window,
                                       LONG_PTR OriginalExStyle,
                                       BOOL OriginallyLayered,
                                       BYTE OriginalAlpha,
                                       DWORD OriginalFlags
                                       )
{
    if (ModifiedWindows == NULL || ModifiedWindowCount == NULL || Window == NULL)
    {
        return FALSE;
    }

    tint_modified_window *Current = TintFindOrCreateModifiedWindow(ModifiedWindows, ModifiedWindowCount, Window);

    if (Current == NULL)
    {
        return FALSE;
    }

    if (Current->has_opacity_snapshot)
    {
        return TRUE;
    }

    Current->original_ex_style = OriginalExStyle;
    Current->originally_layered = OriginallyLayered;
    Current->original_alpha = OriginalAlpha;
    Current->original_flags = OriginalFlags;
    Current->has_opacity_snapshot = TRUE;

    return TRUE;
}

static BOOL TintForgetModifiedWindow(
                                     tint_modified_window *ModifiedWindows,
                                     int *ModifiedWindowCount,
                                     HWND Window
                                     )
{
    tint_modified_window *Current;

    if (ModifiedWindows == NULL || ModifiedWindowCount == NULL || Window == NULL)
    {
        return FALSE;
    }

    Current = TintFindModifiedWindow(ModifiedWindows, Window);

    if (Current == NULL)
    {
        return FALSE;
    }

    ZeroMemory(Current, sizeof(*Current));

    if (*ModifiedWindowCount > 0)
    {
        *ModifiedWindowCount = *ModifiedWindowCount - 1;
    }

    return TRUE;
}

BOOL TintRestoreWindow(
                       tint_modified_window *ModifiedWindows,
                       int *ModifiedWindowCount,
                       HWND Window
                       )
{
    if (ModifiedWindows == NULL || ModifiedWindowCount == NULL || Window == NULL)
    {
        return FALSE;
    }

    tint_modified_window *Current = TintFindModifiedWindow(ModifiedWindows, Window);

    if (Current == NULL)
    {
        return FALSE;
    }

    if (IsWindow(Window))
    {
        if (Current->has_opacity_snapshot)
        {
            
            SetWindowLongPtrW(Window, GWL_EXSTYLE, Current->original_ex_style);

            if (Current->originally_layered)
            {
                SetLayeredWindowAttributes(
                                           Window,
                                           0,
                                           Current->original_alpha,
                                           Current->original_flags
                                           );
            }
        }

        if (Current->has_taskbar_snapshot)
        {
            LONG_PTR NewExStyle = GetWindowLongPtrW(Window, GWL_EXSTYLE);
            LONG_PTR TaskbarMask = WS_EX_APPWINDOW | WS_EX_TOOLWINDOW;
    
            NewExStyle &= ~TaskbarMask;
            NewExStyle |= (Current->original_taskbar_ex_style & TaskbarMask);
    
            SetWindowLongPtrW(Window, GWL_EXSTYLE, NewExStyle);
            SetWindowPos(Window, NULL, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
            Current->taskbar_is_hidden = FALSE;
        }

        if (Current->has_icon_snapshot)
        {
            SendMessageW(Window, WM_SETICON, ICON_SMALL, (LPARAM)Current->original_small_icon);
            SendMessageW(Window, WM_SETICON, ICON_BIG, (LPARAM)Current->original_big_icon);
            Current->icon_is_switched = FALSE;
        }
    }

    return TintForgetModifiedWindow(
                                    ModifiedWindows,
                                    ModifiedWindowCount,
                                    Window
                                    );
}

void TintRestoreAllWindows(
                           tint_modified_window *ModifiedWindows,
                           int *ModifiedWindowCount
                           )
{
    if (ModifiedWindows == NULL || ModifiedWindowCount == NULL)
    {
        return;
    }

    for (int Index = 0; Index < TINT_MAX_MODIFIED_WINDOWS; Index++)
    {
        tint_modified_window *Current = &ModifiedWindows[Index];

        if (Current->in_use)
        {
            TintRestoreWindow(
                              ModifiedWindows,
                              ModifiedWindowCount,
                              Current->hwnd
                              );
        }
    }
}

BOOL TintHideTaskbarToWindow(
                             tint_modified_window *ModifiedWindows,
                             int *ModifiedWindowCount,
                             HWND Window
                             )
{
    
    if (ModifiedWindows == NULL || ModifiedWindowCount == NULL || Window == NULL || !IsWindow(Window))
    {
        return FALSE;
    }

    tint_modified_window *Current = TintFindOrCreateModifiedWindow(ModifiedWindows, ModifiedWindowCount, Window);

    if (Current == NULL)
    {
        return FALSE;
    }

    LONG_PTR ExStyle = GetWindowLongPtrW(Window, GWL_EXSTYLE);

    if (!Current->has_taskbar_snapshot)
    {
        Current->original_taskbar_ex_style = ExStyle;
        Current->has_taskbar_snapshot = TRUE;
    }
    
    if (!Current->taskbar_is_hidden)
    {
        LONG_PTR NewExStyle = ExStyle;
        NewExStyle &= ~WS_EX_APPWINDOW;
        NewExStyle |= WS_EX_TOOLWINDOW;

        SetWindowLongPtrW(Window, GWL_EXSTYLE, NewExStyle);
        SetWindowPos(Window, NULL, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
        Current->taskbar_is_hidden = TRUE;
        return TRUE;
    }

    
    LONG_PTR NewExStyle = ExStyle;
    LONG_PTR TaskbarMask = WS_EX_APPWINDOW | WS_EX_TOOLWINDOW;
    
    NewExStyle &= ~TaskbarMask;
    NewExStyle |= (Current->original_taskbar_ex_style & TaskbarMask);
    
    SetWindowLongPtrW(Window, GWL_EXSTYLE, NewExStyle);
    SetWindowPos(Window, NULL, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
    Current->taskbar_is_hidden = FALSE;
    return TRUE;
}


BOOL TintSwitchIconToWindow(
                            tint_modified_window *ModifiedWindows,
                            int *ModifiedWindowCount,
                            HWND Window
                            )
{
    if (ModifiedWindows == NULL || ModifiedWindowCount == NULL || Window == NULL || !IsWindow(Window))
    {
        return FALSE;
    }
    
    tint_modified_window *Current = TintFindOrCreateModifiedWindow(ModifiedWindows, ModifiedWindowCount, Window);

    if (Current == NULL)
    {
        return FALSE;
    }

    HICON systemIcon = LoadIcon(NULL, IDI_APPLICATION);
    if (systemIcon == NULL)
    {
        return FALSE;
    }

    if (!Current->icon_is_switched)
    {
        if (!Current->has_icon_snapshot)
        {

            HICON SmallIcon = (HICON)SendMessageW(Window, WM_GETICON, ICON_SMALL2, 0);

            if (SmallIcon == NULL)
            {
                SmallIcon = (HICON)SendMessageW(Window, WM_GETICON, ICON_SMALL, 0);
            }

            HICON BigIcon = (HICON)SendMessageW(Window, WM_GETICON, ICON_BIG, 0);
            
            Current->original_small_icon = SmallIcon;
            Current->original_big_icon = BigIcon;
            Current->has_icon_snapshot = TRUE;
        }
        
        SendMessage(Window, WM_SETICON, ICON_SMALL, (LPARAM)systemIcon);
        SendMessage(Window, WM_SETICON, ICON_BIG, (LPARAM)systemIcon);

        Current->icon_is_switched = TRUE;
        return TRUE;
    }

    SendMessageW(Window, WM_SETICON, ICON_SMALL, (LPARAM)Current->original_small_icon);
    SendMessageW(Window, WM_SETICON, ICON_BIG, (LPARAM)Current->original_big_icon);

    Current->icon_is_switched = FALSE;
    return TRUE;
}

BOOL TintApplyOpacityToWindow(
                              tint_modified_window *ModifiedWindows,
                              int *ModifiedWindowCount,
                              HWND Window,
                              int Percent
                              )
{
    LONG_PTR ExStyle;
    BOOL OriginallyLayered;
    BYTE OriginalAlpha = 255;
    DWORD OriginalFlags = 0;
    BYTE Alpha;

    if (
        ModifiedWindows == NULL ||
        ModifiedWindowCount == NULL ||
        Window == NULL ||
        !IsWindow(Window))
    {
        return FALSE;
    }

    ExStyle = GetWindowLongPtrW(Window, GWL_EXSTYLE);

    OriginallyLayered = ((ExStyle & WS_EX_LAYERED) != 0);

    if (OriginallyLayered)
    {
        COLORREF ColorKey = 0;

        if (!GetLayeredWindowAttributes(
                                        Window,
                                        &ColorKey,
                                        &OriginalAlpha,
                                        &OriginalFlags
                                        ))
        {
            OriginalAlpha = 255;
            OriginalFlags = 0;
        }
    }

    if (!TintRememberModifiedWindow(
                                    ModifiedWindows,
                                    ModifiedWindowCount,
                                    Window,
                                    ExStyle,
                                    OriginallyLayered,
                                    OriginalAlpha,
                                    OriginalFlags
                                    ))
    {
        return FALSE;
    }

    if ((ExStyle & WS_EX_LAYERED) == 0)
    {
        SetWindowLongPtrW(Window, GWL_EXSTYLE, ExStyle | WS_EX_LAYERED);
    }

    Alpha = TintOpacityPercentToAlpha(Percent);

    return SetLayeredWindowAttributes(
                                      Window,
                                      0,
                                      Alpha,
                                      LWA_ALPHA
                                      );
}

int TintClampOpacityPercent(int Percent)
{
    if (Percent < TINT_MIN_OPACITY_PERCENT)
    {
        return TINT_MIN_OPACITY_PERCENT;
    }
    if (Percent > TINT_MAX_OPACITY_PERCENT)
    {
        return TINT_MAX_OPACITY_PERCENT;
    }

    return Percent;
}

BYTE TintOpacityPercentToAlpha(int Percent)
{
    int ClampedPercent = TintClampOpacityPercent(Percent);

    return (BYTE)((ClampedPercent * 255 + 50) / 100);
}
