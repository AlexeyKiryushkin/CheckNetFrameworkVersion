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
	//Выделение памяти
	PVOID Alloc(size_t dwBufSize=-1);
	void Attach(PVOID pBuf);
	CAutoBufBase(PVOID pBuf = 0);
	~CAutoBufBase();

	//Явный конструктор	
	explicit CAutoBufBase(size_t BufSize)
	{
		if (BufSize > 0)
			m_pBuf = malloc(BufSize);
		else
			m_pBuf = 0;
		m_BufSize = BufSize;
	};

	//Копирующий конструктор. Происходит копирование только размера
	//Указатель или память не копируется
	CAutoBufBase(const CAutoBufBase& abuf)
	{
		m_pBuf = 0;
		m_BufSize = abuf.m_BufSize;
	};

	//Присоединения выделенного буфера
	CAutoBufBase& operator=(const PVOID pBuf)
	{
		Attach(pBuf);
		return *this;
	};

	//Отсоединение буфера
	PVOID Detach()
	{
		PVOID pbuf = m_pBuf;
		m_pBuf = NULL;
		return pbuf;
	};

	//Получение адреса закрытой переменной, хранящей
	//размер буфера
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

	//Универсальная функция освобождения
	template<class T>
	static void Free(T** pBuf)
	{
		free((PVOID)*pBuf);
		*pBuf = 0;
	};

	//Protected functions
protected:
	size_t GetSize();
	
	//Запрещен оператор присваивания экземпляров классов
	CAutoBufBase& operator=(const CAutoBufBase& abuf);

	//Protected attributes
protected:
	size_t m_BufSize;
	PVOID m_pBuf;
};

	//Удобный шаблон
template<class T>
class CAutoBuf : public CAutoBufBase
{
public:

	//Конструктор
	CAutoBuf(PVOID pBuf = 0):CAutoBufBase(pBuf){};

	//Деструктор
	~CAutoBuf(){};

	//Выделение памяти
	T* Alloc(DWORD dwBufSize=-1)
	{
		return (T*)CAutoBufBase::Alloc(dwBufSize);
	};

	//Отсоединение
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

	//Создание и инициализация (заполнение) буфера
	void Copy(T* pT,size_t dwpTSize)
	{
		Alloc(dwpTSize);
		memcpy(m_pBuf,pT,dwpTSize);
	};
};
#endif // !defined(AFX_AUTOBUF_H__18DD1F01_40E6_4371_9314_33CF5C4B7805__INCLUDED_)
