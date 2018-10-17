#pragma once
#include "SkyeTekAPI.h"
#include "SkyeTekProtocol.h"
#include "physical.h"
#include "application.h"

extern HWND hwnd;

extern LPSKYETEK_DEVICE* devices;
extern LPSKYETEK_READER* readers;

extern int device_count;
extern int reader_count;
extern bool is_reading;