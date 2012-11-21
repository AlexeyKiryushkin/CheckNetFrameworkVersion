// CheckNetFrameworkVersion.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "errcodemsg.h"
#include "autobuf.h"

OSVERSIONINFOEX OSVersionInfo; // needed to distinguish Windows 9x/NT
string GetWindowsVersion();
string GetNetFrameworkVersion();
std::basic_ostream< char, std::char_traits<char> >& operator << ( std::basic_ostream< char, std::char_traits<char> >& out, const char* str );

int _tmain(int argc, _TCHAR* argv[])
{
	cout << endl << (GetWindowsVersion() + "\r\n\r\n" + GetNetFrameworkVersion()).c_str() << endl << "Нажмите любую клавишу...";

	_getch();

	return 0;
}

/************************************************************************/
/* Русские буквы в консоли                                              */
/************************************************************************/
std::basic_ostream< char, std::char_traits<char> >& operator << ( std::basic_ostream< char, std::char_traits<char> >& out, const char* str )
{
	std::vector<char> temp ( strlen ( str ) + 1 );
	if ( ::CharToOemA ( str, &temp[0] ) )
	{
		std::string dest ( &temp[0] );
		out << dest;
	}
	return out;
}

string GetWindowsVersion()
{
	CString m_sTmp;
	BOOL IsVersionEx = FALSE;
	CAutoBuf<char> abuf;

	//версия ОС:
	ZeroMemory ( &OSVersionInfo, sizeof ( OSVERSIONINFOEX ) );
	OSVersionInfo.dwOSVersionInfoSize = sizeof ( OSVERSIONINFOEX );

	if ( ! ( IsVersionEx = ::GetVersionEx ( ( OSVERSIONINFO * ) & OSVersionInfo ) ) )
	{
		OSVersionInfo.dwOSVersionInfoSize = sizeof ( OSVERSIONINFO );

		if ( !::GetVersionEx ( ( OSVERSIONINFO * ) & OSVersionInfo ) )
		{
			CLastErrCodeMsg errinfo;
			m_sTmp.Format("Не удалось определить версию Windows: %s", errinfo.GetString());

			return m_sTmp.GetString();
		}
	}

	if ( OSVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
	{
		switch ( OSVersionInfo.dwMinorVersion )
		{

		case 0:
			m_sTmp.Format ( "Windows 95 %ld.%ld.%ld", OSVersionInfo.dwMajorVersion,
				OSVersionInfo.dwMinorVersion,
				LOWORD ( OSVersionInfo.dwBuildNumber ) );

			if ( OSVersionInfo.szCSDVersion[ 1 ] == 'C' || OSVersionInfo.szCSDVersion[ 1 ] == 'B' )
				m_sTmp += " OSR2";

			break;

		case 10:
			m_sTmp.Format ( "Windows 98 %ld.%ld.%ld", OSVersionInfo.dwMajorVersion,
				OSVersionInfo.dwMinorVersion,
				LOWORD ( OSVersionInfo.dwBuildNumber ) );

			if ( OSVersionInfo.szCSDVersion[ 1 ] == 'A' )
				m_sTmp += " SE";

			break;

		case 90:
			m_sTmp.Format ( "Microsoft Windows Millennium %ld.%ld.%ld", OSVersionInfo.dwMajorVersion,
				OSVersionInfo.dwMinorVersion,
				LOWORD ( OSVersionInfo.dwBuildNumber ) );

			break;

		default:
			m_sTmp.Format ( "Windows %ld %ld.%ld.%ld", OSVersionInfo.dwPlatformId,
				OSVersionInfo.dwMajorVersion,
				OSVersionInfo.dwMinorVersion,
				OSVersionInfo.dwBuildNumber );

			break;
		}
	}
	else
	{
		if ( OSVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT )
		{
			if ( OSVersionInfo.dwMajorVersion <= 4 )
			{
				m_sTmp.Format ( "Windows NT %ld.%ld.%ld", OSVersionInfo.dwMajorVersion,
					OSVersionInfo.dwMinorVersion,
					OSVersionInfo.dwBuildNumber );
			}
			else
			{
				if ( OSVersionInfo.dwMajorVersion == 5 )
				{
					if ( OSVersionInfo.dwMinorVersion == 0 )
					{
						m_sTmp.Format ( "Windows 2000 %ld.%ld.%ld", OSVersionInfo.dwMajorVersion,
							OSVersionInfo.dwMinorVersion,
							OSVersionInfo.dwBuildNumber );

					}

					if ( IsVersionEx )
					{
						if ( OSVersionInfo.wProductType == VER_NT_WORKSTATION && OSVersionInfo.dwMinorVersion == 1 )
						{
							m_sTmp.Format ( "Windows XP %ld.%ld.%ld", OSVersionInfo.dwMajorVersion,
								OSVersionInfo.dwMinorVersion,
								OSVersionInfo.dwBuildNumber );
						}

						if ( OSVersionInfo.wSuiteMask & VER_SUITE_PERSONAL && OSVersionInfo.wProductType != VER_NT_SERVER )
							m_sTmp += " Home Edition";
						else
							m_sTmp += " Professional";

						if ( OSVersionInfo.wProductType == VER_NT_SERVER )
						{
							if ( OSVersionInfo.dwMinorVersion == 1 )
							{
								m_sTmp.Format ( "Microsoft Windows .NET %ld.%ld.%ld", OSVersionInfo.dwMajorVersion,
									OSVersionInfo.dwMinorVersion,
									OSVersionInfo.dwBuildNumber );
							}

							if ( OSVersionInfo.dwMinorVersion == 2 )
							{
								m_sTmp.Format ( "Microsoft Windows 2003 %ld.%ld.%ld", OSVersionInfo.dwMajorVersion,
									OSVersionInfo.dwMinorVersion,
									OSVersionInfo.dwBuildNumber );
							}

							if ( OSVersionInfo.wSuiteMask & VER_SUITE_DATACENTER )
								m_sTmp += " DataCenter Server";
							else if ( OSVersionInfo.wSuiteMask & VER_SUITE_ENTERPRISE )
								if ( OSVersionInfo.dwMajorVersion == 4 )
									m_sTmp += " Advanced Server";
								else
									m_sTmp += " Enterprise Server";
							else if ( OSVersionInfo.wSuiteMask == VER_SUITE_BLADE )
								m_sTmp += " Web Server";
							else
								m_sTmp += " Server";
						}
					}
					else
					{
						m_sTmp.Format ( "Windows XP %ld.%ld.%ld", OSVersionInfo.dwMajorVersion,
							OSVersionInfo.dwMinorVersion,
							OSVersionInfo.dwBuildNumber );
					}
				}
				else
				{
					if ( OSVersionInfo.dwMajorVersion == 6 )
					{
						switch( OSVersionInfo.dwMinorVersion )
						{
							case 0:
								if (OSVersionInfo.wProductType == VER_NT_WORKSTATION)
									m_sTmp.Format ( "Microsoft Windows Vista %ld.%ld.%ld", OSVersionInfo.dwMajorVersion,
										OSVersionInfo.dwMinorVersion,
										OSVersionInfo.dwBuildNumber );
								else
									m_sTmp.Format ( "Microsoft Windows Server 2008 %ld.%ld.%ld", OSVersionInfo.dwMajorVersion,
										OSVersionInfo.dwMinorVersion,
										OSVersionInfo.dwBuildNumber );
							break;
							case 1:
								if (OSVersionInfo.wProductType == VER_NT_WORKSTATION)
									m_sTmp.Format ( "Microsoft Windows 7 %ld.%ld.%ld", OSVersionInfo.dwMajorVersion,
										OSVersionInfo.dwMinorVersion,
										OSVersionInfo.dwBuildNumber );
								else
									m_sTmp.Format ( "Microsoft Windows Server 2008 R2 %ld.%ld.%ld", OSVersionInfo.dwMajorVersion,
										OSVersionInfo.dwMinorVersion,
										OSVersionInfo.dwBuildNumber );
								break;
							case 2:
								if (OSVersionInfo.wProductType == VER_NT_WORKSTATION)
									m_sTmp.Format ( "Microsoft Windows 8 %ld.%ld.%ld", OSVersionInfo.dwMajorVersion,
										OSVersionInfo.dwMinorVersion,
										OSVersionInfo.dwBuildNumber );
								else
									m_sTmp.Format ( "Microsoft Windows Server 2012 %ld.%ld.%ld", OSVersionInfo.dwMajorVersion,
										OSVersionInfo.dwMinorVersion,
										OSVersionInfo.dwBuildNumber );
								break;
							default:
								if (OSVersionInfo.wProductType == VER_NT_WORKSTATION)
									m_sTmp.Format ( "Microsoft Windows %ld.%ld.%ld", OSVersionInfo.dwMajorVersion,
										OSVersionInfo.dwMinorVersion,
										OSVersionInfo.dwBuildNumber );
								else
									m_sTmp.Format ( "Microsoft Windows Server %ld.%ld.%ld", OSVersionInfo.dwMajorVersion,
										OSVersionInfo.dwMinorVersion,
										OSVersionInfo.dwBuildNumber );
						}
					}
					else
						m_sTmp.Format ( "Windows NT %ld.%ld.%ld", OSVersionInfo.dwMajorVersion,	OSVersionInfo.dwMinorVersion, OSVersionInfo.dwBuildNumber );
				}
			}
		}
		else
		{
			m_sTmp.Format ( "Windows %ld %ld.%ld.%ld", OSVersionInfo.dwPlatformId,
				OSVersionInfo.dwMajorVersion,
				OSVersionInfo.dwMinorVersion,
				OSVersionInfo.dwBuildNumber );
		}
	}

	m_sTmp += " ";
	m_sTmp += OSVersionInfo.szCSDVersion;

	return m_sTmp.GetString();
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
		CRegKey keyCurrentVer;

		while ( (err = keyVersions.EnumKey( KeyNumber, &subKey[0], &nSize ) ) == ERROR_SUCCESS )
		{
			// сохраняем название раздела
			string vernumber = &subKey[0];
			string nextver = vernumber + "\t- Version: ";

			// ищем внутри раздела строковый ключ с именем Version
			if( (err = keyCurrentVer.Open( hkNetFrameworkVersions, &subKey[0], KEY_READ )) == ERROR_SUCCESS )
			{
				char vernum[100];
				ZeroMemory( vernum, sizeof(vernum));
				DWORD vernumlen = sizeof(vernum) - 1;

				if( (err = keyCurrentVer.QueryStringValue("Version", vernum, &vernumlen)) == ERROR_SUCCESS )
				{
					nextver += vernum;
					nextver += "\r\n";
				}
				else
				{
					// нет ключа с именем Version, может это .net v4 где Version лежит в подключах Full и Client?
					if( vernumber == "v4" )
					{
						CRegKey keyV4Vers(hkNetFrameworkVersions);
						string v4vernumber = vernumber + "\\Full";
						ZeroMemory( vernum, sizeof(vernum));
						// после предыдущего неудачного запроса QueryStringValue в vernumlen будет 0!
						vernumlen = sizeof(vernum) - 1;

						if( (err = keyV4Vers.Open( hkNetFrameworkVersions, v4vernumber.c_str(), KEY_READ )) == ERROR_SUCCESS )
						{
							if( (err = keyV4Vers.QueryStringValue("Version", vernum, &vernumlen)) == ERROR_SUCCESS )
							{
								nextver += vernum;
								nextver += " Full";
								nextver += "\r\n";
							}
							else
							{
								CErrCodeMsg errinfo(err);
								nextver += errinfo.GetString();
								nextver += "\r\n";
							}
						}
						else
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
									nextver += " Client only";
									nextver += "\r\n";
								}
								else
								{
									CErrCodeMsg errinfo(err);
									nextver += errinfo.GetString();
									nextver += "\r\n";
								}
							}
						}
						keyV4Vers.Close();
					}
					else
					{
						nextver += "неизв.\r\n";
						//CErrCodeMsg errinfo(err);
						//nextver += errinfo.GetString();
					}
				}

				keyCurrentVer.Close();
			}
			else
			{
				CErrCodeMsg errinfo(err);
				nextver += errinfo.GetString();
			}

			allverinfo += nextver;
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

