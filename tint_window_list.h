#ifndef TINT_WINDOW_LIST_H
#define TINT_WINDOW_LIST_H

#include <windows.h>
#include <wchar.h>

#define TINT_MAX_WINDOWS 512
#define TINT_MAX_TITLE_LENGTH 260
#define TINT_MAX_PROCESS_NAME_LENGTH 260

typedef struct tint_window_item
{
    HWND hwnd;
    DWORD pid;
    wchar_t title[TINT_MAX_TITLE_LENGTH];
    wchar_t process_name[TINT_MAX_PROCESS_NAME_LENGTH];
} tint_window_item;


int TintLoadWindowItems(
                        HWND MainWindow,
                        tint_window_item *Items,
                        int MaxItems
                        );

#endif

