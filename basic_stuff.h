#ifndef BASIC_STUFF_H
#define BASIC_STUFF_H

#include <windows.h>
#include <stdio.h>


HWND CreateButton(HINSTANCE hInstance, HWND hwndParent, int x, int y, int width, int height, LPSTR str_Prompt, UINT ID_BUTTON);
HWND CreateEdit(HINSTANCE hInstance, HWND hwndParent, int x, int y, int width, int height, LPSTR str_Prompt, UINT ID_EDIT, DWORD dwStyle);
HWND CreateStatic(HINSTANCE hInstance, HWND hwndParent, int x, int y, int width, int height, LPSTR str_Prompt/* , UINT ID_STATIC */);

void make_console();

#endif