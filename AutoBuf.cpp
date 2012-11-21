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
	//��������� �������� m_BufSize
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
		//��������� �������� m_BufSize
		GetSize();
	}
}

PVOID CAutoBufBase::Alloc(size_t dwBufSize)
{
	DWORD BufSize = m_BufSize;
	if (dwBufSize != -1)		//���� �������� ������ - ����� ������
		BufSize = dwBufSize;	//�� ���������� ����������
	if (m_pBuf){
		if (m_BufSize < BufSize)//���������������� ������, ������
			m_pBuf = realloc(m_pBuf,BufSize);//���� ��������� ������ ��� ����
	}
	else
		m_pBuf = malloc(BufSize);//�������� ������

	m_BufSize = BufSize;
	return m_pBuf;
}

size_t CAutoBufBase::GetSize()
{
	if (m_pBuf)
		m_BufSize = _msize(m_pBuf);	//�������� ������ ����� ������
	else
		m_BufSize = 0;
	
	return m_BufSize;
}
