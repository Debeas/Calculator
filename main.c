#include <windows.h>
#include "calculator_parse.h"
#include "basic_stuff.h"

#define GENERATE_MAZE 1999
#define MAZE_WIDTH  50
#define MAZE_HEIGHT 40
#define MAZE_OFFSET_X 10
#define MAZE_OFFSET_Y 10

#define BUTTON_0            0x2000
#define BUTTON_1            0x2001
#define BUTTON_2            0x2002
#define BUTTON_3            0x2003
#define BUTTON_4            0x2004
#define BUTTON_5            0x2005
#define BUTTON_6            0x2006
#define BUTTON_7            0x2007
#define BUTTON_8            0x2008
#define BUTTON_9            0x2009
#define BUTTON_PLUS         0x2010
#define BUTTON_MINUS        0x2011
#define BUTTON_SLASH        0x2012
#define BUTTON_ASTERIX      0x2013
#define BUTTON_CALCULATE    0x2014

#define STATIC_CALCULATOR   0x2015



#define SET_WIDTH width = max(width, (int)(lstrlen(str_Prompt) * 8));
// HWND CreateButton(HINSTANCE hInstance, HWND hwndParent, int x, int y, int width, int height, LPSTR str_Prompt, UINT ID_BUTTON);

// static map_t* Main_m = NULL;
static calculator_state_t* main_cs = NULL;
// static parser_state_t* main_ps = NULL;
// static char* calculator_string = NULL;
static HWND main_static = NULL;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void make_console();

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR     lpCmdLine,
    int       nCmdShow
) {
    make_console();

    const char CLASS_NAME[] = "MyWindowClass";

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClass(&wc)) {
        MessageBox(NULL, "Window class registration failed!", "Error",
                   MB_ICONERROR | MB_OK);
        return 1;
    }

    /* Window sized to fit the maze exactly plus a small margin */
    int win_width  = 320; /* +100 for button area */
    int win_height = 250;  /* +60  for button area */

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "Calculator",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        win_width, win_height,
        NULL, NULL,
        hInstance,
        NULL
    );

    if (!hwnd) {
        MessageBox(NULL, "Window creation failed!", "Error",
                   MB_ICONERROR | MB_OK);
        return 1;
    }

    // calculator_string = malloc(101);
    main_cs = calculator_state_create();
    // main_ps = parser_state_create();

    int x = 30;
    int y = 40;
    int x_width = 50;
    int y_height = 25;
    CreateButton(hInstance, hwnd, x,                y,                  x_width, y_height, "7",             BUTTON_7);
    CreateButton(hInstance, hwnd, x+1.25*x_width,   y,                  x_width, y_height, "8",             BUTTON_8);
    CreateButton(hInstance, hwnd, x+2.5*x_width,    y,                  x_width, y_height, "9",             BUTTON_9);
    CreateButton(hInstance, hwnd, x,                y + 1.25*y_height,  x_width, y_height, "4",             BUTTON_4);
    CreateButton(hInstance, hwnd, x+1.25*x_width,   y + 1.25*y_height,  x_width, y_height, "5",             BUTTON_5);
    CreateButton(hInstance, hwnd, x+2.5*x_width,    y + 1.25*y_height,  x_width, y_height, "6",             BUTTON_6);
    CreateButton(hInstance, hwnd, x,                y + 2.5*y_height,   x_width, y_height, "1",             BUTTON_1);
    CreateButton(hInstance, hwnd, x+1.25*x_width,   y + 2.5*y_height,   x_width, y_height, "2",             BUTTON_2);
    CreateButton(hInstance, hwnd, x+2.5*x_width,    y + 2.5*y_height,   x_width, y_height, "3",             BUTTON_3);
    CreateButton(hInstance, hwnd, x+1.25*x_width,   y + 3.75*y_height,  x_width, y_height, "0",             BUTTON_0);
    CreateButton(hInstance, hwnd, x+3*1.25*x_width, y,                  x_width, y_height, "+",             BUTTON_PLUS);
    CreateButton(hInstance, hwnd, x+3*1.25*x_width, y + 1.25*y_height,  x_width, y_height, "-",             BUTTON_MINUS);
    CreateButton(hInstance, hwnd, x+3*1.25*x_width, y +2*1.25*y_height, x_width, y_height, "*",             BUTTON_ASTERIX);
    CreateButton(hInstance, hwnd, x+3*1.25*x_width, y +3*1.25*y_height, x_width, y_height, "/",             BUTTON_SLASH);
    CreateButton(hInstance, hwnd, x, y +4*1.25*y_height, 4.75*x_width, y_height,           "CALCULATE",     BUTTON_CALCULATE);

    int edit_x = 30;
    int edit_y = 10;
    main_static = CreateStatic(hInstance, hwnd, edit_x, edit_y, 4.75*x_width, y_height, "0", STATIC_CALCULATOR);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_CREATE:
            return 0;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // if (Main_m != NULL)
            //     map_paint(hdc, Main_m, MAZE_OFFSET_X, MAZE_OFFSET_Y + 40); /* offset down past button */
            SetWindowText(main_static, main_cs->str);
            // printf("%s\n", main_cs->text);
            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case BUTTON_0:
                    calculator_button_character(main_cs, '0');
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case BUTTON_1:
                    calculator_button_character(main_cs, '1');
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case BUTTON_2:
                    calculator_button_character(main_cs, '2');
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case BUTTON_3:
                    calculator_button_character(main_cs, '3');
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case BUTTON_4:
                    calculator_button_character(main_cs, '4');
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case BUTTON_5:
                    calculator_button_character(main_cs, '5');
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case BUTTON_6:
                    calculator_button_character(main_cs, '6');
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case BUTTON_7:
                    calculator_button_character(main_cs, '7');
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case BUTTON_8:
                    calculator_button_character(main_cs, '8');
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case BUTTON_9:
                    calculator_button_character(main_cs, '9');
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case BUTTON_PLUS:
                    calculator_button_character(main_cs, '+');
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case BUTTON_MINUS:
                    calculator_button_character(main_cs, '-');
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case BUTTON_SLASH:
                    calculator_button_character(main_cs, '/');
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case BUTTON_ASTERIX:
                    calculator_button_character(main_cs, '*');
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                case BUTTON_CALCULATE:
                    printf("Calculate Stub\n");
                    double value = calculate(main_cs->str);
                    printf("Value %.2lf\n", value);
                    snprintf(main_cs->str, main_cs->len, "%.2lf", value);
                    InvalidateRect(hwnd, NULL, TRUE);
                    break;
                default:
                    break;
            }
            break; /* FIX: was missing — prevented fall-through into WM_DESTROY */
        }

        case WM_DESTROY:
            // if (Main_m != NULL) {   /* FIX: free map on exit */
            //     map_free(Main_m);
            //     Main_m = NULL;
            // }
            PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
