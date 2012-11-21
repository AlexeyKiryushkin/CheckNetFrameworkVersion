#ifndef IV_ERRCODEMSG_classes_H
#define IV_ERRCODEMSG_classes_H

////////////////////////////////////////////////////////////////////////
//  (MFC independent)
//  class CErrCodeMsg
//  Version 1.03 (Aug 27 2002)
//  Igor_Vartanov, FIDO: 2:5025/16.26 AKA /38.70
//               e-mail: ivartanov@rsdn.ru
//  Alexey Kirjushkin
//               e-mail: odissey@rsdn.ru
//
//  v1.03 - Добавлена возможность получение описания ошибок
//          из модуля заданного в параметре конструктора HMODULE hm
//        - Изменён прототип GetOemString() для совместимости с UNICODE
////////////////////////////////////////////////////////////////////////

class CErrCodeMsg
{

    public:
        CErrCodeMsg( DWORD code,
                     DWORD flags =
                         FORMAT_MESSAGE_ALLOCATE_BUFFER |
                         FORMAT_MESSAGE_FROM_SYSTEM,
                     DWORD   size    = 0,
                     LPTSTR  buffer  = NULL,
                     LANGID  langId  = LANG_NEUTRAL,
                     LANGID  langSub = SUBLANG_DEFAULT,
                     HMODULE hm      = NULL )
                : m_dwErrCode( code ),
                m_dwFlags( flags ),
                m_dwSize( size ),
                m_pszUserBuf( buffer ),
                m_pszErrMsg( NULL ),
                m_pszErrMsgOem( NULL ),
                m_langId( langId ),
                m_langSub( langSub ),
                m_hmDescSource( hm )
        {
            if ( hm )
                m_dwFlags |= FORMAT_MESSAGE_FROM_HMODULE;
        }

        virtual ~CErrCodeMsg()
        {
            if ( m_pszErrMsgOem )
                ::LocalFree( m_pszErrMsgOem );

            if ( FORMAT_MESSAGE_ALLOCATE_BUFFER & m_dwFlags )
                if ( m_pszErrMsg )
                    ::LocalFree( m_pszErrMsg );

            // в противном случае за освобождение буфера отвечает клиент
        }

        DWORD GetCode()
        {
            return m_dwErrCode;
        }

        virtual LPCTSTR GetString( LPDWORD pdwResult = NULL )
        {
            if ( m_pszErrMsg )
                return m_pszErrMsg; // Сообщение уже было сформировано!

            if ( ( FORMAT_MESSAGE_ALLOCATE_BUFFER & m_dwFlags ) == FALSE )
            {
                // Буфер должен быть предоставлен пользователем
                if ( m_pszUserBuf )
                    m_pszErrMsg = m_pszUserBuf; // Так оно и есть...
                else
                {
                    // Странно, а буфера-то нет!..
                    // тогда попросим систему выделить его
                    m_dwFlags |= FORMAT_MESSAGE_ALLOCATE_BUFFER;
                }
            }

            DWORD res =
                ::FormatMessage(
                    m_dwFlags, m_hmDescSource, m_dwErrCode,
                    MAKELANGID( m_langId, m_langSub ),
                    ( FORMAT_MESSAGE_ALLOCATE_BUFFER & m_dwFlags ) ? 
                    ( LPTSTR ) & m_pszErrMsg : m_pszErrMsg ,
                    m_dwSize, NULL );

            if ( pdwResult )
                * pdwResult = res;

            return m_pszErrMsg;
        }

        LPCSTR GetOemString( LPDWORD pdwRes = NULL )
        {
            if ( m_pszErrMsgOem )
                return m_pszErrMsgOem;  // Сообщение уже было сформировано

            LPCTSTR str = GetString( pdwRes );

            if ( !str )
                return NULL;

            m_pszErrMsgOem = ( LPSTR ) ::LocalAlloc( GPTR,  lstrlen( str ) + 1 );

            if ( !m_pszErrMsgOem )
                return NULL;

            ::CharToOem( str, m_pszErrMsgOem );

            return m_pszErrMsgOem;
        }

        void Deallocate()
        {
            if ( m_pszErrMsgOem )
            {
                ::LocalFree( m_pszErrMsgOem );
                m_pszErrMsgOem = NULL;
            }

            if ( FORMAT_MESSAGE_ALLOCATE_BUFFER & m_dwFlags )
                if ( m_pszErrMsg )
                {
                    ::LocalFree( m_pszErrMsg );
                    m_pszErrMsg = NULL;
                }
        }

        DWORD operator=( DWORD code )
        {
            if ( m_dwErrCode != code )
            {
                Deallocate();
                m_dwErrCode = code;
            }

            return m_dwErrCode;
        }

        operator DWORD ()
        {
            return m_dwErrCode;
        }

        operator HRESULT ()
        {
            return ( HRESULT ) m_dwErrCode;
        }

    protected:
        DWORD m_dwErrCode;
        DWORD m_dwFlags;

    private:
        DWORD   m_dwSize;
        LPTSTR  m_pszErrMsg;
        LPSTR   m_pszErrMsgOem;
        LPTSTR  m_pszUserBuf;
        LANGID  m_langId;
        LANGID  m_langSub;
        HMODULE m_hmDescSource;
};

////////////////////////////////////////////////////////////////////////
//  (MFC independent)
//  class CLastErrCodeMsg
//  Version 1.02 (Sept 11 2001)
//  Igor_Vartanov, FIDO: 2:5025/16.26 AKA /38.70
//               e-mail: ivartanov@rsdn.ru
//
////////////////////////////////////////////////////////////////////////

class CLastErrCodeMsg : public CErrCodeMsg
{

    public:
        CLastErrCodeMsg() : CErrCodeMsg( ::GetLastError() )
        {}

        ;

        DWORD operator=( DWORD code )
        {
            return ( ( CErrCodeMsg& ) ( *this ) = code );
        }
};

////////////////////////////////////////////////////////////////////////

#endif
