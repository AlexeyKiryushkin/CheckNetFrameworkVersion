// AutoBuf.h: interface for the CAutoBuf class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTOBUF_H__18DD1F01_40E6_4371_9314_33CF5C4B7805__INCLUDED_)
#define AFX_AUTOBUF_H__18DD1F01_40E6_4371_9314_33CF5C4B7805__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAutoBufBase
{
public:
	//��������� ������
	PVOID Alloc(size_t dwBufSize=-1);
	void Attach(PVOID pBuf);
	CAutoBufBase(PVOID pBuf = 0);
	~CAutoBufBase();

	//����� �����������	
	explicit CAutoBufBase(size_t BufSize)
	{
		if (BufSize > 0)
			m_pBuf = malloc(BufSize);
		else
			m_pBuf = 0;
		m_BufSize = BufSize;
	};

	//���������� �����������. ���������� ����������� ������ �������
	//��������� ��� ������ �� ����������
	CAutoBufBase(const CAutoBufBase& abuf)
	{
		m_pBuf = 0;
		m_BufSize = abuf.m_BufSize;
	};

	//������������� ����������� ������
	CAutoBufBase& operator=(const PVOID pBuf)
	{
		Attach(pBuf);
		return *this;
	};

	//������������ ������
	PVOID Detach()
	{
		PVOID pbuf = m_pBuf;
		m_pBuf = NULL;
		return pbuf;
	};

	//��������� ������ �������� ����������, ��������
	//������ ������
	PDWORD GetSizeAddr()
	{
		return (PDWORD)&m_BufSize;
	};

	operator PVOID() const
	{
		return m_pBuf;
	};

	operator DWORD() const
	{
		return (DWORD)m_BufSize;
	};

	DWORD Size() const
	{
		return m_BufSize;
	};

	PVOID GetBuffer()
	{
		return m_pBuf;
	};

	//������������� ������� ������������
	template<class T>
	static void Free(T** pBuf)
	{
		free((PVOID)*pBuf);
		*pBuf = 0;
	};

	//Protected functions
protected:
	size_t GetSize();
	
	//�������� �������� ������������ ����������� �������
	CAutoBufBase& operator=(const CAutoBufBase& abuf);

	//Protected attributes
protected:
	size_t m_BufSize;
	PVOID m_pBuf;
};

	//������� ������
template<class T>
class CAutoBuf : public CAutoBufBase
{
public:

	//�����������
	CAutoBuf(PVOID pBuf = 0):CAutoBufBase(pBuf){};

	//����������
	~CAutoBuf(){};

	//��������� ������
	T* Alloc(DWORD dwBufSize=-1)
	{
		return (T*)CAutoBufBase::Alloc(dwBufSize);
	};

	//������������
	T* Detach()
	{
		return (T*)CAutoBufBase::Detach();
	};

	T* GetBuffer()
	{
		return (T*)m_pBuf;
	};

	operator T*() const
	{
		return (T*)m_pBuf;
	};

	operator DWORD() const
	{
		return m_BufSize;
	};

	operator PDWORD()
	{
		return GetSizeAddr();
	};

	T* operator->() const
	{
		return (T*)m_pBuf;
	};

	//�������� � ������������� (����������) ������
	void Copy(T* pT,size_t dwpTSize)
	{
		Alloc(dwpTSize);
		memcpy(m_pBuf,pT,dwpTSize);
	};
};
#endif // !defined(AFX_AUTOBUF_H__18DD1F01_40E6_4371_9314_33CF5C4B7805__INCLUDED_)
