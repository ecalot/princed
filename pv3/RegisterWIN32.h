/******************************************************************************
/** Class Name.....: CRegisterWIN32 (RegisterWIN32.h)
/** Description....: Some registry functions in WIN32 Environnment
/**                  If you want to use the Class in MFC (CString)
/**                  #define USE_MFC
/**
/** Author, date...: Yves Lessard , 27-Apr-2000.
/**
/** 1.0.0.0   30-Apr-2000
/**
/** Modifications..:
/**
/** Method(Public)						Description
/**
/**	--> There are 2 ways to initialize a Session Key
/**
/** 1) Open(RootKey,SubKey,Mode)		Open a Session and must pass the RootKey and Path
/**                                     By default Mode is Read Only
/** 2) Open(Subkey, Mode)               The Rootkey = HKEY_LOCAL_MACHINE as default
/**                                     You specify the Path and The Key
/** Close								Close a Session Key
/**
/** Write(szKey, LPCTSTR szValue)       Write a String to the Key
/** Write(szKey, DWORD)					Write a DWORD value
/** Write(szKey , Int)					Write a Int value
/** Write(szKey, LPBYTE)				Write Byte array
/** WriteStruct(szKey, T &obj)          For all other kind of Data
/**                                     Float, Double, CRect, etc
/**  
/** Read(szKey, &pszDest)               Read a string value
/** Read(szKey, CString)                Read a String value Store in CString
/** Read(szKey, &DWORD)					Read a Dword value
/** Read(szKey, Int)					Read Integer value
/** Read(szKey, LPBYTE, &DWORD)			Read a Byte array
/** ReadStruct(LPCTSTR szKey, T &obj)	Read all other kind of Data
/**
/** DeleteValue(LPCTSTR szKey)			Delete a Key Value
/** DeleteKey(HKEY, LPCTSTR szSubKey)   Delete a Key or SubKey (Recursive)
/**    
******************************************************************************/

//#if !defined(AFX_REGISTERWIN32_H__305C4B07_38D6_11D5_8068_0050BAB07D8B__INCLUDED_)
//#define AFX_REGISTERWIN32_H__305C4B07_38D6_11D5_8068_0050BAB07D8B__INCLUDED_

//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
#include <tchar.h>
#include <windows.h>
#include <stdlib.h>

//** If use with MFC and you want CString remove rem on next line
//#define USE_MFC

class CRegisterWIN32  
{
public:
	BOOL DeleteKey(HKEY hKeyRoot, LPCTSTR szPath);
	BOOL DeleteValue(LPCTSTR szKey);
#ifdef USE_MFC
	BOOL Read(LPCTSTR szKey, CString &pszValue);
#endif
	BOOL Read(LPCTSTR szKey, int &piVal);
	BOOL Read(LPCTSTR szKey, DWORD &pdwVal);
	BOOL Read(LPCTSTR szKey, LPTSTR &pszValue);
	BOOL Read(LPCTSTR szKey, LPBYTE pValue, DWORD &pnLen);

	BOOL Write(LPCTSTR szKey, int iVal);
	BOOL Write(LPCTSTR szKey, DWORD dwVal);
	BOOL Write(LPCTSTR szKey, LPCTSTR szValue);
	BOOL Write(LPCTSTR szKey, LPBYTE pValue, DWORD nLen);
	
	BOOL Open(LPCTSTR szPath, BOOL bReadOnly = TRUE);
	BOOL Close();
	BOOL Open(HKEY hKeyRoot, LPCTSTR szPath, BOOL bReadOnly = TRUE);
	CRegisterWIN32();
	virtual ~CRegisterWIN32();
	//**************
	//  WriteStruct
	//**************
	template <class T>BOOL WriteStruct(LPCTSTR szKey, T &obj)
	{
		BOOL bResult = FALSE;
		try
		{
			//** Translate data to Binary
			if ( RegSetValueEx(m_hKey, szKey, 0, REG_BINARY,(LPBYTE)&obj, sizeof(T)) == ERROR_SUCCESS )
				bResult = TRUE;
		}
		catch(...)
		{
			bResult = FALSE;
		}
		return bResult;
	}
	//********************
	// ReadStruct
	//********************
	template <class T>BOOL ReadStruct(LPCTSTR szKey, T &obj)
	{
		BOOL bResult = FALSE;
		try
		{
			//** Read the data
			DWORD dwLen = sizeof(T);
			if ( ::RegQueryValueEx(m_hKey, szKey, NULL, NULL, (LPBYTE)&obj,&dwLen) == ERROR_SUCCESS )
				bResult = TRUE;
		}
		catch(...)
		{	
			bResult = FALSE;
		}
		return bResult;
	}

private:
	BOOL DeleteNTway(HKEY hKey, LPCTSTR szSubKey);
	BOOL GetOSversion();
	_TCHAR * m_szTemp;
	BOOL StringIN(LPCTSTR szKey);
	BOOL GetValue(LPCTSTR szKey);
	DWORD m_dTemp;
	BOOL SetValue(LPCTSTR szKey, DWORD dVal);
	HKEY m_hKey;
	BOOL CreateKey();
	BOOL VerifyKey();
	_TCHAR *m_pszPath;
	HKEY m_RootKey;
	int iOSversion;
};

//#endif // !defined(AFX_REGISTERWIN32_H__305C4B07_38D6_11D5_8068_0050BAB07D8B__INCLUDED_)
