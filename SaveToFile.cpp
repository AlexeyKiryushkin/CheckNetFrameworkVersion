#include "stdafx.h"
#include "rusout.h"
#include "util.h"
#include "ErrCodeMsg.h"

string SaveToFile(string info)
{
	string fname = DirectoryPathFromFilePath(GetApplicationExecutablePath()) + "\\CheckNetFrameworkVersion.txt";
	string result = "";

	ofstream f(fname, ios::out | ios::binary);

	if(f)
	{
		f << info.c_str() << endl;

		result = "���������� ��������� � ����\r\n" + fname;
	}
	else
	{
		CLastErrCodeMsg errinfo;
		result = "�� �������� ��������� ����� � ����\r\n" + fname + ":";
		result+=errinfo.GetString();
	}

	return result;
}