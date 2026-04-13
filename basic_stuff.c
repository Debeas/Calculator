#include "basic_stuff.h"

#define SET_WIDTH width = max(width, (int) lstrlen(str_Prompt) * 8.);

HWND CreateButton(HINSTANCE hInstance, HWND hwndParent, int x, int y, int width, int height, LPSTR str_Prompt, UINT ID_BUTTON) {
    SET_WIDTH
    HWND hwndButton = CreateWindowEx(
        WS_EX_CLIENTEDGE,                      // Extended styles (usually 0)
        "BUTTON",               // Predefined button class
        str_Prompt,             // Button text
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles
        x, y,                   // Position relative to parent
        width, height,          // Size
        hwndParent,             // Parent window
        (HMENU)(INT_PTR)ID_BUTTON,       // Button ID (used in WM_COMMAND)
        hInstance,              // Module instance
        NULL                    // Extra params (usually NULL)
    );
    return hwndButton;
}
HWND CreateEdit(HINSTANCE hInstance, HWND hwndParent, int x, int y, int width, int height, LPSTR str_Prompt, UINT ID_EDIT, DWORD dwStyle) {
    SET_WIDTH
    HWND hwndEdit = CreateWindowEx(
        WS_EX_CLIENTEDGE,                      // Extended styles (usually 0)
        "EDIT",                 // Predefined Edit class
        str_Prompt,             // Edit text
        WS_VISIBLE | WS_CHILD | dwStyle, // Styles
        x, y,                   // Position relative to parent
        width, height,          // Size
        hwndParent,             // Parent window
        (HMENU)(INT_PTR)ID_EDIT,       // Edit ID (used in WM_COMMAND)
        hInstance,              // Module instance
        NULL                    // Extra params (usually NULL)
    );
    return hwndEdit;
}
HWND CreateStatic(HINSTANCE hInstance, HWND hwndParent, int x, int y, int width, int height, LPSTR str_Prompt/* , UINT ID_STATIC */) {
    SET_WIDTH
    HWND hwndStatic = CreateWindowEx(
        WS_EX_CLIENTEDGE,                      // Extended styles (usually 0)
        "STATIC",                              // Predefined Edit class
        str_Prompt,                            // Edit text
        WS_VISIBLE | WS_CHILD,                 // Styles
        x, y,                   // Position relative to parent
        width, height,          // Size
        hwndParent,             // Parent window
        /* (HMENU)(INT_PTR)ID_STATIC */ NULL,       // Edit ID (used in WM_COMMAND)
        hInstance,              // Module instance
        NULL                    // Extra params (usually NULL)
    );
    return hwndStatic;
}

void make_console() {
    AllocConsole();
    freopen("CONIN$",  "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
}