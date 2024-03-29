/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: application.cpp - An application that displays three buttons (START, STOP & EXIT) which controls the
--								  SkyeTek RFID Reader device and a window for displaying the RFID tag information read
--								  by the reader.
--
--
-- PROGRAM: RFIDReader
--
-- FUNCTIONS:
--			int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprevInstance, LPSTR lspszCmdParam, int nCmdShow)
--			LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
--			void display_tag(const HWND hwnd, const LPCWSTR tag)
--			void display_traffic_light(HDC hdc, int color)
--
-- DATE: October 15, 2018
--
-- REVISIONS: None
--
-- DESIGNER: Daniel Shin
--
-- PROGRAMMER: Daniel Shin
--
-- NOTES:
--		This program displays a window which consists of 5 components:
--			- START button
--			- STOP button
--			- EXIT button
--			- Traffic light UI Component
--			- RFID tag information display area
--		Furthermore, it interacts with the physical layer of the application (physical.cpp) which provides all the essential
--		functionalities that are needed to communicate with the SkyeTek RFID Reader.
----------------------------------------------------------------------------------------------------------------------*/
#include "stdafx.h"
#include "application.h"

using namespace std;

static TCHAR name[] = TEXT("RFID Reader");

HWND hwnd;
HWND start_button;
HWND stop_button;
HWND quit_button;

int device_count;
int reader_count;
int y_pos = 120;
bool is_reading = FALSE;
int rgb_r = 10;
int rgb_g = 10;
int rgb_b = 10;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hprevInstance, LPSTR lspszCmdParam, int nCmdShow) {
	MSG Msg;
	WNDCLASSEX Wcl;
	// Define a Window class
	Wcl.cbSize = sizeof(WNDCLASSEX);
	Wcl.style = 0; // default style
	Wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION); // large icon 
	Wcl.hIconSm = NULL; // use small version of large icon
	Wcl.hCursor = LoadCursor(NULL, IDC_ARROW);  // cursor style

	Wcl.lpfnWndProc = WndProc; // window function
	Wcl.hInstance = hInst; // handle to this instance
	Wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //white background
	Wcl.lpszClassName = name; // window class name

	Wcl.lpszMenuName = NULL; // no class menu 
	Wcl.cbClsExtra = 0;      // no extra memory needed
	Wcl.cbWndExtra = 0;

	// Register the class
	if (!RegisterClassEx(&Wcl))
		return 0;

	hwnd = CreateWindow(
		name, // name of window class
		name, // title 
		WS_OVERLAPPEDWINDOW, // window style - normal
		10,	// X coord
		10, // Y coord
		315, // width
		600, // height
		NULL, // no parent window
		NULL, // no menu
		hInst, // instance handle
		NULL // no additional arguments
	);

	start_button = CreateWindow(
		L"button",
		L"START",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WS_BORDER,
		0,
		0,
		100,
		50,
		hwnd,
		reinterpret_cast<HMENU>(1),
		NULL,
		NULL
	);

	stop_button = CreateWindow(
		L"button",
		L"STOP",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WS_BORDER,
		100,
		0,
		100,
		50,
		hwnd,
		reinterpret_cast<HMENU>(2),
		NULL,
		NULL);

	quit_button = CreateWindow(
		L"button",
		L"EXIT",
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WS_BORDER,
		200,
		0,
		100,
		50,
		hwnd,
		reinterpret_cast<HMENU>(3),
		NULL,
		NULL
	);

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&Msg, NULL, 0, 0)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	return Msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	auto hdc = GetDC(hwnd);

	PAINTSTRUCT paint_struct;
	const auto brush = CreateSolidBrush(RGB(0, 0, 0));
	RECT screen_rect;

	

	int button_clicked = LOWORD(wParam);

	switch (Message) {
	case WM_COMMAND:
		switch (button_clicked) {
		case 1:
			read_tag(hwnd);
			break;
		case 2:
			is_reading = false;
			Sleep(15);
			diplay_red(hdc);
			diplay_green_off(hdc);
			break;
		case 3:
			is_reading = false;
			PostQuitMessage(0);
			break;
		default: break;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &paint_struct);
		GetClientRect(hwnd, &screen_rect);
		FillRect(hdc, &screen_rect, brush);
		diplay_red(hdc);
		diplay_green_off(hdc);
		break;
	case WM_DESTROY:
		exit();
		break;
	default:
		return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

void display_tag(const HWND hwnd, const LPCWSTR tag) {
	const auto hdc = GetDC(hwnd);
	SetTextColor(hdc, RGB(rgb_r += 10, rgb_g += 5, rgb_b += 3));
	SetBkColor(hdc, RGB(0, 0, 0));
	TextOut(hdc, 60, y_pos, tag, 20);
	y_pos += 25;
	if (y_pos >= 500) y_pos = 120;
}

void diplay_red(HDC hdc) {
	auto temp = CreateSolidBrush(RGB(255, 50, 50));
	SelectObject(hdc, temp);
	Ellipse(hdc, 260, 150, 290, 180);
}

void diplay_red_off(HDC hdc) {
	auto temp = CreateSolidBrush(RGB(119, 0, 0));
	SelectObject(hdc, temp);
	Ellipse(hdc, 260, 150, 290, 180);
}

void diplay_green(HDC hdc) {
	auto temp = CreateSolidBrush(RGB(45, 201, 55));
	SelectObject(hdc, temp);
	Ellipse(hdc, 260, 90, 290, 120);
}

void diplay_green_off(HDC hdc) {
	auto temp = CreateSolidBrush(RGB(36, 119, 64));
	SelectObject(hdc, temp);
	Ellipse(hdc, 260, 90, 290, 120);
}