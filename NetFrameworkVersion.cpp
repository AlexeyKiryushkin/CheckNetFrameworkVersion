#include "stdafx.h"
#include "errcodemsg.h"

DWORD GetVersionStringValue(const HKEY hkNetFrameworkVersions, const string subkey, string &versiontext)
{
	DWORD err = 0;
	CRegKey keyVer;
	char vernum[100];
	ZeroMemory( vernum, sizeof(vernum));
	DWORD vernumlen = sizeof(vernum) - 1;

	// ищем внутри раздела строковый ключ с именем Version
	if( (err = keyVer.Open( hkNetFrameworkVersions, subkey.c_str(), KEY_READ )) == ERROR_SUCCESS )
	{
		// ошибка нехватки буфера ERROR_MORE_DATA = 234 никак не анализируется, 
		// так как версия длинее 99 символов - это все равно неправильная какая-то версия
		if( (err = keyVer.QueryStringValue("Version", vernum, &vernumlen)) == ERROR_SUCCESS )
		{
			versiontext += vernum;
			versiontext += " ";
		}

		DWORD spnum = 0;
		if( keyVer.QueryDWORDValue("SP", spnum) == ERROR_SUCCESS && spnum != 0 )
			versiontext += "SP " + boost::lexical_cast<string>(spnum) + " ";
	}
	else
	{
		// раздел есть, но открыть его не получается - фигня какая-то, надо показать
		CErrCodeMsg errinfo(err);
		versiontext += errinfo.GetString();
	}

	return err;
}

DWORD GerVersion45ReleaseValue(const HKEY hkNetFrameworkVersions, const string subkey, string &versiontext)
{
	DWORD err = 0;
	CRegKey keyVer;
	DWORD release = 0;

	if( (err = keyVer.Open( hkNetFrameworkVersions, subkey.c_str(), KEY_READ )) == ERROR_SUCCESS )
	{
		if( (err = keyVer.QueryDWORDValue("Release", release)) == ERROR_SUCCESS )
		{
			if( release <= 378389 )
				versiontext += "( v4.5 ";
			else
			{
				if( release > 378389 && release < 379893 )
					versiontext += "( v4.5.1 ";
				else
					versiontext += "( v4.5.2 ";
			}
			versiontext += "release " + boost::lexical_cast<string>(release) + " ) ";
		}
		else
		{
			// нет поля Release - это просто 4.0, 4.5 нет никакой
		}
	}
	else
	{
		// раздел есть, но открыть его не получается - фигня какая-то, надо показать
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
	DWORD err = 0;

	// для .NET v4 есть два подключа, каждый со своей версией
	if ( GetVersionStringValue(hkNetFrameworkVersions, vernumberkey + "\\Full", versionvalue) == ERROR_SUCCESS )
	{
		// анализируем Release и уточняем номер версии
		GerVersion45ReleaseValue(hkNetFrameworkVersions, vernumberkey + "\\Full", versionvalue);

		versionvalue += "Full";
	}
	else
	{
		if ( GetVersionStringValue(hkNetFrameworkVersions, vernumberkey + "\\Client", versionvalue)  == ERROR_SUCCESS )
		{
			// анализируем Release и уточняем номер версии
			GerVersion45ReleaseValue(hkNetFrameworkVersions, vernumberkey + "\\Client", versionvalue);

			versionvalue += "Client";
		}
	}

	return versionvalue + "\r\n";
}

string GetV3VersionFromKey(const HKEY hkNetFrameworkVersions, const string vernumberkey)
{
	string versionvalue;

	// для .NET v3.0 Version может быть в корне или в подключе Setup
	if ( GetVersionStringValue(hkNetFrameworkVersions, vernumberkey, versionvalue) != ERROR_SUCCESS )
		GetVersionStringValue(hkNetFrameworkVersions, vernumberkey + "\\Setup", versionvalue);

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

	if( vernumberkey == "v4" )
		nextver += GetV4VersionFromKey(hkNetFrameworkVersions, vernumberkey);
	else 
	{
		if( vernumberkey == "v3.0" )
			nextver += GetV3VersionFromKey(hkNetFrameworkVersions, vernumberkey);
		else
			nextver += GetVersionFromKey(hkNetFrameworkVersions, vernumberkey);
	}

	return nextver;
}

// все установленные версии .NET Framework перечислены в 
// HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\NET Framework Setup\NDP
// в виде отдельных разделов. В каждом разделе (кроме 1.0) есть 
// строковый ключ Version с полным номером версии.
string GetNetFrameworkVersion()
{
	string allverinfo = "Установлены .Net Framework версий:\r\n\r\n";
	HKEY hkNetFrameworkVersions;
	DWORD err = 0;
	DWORD nMaxLen = 0;

	if ( (err = ::RegOpenKeyEx( HKEY_LOCAL_MACHINE,	"SOFTWARE\\Microsoft\\NET Framework Setup\\NDP",
		NULL,
		KEY_ENUMERATE_SUB_KEYS | KEY_READ ,
		&hkNetFrameworkVersions )) == ERROR_SUCCESS )
	{
		// определяем размер буфера под имя подключа subKey
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
			// для очередной итерации
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

