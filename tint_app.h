#ifndef TINT_APP_H
#define TINT_APP_H

#include <windows.h>
#include "tint_window_list.h"
#include "tint_opacity.h"

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
    tint_window_item windows[TINT_MAX_WINDOWS];
    int window_count;
    tint_modified_window modified_windows[TINT_MAX_MODIFIED_WINDOWS];
    int modified_window_count;
    int current_opacity_percent;
} tint_app_state;


#endif
