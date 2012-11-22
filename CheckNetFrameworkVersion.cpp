// CheckNetFrameworkVersion.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "errcodemsg.h"
#include "winversion.h"
#include "rusout.h"
#include "NetFrameworkVersion.h"

int _tmain(int argc, _TCHAR* argv[])
{
	cout << endl << (GetWindowsVersion() + "\r\n\r\n" + GetNetFrameworkVersion()).c_str() << endl << "Нажмите любую клавишу...";

	_getch();

	return 0;
}

