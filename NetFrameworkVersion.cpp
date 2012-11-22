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
		// ищем внутри раздела строковый ключ с именем Version
		if( (err = keyCurrentVer.Open( hkNetFrameworkVersions, vernumber.c_str(), KEY_READ )) == ERROR_SUCCESS )
		{

			if( (err = keyCurrentVer.QueryStringValue("Version", vernum, &vernumlen)) == ERROR_SUCCESS )
			{
				nextver += vernum;
				nextver += "\r\n";
			}
			else
				nextver += "неизв.\r\n";
		}
		else
		{
			// раздел есть, но открыть его не получается - фигня какая-то
			CErrCodeMsg errinfo(err);
			nextver += errinfo.GetString();
		}
	}
	else 
	{
		// vernumber == "v4" - ищем в подключах
		CRegKey keyV4Vers;
		string v4vernumber = vernumber + "\\Full";
		ZeroMemory( vernum, sizeof(vernum));
		// после предыдущего неудачного запроса QueryStringValue в vernumlen будет 0!
		vernumlen = sizeof(vernum) - 1;

		if( (err = keyV4Vers.Open( hkNetFrameworkVersions, v4vernumber.c_str(), KEY_READ )) == ERROR_SUCCESS )
		{
			if( (err = keyV4Vers.QueryStringValue("Version", vernum, &vernumlen)) == ERROR_SUCCESS )
			{
				nextver += vernum;
				nextver += " Full\r\n";
			}
			else
				nextver += "неизв.\r\n";
		}
		else // подраздела Full нету, ищем в Client
		{
			v4vernumber = vernumber + "\\Client";
			if( (err = keyV4Vers.Open( hkNetFrameworkVersions, v4vernumber.c_str(), KEY_READ )) == ERROR_SUCCESS )
			{
				ZeroMemory( vernum, sizeof(vernum));
				// после предыдущего неудачного запроса QueryStringValue в vernumlen будет 0!
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
			else // странно это, v4 есть, а Full или Client нету
			{
				CErrCodeMsg errinfo(err);
				nextver += errinfo.GetString();
			}
		}
	}

	return nextver;
}

// все установленные версии .NET Framework перечислены в 
// HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\NET Framework Setup\NDP
// в виде отдельных разделов. В каждом разделе (кроме 1.0) есть 
// строковый ключ Version с полным номером версии.
string GetNetFrameworkVersion()
{
	string allverinfo = "Установлены .Net Framework версий:\r\n";
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
			allverinfo += GetNextFrameworkVersionInfo(hkNetFrameworkVersions, &subKey[0]);
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

