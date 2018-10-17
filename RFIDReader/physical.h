#pragma once
#include <windows.h>
#include "share.h"

bool connect_reader();
void read_tag(HWND);
DWORD WINAPI read_thread(LPVOID);
unsigned char read_loop_callback(const LPSKYETEK_TAG lpTag, void* user);
void exit();