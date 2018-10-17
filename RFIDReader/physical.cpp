/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: physical.cpp - Source file that provides essential functions using the RFID Reader using the SkyeTek
--							   RFID Reader API.
--                             
-- PROGRAM: RFIDReader
--
-- FUNCTIONS: 
--		    bool connect_reader() 
--			void read_tag(HWND hwnd)
--			DWORD WINAPI read_thread(LPVOID v)
--			unsigned char read_loop_callback(const LPSKYETEK_TAG lpTag, void* user)
--			void exit()
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
--		This file contains essential functions that are required to communicate with the SkyeTek RFID Reader. The functions
--		implemented are wrapper functions of the SkyeTek API that are catered for the use of the assignment.
----------------------------------------------------------------------------------------------------------------------*/
#include "stdafx.h"
#include "physical.h"

LPSKYETEK_DEVICE* devices = nullptr;
LPSKYETEK_READER* readers = nullptr;
HANDLE reader_thread;

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: connect_reader
--
-- DATE: October 15, 2018
--
-- REVISIONS: None
--
-- DESIGNER: Daniel Shin
--
-- PROGRAMMER: Daniel Shin
--
-- INTERFACE: bool connect_reader()
--
-- RETURNS: bool; returns true if reader is found, false otherwise
--
-- NOTES:
--		This function is used scan the devices to find a SkyeTek RFID reader and returns true if the read is found.
--		Additionally, the function tracks how many devices and readers were found.
----------------------------------------------------------------------------------------------------------------------*/
bool connect_reader() {
	if ((device_count = SkyeTek_DiscoverDevices(&devices)) > 0)
		return (reader_count = SkyeTek_DiscoverReaders(devices, device_count, &readers)) > 0;
	return false;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: read_tag
--
-- DATE: October 15, 2018
--
-- REVISIONS: None
--
-- DESIGNER: Daniel Shin
--
-- PROGRAMMER: Daniel Shin
--
-- INTERFACE: void read_tag(HWND hwnd)
--
-- RETURNS: void
--
-- NOTES:
--		This function reads the RFID tag by creating a thread for reading the RFID tag if a reader is found.
----------------------------------------------------------------------------------------------------------------------*/
void read_tag(HWND hwnd) {
	auto hdc = GetDC(hwnd);
	DWORD thread{};
	is_reading = true;
	SetBkColor(hdc, RGB(0, 0, 0));
	SetTextColor(hdc, RGB(255,255,255));

	TextOut(hdc, 60, 60, L"Discovering Reader!", 20);
	if (!thread && connect_reader()) {
		reader_thread = CreateThread(nullptr, 0, read_thread, nullptr, 0, &thread);
		TextOut(hdc, 60, 85, L"Reader Found!", 13);
		diplay_red_off(hdc);
		diplay_green(hdc);
	}
	else {
		TextOut(hdc, 60, 85, L"Reader NOT Found!", 17);
	}
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: read_thread
--
-- DATE: October 15, 2018
--
-- REVISIONS: None
--
-- DESIGNER: Daniel Shin
--
-- PROGRAMMER: Daniel Shin
--
-- INTERFACE: DWORD WINAPI read_thread(LPVOID v)
--
-- RETURNS: DWORD; a pointer to a function that notifies the host that a thread has started to execute
--
-- NOTES:
--		This function constantly tries to read an RFID tag near the reader.
----------------------------------------------------------------------------------------------------------------------*/
DWORD WINAPI read_thread(LPVOID v) {
	LPSKYETEK_TAG* lp_tags = nullptr;
	auto hdc = GetDC(hwnd);
	diplay_red_off(hdc);
	diplay_green(hdc);
	while (true) 
		if (is_reading) 
			const auto status = SkyeTek_SelectTags(readers[0], AUTO_DETECT, read_loop_callback, 0, 1, nullptr);
		
	return 0;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: read_loop_callback
--
-- DATE: October 15, 2018
--
-- REVISIONS: None
--
-- DESIGNER: Daniel Shin
--
-- PROGRAMMER: Daniel Shin
--
-- INTERFACE: unsigned char read_loop_callback(const LPSKYETEK_TAG lpTag, void* user)
--
-- RETURNS: unsigned char; callback Function to call when a tag is found.  The return of this function determines when 
--          this call completes if in loop mode (0 to stop, 1 to continue)
--
-- NOTES:
--		This function is executed when the reader detects an RFID tag nearby which then calls a function to print.
----------------------------------------------------------------------------------------------------------------------*/
unsigned char read_loop_callback(const LPSKYETEK_TAG lpTag, void* user) {
	auto hdc = GetDC(hwnd);
	if (is_reading && lpTag) display_tag(hwnd, lpTag->friendly);

	return is_reading;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: exit
--
-- DATE: October 15, 2018
--
-- REVISIONS: None
--
-- DESIGNER: Daniel Shin
--
-- PROGRAMMER: Daniel Shin
--
-- INTERFACE: void exit()
--
-- RETURNS: void
--
-- NOTES:
--		This function is executed when the user selects the <EXIT> button, which then frees all the readers and devices
--		that were in use.
----------------------------------------------------------------------------------------------------------------------*/
void exit() {
	is_reading = false;
	SkyeTek_FreeReaders(readers, reader_count);
	SkyeTek_FreeDevices(devices, device_count);
	PostQuitMessage(0);
}
