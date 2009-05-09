/*********************************************************
  * Copyright (C) 2001-2002 Kresimir Petric (kreso@weonlydo.com)
  * All rights reserved.
  *
  * This code is property of WeOnlyDo! Inc.
  * You have no right to change or use this code without written
  * permission of WeOnlyDo! Software.
  *
  * Any changes to the code must be requested from general@weonlydo.com 
  *
  * Last revised on 24.10.2002 17:50:32
 *********************************************************/

#include "stdafx.h"
#include "ComBSTR2.h"

// Function name	: CComBSTR2::CComBSTR2
// Description	    : 
// Return type		: 
CComBSTR2::CComBSTR2() : CComBSTR()
{
	m_Buffer = NULL;
}


// Function name	: CComBSTR2::CComBSTR2
// Description	    : 
// Return type		: 
// Argument         : int nSize
CComBSTR2::CComBSTR2(int nSize) : CComBSTR(nSize) 
{
	m_Buffer = NULL;
}


// Function name	: CComBSTR2::CComBSTR2
// Description	    : 
// Return type		: 
// Argument         : int nSize
// Argument         : LPCOLESTR sz ) : CComBSTR(nSize
// Argument         : sz
CComBSTR2::CComBSTR2(int nSize, LPCOLESTR sz ) : CComBSTR(nSize, sz)
{
	m_Buffer = NULL;
}


// Function name	: CComBSTR2::CComBSTR2
// Description	    : 
// Return type		: 
// Argument         : LPCOLESTR pSrc
CComBSTR2::CComBSTR2(LPCOLESTR pSrc ) : CComBSTR (pSrc)
{
	m_Buffer = NULL;
}


// Function name	: CComBSTR2::CComBSTR2
// Description	    : 
// Return type		: 
// Argument         : const CComBSTR& src
CComBSTR2::CComBSTR2(const CComBSTR& src ) : CComBSTR (src)
{
	m_Buffer = NULL;
}


// Function name	: CComBSTR2::CComBSTR2
// Description	    : 
// Return type		: 
// Argument         : REFGUID src
CComBSTR2::CComBSTR2(REFGUID src) : CComBSTR (src)
{
	m_Buffer = NULL;
}

// Function name	: CComBSTR2::~CComBSTR2
// Description	    : 
// Return type		: 
CComBSTR2::CComBSTR2(LPCSTR src) : CComBSTR(src)
{
	m_Buffer = NULL;
}


// Function name	: CComBSTR2::~CComBSTR2
// Description	    : 
// Return type		: 
CComBSTR2::~CComBSTR2() 
{
	if (m_Buffer)
		free(m_Buffer);
}


// Function name	: *CComBSTR2::ToString
// Description	    : 
// Return type		: char 
char *CComBSTR2::ToString(int *Len)
{
	if (m_Buffer)
		free(m_Buffer);
	m_Buffer = NULL;

	int result = ::WideCharToMultiByte(CP_ACP, 0, m_str, Length() ,0,0 , NULL, NULL);
	if (result<1)
		return "";

	m_Buffer = (char *)malloc(result + 1);
	::WideCharToMultiByte(CP_ACP, 0, m_str, Length() ,m_Buffer,result , NULL, NULL);
	m_Buffer[result]=0;

	if (Len)
		*Len = result;
	return m_Buffer;	
}

char *CComBSTR2::ToString()
{
	return ToString(NULL);
}

// Function name	: *CComBSTR2::ToPersistentString
// Description	    : 
// Return type		: char 
char *CComBSTR2::ToPersistentString()
{

	if (m_Buffer)
		return m_Buffer;
	else
		return ToString();
}

void CComBSTR2::FromUTF8String(char *Source, int nLen)
{
	::SysFreeString(m_str);
	int nConvertedLen = MultiByteToWideChar(CP_UTF8, 0, Source, nLen+1, NULL, NULL)-1;
	m_str = ::SysAllocStringLen(NULL, nConvertedLen);
	if (m_str != NULL)
	{
		MultiByteToWideChar(CP_UTF8, 0, Source, -1,
			m_str, nConvertedLen);
	}
}
void CComBSTR2::FromUTF8String(char *Source)
{
	FromUTF8String(Source, strlen(Source));
}

char *CComBSTR2::ToUTF8String()
{
	return ToUTF8String(NULL);
}
char *CComBSTR2::ToUTF8String(int *Len)
{
	if (m_Buffer)
		free(m_Buffer);
	m_Buffer = NULL;

	if (Len)
		*Len = 0;
	int result = ::WideCharToMultiByte(CP_UTF8, 0, m_str, Length() ,0,0 , NULL, NULL);
	if (result<1)
		return ToString();

	m_Buffer = (char *)malloc(result + 1);
	memset(m_Buffer, 0, result+1);
	::WideCharToMultiByte(CP_UTF8, 0, m_str, Length() ,m_Buffer,result , NULL, NULL);
	m_Buffer[result]=0;

	if (Len)
		*Len = result;
	return m_Buffer;
}

void CComBSTR2::FromTextBox(HWND h)
{
	::SysFreeString(m_str);
	int cch = GetWindowTextLengthW(h);
	WCHAR *pszTitle = new WCHAR[cch + 1];
    pszTitle[0] = '\0';

	GetWindowTextW(h, pszTitle, cch + 1);
    pszTitle[cch] = '\0';

    CComBSTR cbstrTitle;
    cbstrTitle = pszTitle;
    delete [] pszTitle;

	Attach(cbstrTitle.Detach());
}
