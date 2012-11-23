#include "stdafx.h"
#include "errcodemsg.h"

DWORD GetVersionStringValue(const HKEY hkNetFrameworkVersions, const string subkey, string &versiontext)
{
	DWORD err = 0;
	DWORD errsp = 0;
	CRegKey keyVer;
	char vernum[100];
	ZeroMemory( vernum, sizeof(vernum));
	DWORD vernumlen = sizeof(vernum) - 1;

	// ищем внутри раздела строковый ключ с именем Version
	if( (err = keyVer.Open( hkNetFrameworkVersions, subkey.c_str(), KEY_READ )) == ERROR_SUCCESS )
	{
		// ошибка нехватки буфера ERROR_MORE_DATA = 234 никак не анализируетс€, 
		// так как верси€ длинее 99 символов - это все равно неправильна€ кака€-то верси€
		if( (err = keyVer.QueryStringValue("Version", vernum, &vernumlen)) == ERROR_SUCCESS )
			versiontext += vernum;

		DWORD spnum = 0;
		if( (errsp = keyVer.QueryDWORDValue("SP", spnum)) == ERROR_SUCCESS && spnum != 0 )
			versiontext += " SP " + boost::lexical_cast<string>(spnum);
	}
	else
	{
		// раздел есть, но открыть его не получаетс€ - фигн€ кака€-то, надо показать
		CErrCodeMsg errinfo(err);
		versiontext += errinfo.GetString();
	}

	return err;
}

string GetVersionFromKey(const HKEY hkNetFrameworkVersions, const string vernumberkey)
{
	DWORD err = 0;
	string versionvalue;

	GetVersionStringValue(hkNetFrameworkVersions, vernumberkey, versionvalue);

	return versionvalue + "\r\n";
}

string GetV4VersionFromKey(const HKEY hkNetFrameworkVersions, const string vernumberkey)
{
	string versionvalue;

	// дл€ .NET v4 есть два подключа, каждый со своей версией
	if ( GetVersionStringValue(hkNetFrameworkVersions, vernumberkey + "\\Full", versionvalue) == ERROR_SUCCESS )
		versionvalue += " Full";
	else
		if ( GetVersionStringValue(hkNetFrameworkVersions, vernumberkey + "\\Client", versionvalue)  == ERROR_SUCCESS )
			versionvalue += " Client";

	return versionvalue + "\r\n";
}

string AlignTo( string str, const DWORD align )
{
	while( str.length() < align )
		str += " ";

	return str;
}

string GetNextFrameworkVersionInfo(const HKEY hkNetFrameworkVersions, const string vernumberkey, const DWORD align)
{
	string nextver = AlignTo( vernumberkey, align ) + "- ";

	if( vernumberkey != "v4" )
		nextver += GetVersionFromKey(hkNetFrameworkVersions, vernumberkey);
	else 
		nextver += GetV4VersionFromKey(hkNetFrameworkVersions, vernumberkey);

	return nextver;
}

// все установленные версии .NET Framework перечислены в 
// HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\NET Framework Setup\NDP
// в виде отдельных разделов. ¬ каждом разделе (кроме 1.0) есть 
// строковый ключ Version с полным номером версии.
string GetNetFrameworkVersion()
{
	string allverinfo = "”становлены .Net Framework версий:\r\n";
	HKEY hkNetFrameworkVersions;
	DWORD err = 0;
	DWORD nMaxLen = 0;

	if ( (err = ::RegOpenKeyEx( HKEY_LOCAL_MACHINE,	"SOFTWARE\\Microsoft\\NET Framework Setup\\NDP",
		NULL,
		KEY_ENUMERATE_SUB_KEYS | KEY_READ ,
		&hkNetFrameworkVersions )) == ERROR_SUCCESS )
	{
		// определ€ем размер буфера под им€ подключа subKey
		::RegQueryInfoKey( hkNetFrameworkVersions, NULL, NULL, NULL, NULL, &nMaxLen, NULL, NULL, NULL, NULL, NULL, NULL );
		nMaxLen++;

		vector<char> subKey(nMaxLen);
		DWORD nSize = nMaxLen;
		DWORD KeyNumber = 0;

		// перебираем все вложенные ключи
		CRegKey keyVersions( hkNetFrameworkVersions );

		while ( (err = keyVersions.EnumKey( KeyNumber, &subKey[0], &nSize ) ) == ERROR_SUCCESS )
		{
			allverinfo += GetNextFrameworkVersionInfo(hkNetFrameworkVersions, &subKey[0], nMaxLen + 3);
			// дл€ очередной итерации
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

