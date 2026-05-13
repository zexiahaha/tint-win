#ifndef TINT_OPACITY_H
#define TINT_OPACITY_H

#include <windows.h>

#define TINT_MIN_OPACITY_PERCENT 20
#define TINT_MAX_OPACITY_PERCENT 100
#define TINT_MAX_MODIFIED_WINDOWS 128

typedef struct tint_modified_window
{
    HWND hwnd;
    LONG_PTR original_ex_style;
    BOOL originally_layered;
    BYTE original_alpha;
    DWORD original_flags;
    BOOL in_use;
} tint_modified_window;

int TintClampOpacityPercent(int Percent);

BYTE TintOpacityPercentToAlpha(int Percent);

BOOL TintRestoreWindow(
                       tint_modified_window *ModifiedWindows,
                       int *ModifiedWindowCount,
                       HWND Window
                       );

void TintRestoreAllWindows(
                           tint_modified_window *ModifiedWindows,
                           int *ModifiedWindowCount
                           );

BOOL TintApplyOpacityToWindow(
                              tint_modified_window *ModifiedWindows,
                              int *ModifiedWindowCount,
                              HWND Window,
                              int Percent
                              );

#endif
