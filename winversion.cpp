#include "stdafx.h"
#include "autobuf.h"
#include "errcodemsg.h"

OSVERSIONINFOEX OSVersionInfo; // needed to distinguish Windows 9x/NT

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
