#include "tint_view.h"
#include <commctrl.h>

void TintSetSelectionText(
                          tint_app_state *State,
                          LPCWSTR Title,
                          LPCWSTR Process
                          )
{
    if (State == NULL)
    {
        return;
    }

    SetWindowTextW(State->title_value, Title);
    SetWindowTextW(State->process_value, Process);
}

void TintSetStatusText(
                       tint_app_state *State,
                       LPCWSTR Text
                       )
{
    if (State == NULL || State->status_bar == NULL)
    {
        return;
    }
    SendMessageW(State->status_bar, SB_SETTEXTW, 0, (LPARAM)Text);
}

void TintSetOpacityPercent(
                           tint_app_state *State,
                           int Percent
                           )
{
    wchar_t Buffer[16];

    if (State == NULL)
    {
        return;
    }

    State->current_opacity_percent = Percent;
    wsprintfW(Buffer, L"%d%%", Percent);
    SetWindowTextW(State->opacity_value, Buffer);
}
