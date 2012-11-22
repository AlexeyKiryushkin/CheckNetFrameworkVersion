#include "stdafx.h"
#include "errcodemsg.h"

string GetNextFrameworkVersionInfo(HKEY hkNetFrameworkVersions, string vernumber)
{
	CRegKey keyCurrentVer;
	DWORD err = 0;
	string nextver = vernumber + "\t- Version: ";
	char vernum[100];
	ZeroMemory( vernum, sizeof(vernum));
	DWORD vernumlen = sizeof(vernum) - 1;

	if( vernumber != "v4" )
	{
		// ���� ������ ������� ��������� ���� � ������ Version
		if( (err = keyCurrentVer.Open( hkNetFrameworkVersions, vernumber.c_str(), KEY_READ )) == ERROR_SUCCESS )
		{

			if( (err = keyCurrentVer.QueryStringValue("Version", vernum, &vernumlen)) == ERROR_SUCCESS )
			{
				nextver += vernum;
				nextver += "\r\n";
			}
			else
				nextver += "�����.\r\n";
		}
		else
		{
			// ������ ����, �� ������� ��� �� ���������� - ����� �����-��
			CErrCodeMsg errinfo(err);
			nextver += errinfo.GetString();
		}
	}
	else 
	{
		// vernumber == "v4" - ���� � ���������
		CRegKey keyV4Vers;
		string v4vernumber = vernumber + "\\Full";
		ZeroMemory( vernum, sizeof(vernum));
		// ����� ����������� ���������� ������� QueryStringValue � vernumlen ����� 0!
		vernumlen = sizeof(vernum) - 1;

		if( (err = keyV4Vers.Open( hkNetFrameworkVersions, v4vernumber.c_str(), KEY_READ )) == ERROR_SUCCESS )
		{
			if( (err = keyV4Vers.QueryStringValue("Version", vernum, &vernumlen)) == ERROR_SUCCESS )
			{
				nextver += vernum;
				nextver += " Full\r\n";
			}
			else
				nextver += "�����.\r\n";
		}
		else // ���������� Full ����, ���� � Client
		{
			v4vernumber = vernumber + "\\Client";
			if( (err = keyV4Vers.Open( hkNetFrameworkVersions, v4vernumber.c_str(), KEY_READ )) == ERROR_SUCCESS )
			{
				ZeroMemory( vernum, sizeof(vernum));
				// ����� ����������� ���������� ������� QueryStringValue � vernumlen ����� 0!
				vernumlen = sizeof(vernum) - 1;
				if( (err = keyV4Vers.QueryStringValue("Version", vernum, &vernumlen)) == ERROR_SUCCESS )
				{
					nextver += vernum;
					nextver += " Client only\r\n";
				}
				else
				{
					CErrCodeMsg errinfo(err);
					nextver += errinfo.GetString();
					nextver += "\r\n";
				}
			}
			else // ������� ���, v4 ����, � Full ��� Client ����
			{
				CErrCodeMsg errinfo(err);
				nextver += errinfo.GetString();
			}
		}
	}

	return nextver;
}

// ��� ������������� ������ .NET Framework ����������� � 
// HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\NET Framework Setup\NDP
// � ���� ��������� ��������. � ������ ������� (����� 1.0) ���� 
// ��������� ���� Version � ������ ������� ������.
string GetNetFrameworkVersion()
{
	string allverinfo = "����������� .Net Framework ������:\r\n";
	HKEY hkNetFrameworkVersions;
	DWORD err = 0;
	DWORD nMaxLen = 0;

	if ( (err = ::RegOpenKeyEx( HKEY_LOCAL_MACHINE,	"SOFTWARE\\Microsoft\\NET Framework Setup\\NDP",
		NULL,
		KEY_ENUMERATE_SUB_KEYS | KEY_READ ,
		&hkNetFrameworkVersions )) == ERROR_SUCCESS )
	{
		// ���������� ������ ������ ��� ��� �������� subKey
		::RegQueryInfoKey( hkNetFrameworkVersions, NULL, NULL, NULL, NULL, &nMaxLen, NULL, NULL, NULL, NULL, NULL, NULL );
		nMaxLen++;

		vector<char> subKey(nMaxLen);
		DWORD nSize = nMaxLen;
		DWORD KeyNumber = 0;

		// ���������� ��� ��������� �����
		CRegKey keyVersions( hkNetFrameworkVersions );

		while ( (err = keyVersions.EnumKey( KeyNumber, &subKey[0], &nSize ) ) == ERROR_SUCCESS )
		{
			allverinfo += GetNextFrameworkVersionInfo(hkNetFrameworkVersions, &subKey[0]);
			// ��� ��������� ��������
			KeyNumber++;
			nSize = nMaxLen;
		}
	}
	else // ::RegOpenKeyEx error
	{
		CErrCodeMsg errinfo(err);
		allverinfo = errinfo.GetString();
	}

	return allverinfo;
}

