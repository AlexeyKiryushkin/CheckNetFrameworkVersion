// CheckNetFrameworkVersion.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "errcodemsg.h"
#include "winversion.h"
#include "rusout.h"
#include "NetFrameworkVersion.h"
#include "SaveToFile.h"

int _tmain(int argc, _TCHAR* argv[])
{
	string verinfo = GetWindowsVersion() + "\r\n\r\n" + GetNetFrameworkVersion();

	cout << endl << verinfo.c_str() << endl;
	
	cout << SaveToFile(verinfo).c_str() << endl << endl << endl;

	cout << "Нажмите любую клавишу...";

	_getch();

	return 0;
}

