#include "tint_window_list.h"

typedef struct tint_window_enum_context
{
    HWND main_window;
    tint_window_item *items;
    int max_items;
    int item_count;
} tint_window_enum_context;

static BOOL TintShouldIncludeWindow(HWND Window)
{
    wchar_t ClassName[128];

    if (Window == NULL || !IsWindowVisible(Window) || GetWindowTextLengthW(Window) == 0)
    {
        return FALSE;
    }

    ClassName[0] = L'\0';
    GetClassNameW(Window, ClassName, 128);

    if (wcscmp(ClassName, L"Shell_TrayWnd") == 0 ||
        wcscmp(ClassName, L"Shell_SecondaryTrayWnd") == 0 ||
        wcscmp(ClassName, L"Progman") == 0 ||
        wcscmp(ClassName, L"WorkerW") == 0)
    {
        return FALSE;
    }
    
    return TRUE;
}

static void TintGetProcessName(
                        DWORD ProcessId,
                        wchar_t *Buffer,
                        DWORD BufferCount
                        )
{
    if (Buffer == NULL || BufferCount == 0)
    {
        return;
    }

    Buffer[0] = L'\0';

    HANDLE process = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, ProcessId);
    if (process == NULL)
    {
        wcscpy_s(Buffer, BufferCount, L"<unknown>");
        return;
    }

    DWORD Size = BufferCount;
    if (!QueryFullProcessImageNameW(process, 0, Buffer, &Size))
    {
        wcscpy_s(Buffer, BufferCount, L"<unknown>");
    }
    else
    {
        wchar_t *FileName = wcsrchr(Buffer, L'\\');
        if (FileName != NULL && FileName[1] != L'\0')
        {
            wcscpy_s(Buffer, BufferCount, FileName + 1);
        }
    }

    CloseHandle(process);
}


static BOOL CALLBACK TintEnumWindowsCallback(
                                             HWND Window,
                                             LPARAM Param
                                             )
{
    tint_window_enum_context *Context = (tint_window_enum_context *)Param;
    DWORD ProcessId = 0;
    tint_window_item *Item = NULL;

    if (Context == NULL)
    {
        return FALSE;
    }

    if (Window == Context->main_window)
    {
        return TRUE;
    }

    if (!TintShouldIncludeWindow(Window))
    {
        return TRUE;
    }

    if (Context->item_count >= Context->max_items)
    {
        return FALSE;
    }

    Item = &Context->items[Context->item_count];
    ZeroMemory(Item, sizeof(*Item));

    Item->hwnd = Window;
    GetWindowThreadProcessId(Window, &ProcessId);
    Item->pid = ProcessId;

    GetWindowTextW(
                   Window,
                   Item->title,
                   TINT_MAX_TITLE_LENGTH
                   );

    TintGetProcessName(
                       ProcessId,
                       Item->process_name,
                       TINT_MAX_PROCESS_NAME_LENGTH
                       );

    Context->item_count += 1;

    return TRUE;
}

int TintLoadWindowItems(
                        HWND MainWindow,
                        tint_window_item *Items,
                        int MaxItems
                        )
{
    tint_window_enum_context Context;

    if (Items == NULL || MaxItems <= 0)
    {
        return 0;
    }

    ZeroMemory(&Context, sizeof(Context));
    Context.main_window = MainWindow;
    Context.items = Items;
    Context.max_items = MaxItems;
    Context.item_count = 0;

    EnumWindows(
                TintEnumWindowsCallback,
                (LPARAM)&Context
                );

    return Context.item_count;
}
