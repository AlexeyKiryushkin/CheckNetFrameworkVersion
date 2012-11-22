#pragma once

#include <atlbase.h>
#include <string>

extern OSVERSIONINFOEX OSVersionInfo; // needed to distinguish Windows 9x/NT
std::string GetWindowsVersion();