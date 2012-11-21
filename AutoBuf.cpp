// AutoBuf.cpp: implementation of the CAutoBufBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AutoBuf.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAutoBufBase::CAutoBufBase(PVOID pBuf):m_pBuf(pBuf)
{
	//Обновляем значение m_BufSize
	GetSize();
}

CAutoBufBase::~CAutoBufBase()
{
	if (m_pBuf)
		free(m_pBuf);
}

void CAutoBufBase::Attach(PVOID pBuf)
{
	_ASSERTE(pBuf);

	if (m_pBuf != pBuf){
		if (m_pBuf)
			free(m_pBuf);
		m_pBuf = pBuf;
		//Обновляем значение m_BufSize
		GetSize();
	}
}

PVOID CAutoBufBase::Alloc(size_t dwBufSize)
{
	DWORD BufSize = m_BufSize;
	if (dwBufSize != -1)		//Если параметр опущен - берем размер
		BufSize = dwBufSize;	//из внутренней переменной
	if (m_pBuf){
		if (m_BufSize < BufSize)//Перераспределяем память, только
			m_pBuf = realloc(m_pBuf,BufSize);//если запрошено больше чем есть
	}
	else
		m_pBuf = malloc(BufSize);//Выделяем память

	m_BufSize = BufSize;
	return m_pBuf;
}

size_t CAutoBufBase::GetSize()
{
	if (m_pBuf)
		m_BufSize = _msize(m_pBuf);	//Получаем размер блока памяти
	else
		m_BufSize = 0;
	
	return m_BufSize;
}
