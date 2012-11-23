#include "stdafx.h"
#include "errcodemsg.h"

DWORD GetVersionStringValue(HKEY hkNetFrameworkVersions, string subkey, string &versiontext)
{
	DWORD err = 0;
	CRegKey keyVer;
	char vernum[100];
	ZeroMemory( vernum, sizeof(vernum));
	DWORD vernumlen = sizeof(vernum) - 1;

	// ���� ������ ������� ��������� ���� � ������ Version
	if( (err = keyVer.Open( hkNetFrameworkVersions, subkey.c_str(), KEY_READ )) == ERROR_SUCCESS )
	{
		// ������ �������� ������ ERROR_MORE_DATA = 234 ����� �� �������������, 
		// ��� ��� ������ ������ 99 �������� - ��� ��� ����� ������������ �����-�� ������
		if( (err = keyVer.QueryStringValue("Version", vernum, &vernumlen)) == ERROR_SUCCESS )
		{
			versiontext += vernum;
		}
		else
			// �� ���� ������ �����, ������
			versiontext += "�����.";
	}
	else
	{
		// ������ ����, �� ������� ��� �� ���������� - ����� �����-��, ���� ��������
		CErrCodeMsg errinfo(err);
		versiontext += errinfo.GetString();
	}

	return err;
}

string GetVersionFromKey(HKEY hkNetFrameworkVersions, string vernumberkey)
{
	DWORD err = 0;
	string versionvalue;

	GetVersionStringValue(hkNetFrameworkVersions, vernumberkey, versionvalue);

	return versionvalue + "\r\n";
}

string GetV4VersionFromKey(HKEY hkNetFrameworkVersions, string vernumberkey)
{
	string versionvalue;

	// ��� .NET v4 ���� ��� ��������, ������ �� ����� �������
	if ( GetVersionStringValue(hkNetFrameworkVersions, vernumberkey + "\\Full", versionvalue) == ERROR_SUCCESS )
		versionvalue += " Full";
	else
		if ( GetVersionStringValue(hkNetFrameworkVersions, vernumberkey + "\\Client", versionvalue)  == ERROR_SUCCESS )
			versionvalue += " Client";

	return versionvalue + "\r\n";
}

string AlignTo( string str, DWORD align )
{
	while( str.length() < align )
		str += " ";

	return str;
}

string GetNextFrameworkVersionInfo(HKEY hkNetFrameworkVersions, string vernumberkey, DWORD align)
{
	string nextver = AlignTo( vernumberkey, align ) + "- ";

	if( vernumberkey != "v4" )
		nextver += GetVersionFromKey(hkNetFrameworkVersions, vernumberkey);
	else 
		nextver += GetV4VersionFromKey(hkNetFrameworkVersions, vernumberkey);

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
			allverinfo += GetNextFrameworkVersionInfo(hkNetFrameworkVersions, &subKey[0], nMaxLen + 3);
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
