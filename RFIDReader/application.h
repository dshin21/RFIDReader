#pragma once
#include <windows.h>
#include "share.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void display_tag(HWND, LPCWSTR);
void diplay_red(HDC);
void diplay_red_off(HDC);
void diplay_green(HDC);
void diplay_green_off(HDC);

