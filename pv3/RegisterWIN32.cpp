// RegisterWIN32.cpp: implementation of the CRegistreWIN32 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RegisterWIN32.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegisterWIN32::CRegisterWIN32()
{
	//** Par defaut on utilise ce RootKey
	m_RootKey = HKEY_LOCAL_MACHINE;
	m_pszPath = new TCHAR[_MAX_PATH * sizeof(TCHAR)];
	m_hKey = NULL;
	m_szTemp = new TCHAR[_MAX_PATH * sizeof(TCHAR)];
}

CRegisterWIN32::~CRegisterWIN32()
{
	//** Clean up memory and Close Session
	delete []m_pszPath;
	delete []m_szTemp;
	if(m_hKey) RegCloseKey(m_hKey);
}


////////////////////////////////////////////////////////////////////////////////
//  Function.......: Open 
//
//  Description....: Open a registry Session
//          Example: CRegistreWIN32 MyReg;
//                   if( MyReg.Open(HKEY_CURRENT_USER, _T("Software\\SevySoft\\Admin")) )
//						//** Open the Key in Read Mode Only
//						//** The Key is in the Registry
//					else
//						//** The Key is not founded
//
//			Example: Open(HKEY_CURRENT_USER, _T("Software\\SevySoft\\Admin"), FALSE) 						  
//                   //** This time if the Key do not exist then
//					 //** it will be created 
//                  
//
//  Author, date...: Yves Lessard , 24-Avril-2001.
//
//  Modifications..:
//
//  Arguments
//  Name                Type     Acces   Description
//  ------------------  -------  ------  -------------------------------------
//  hKeyRoot            HKEY       R      Root Key
//  szSubKey            LPCTSTR    R      Sub Key
//  bReadOnly           BOOL       R      TRUE -> (Default) Read Only
//                                        FALSE-> Read/Write
//                                                If Subkey not found Create it     
//  Open                BOOL       W      TRUE-> OK else FALSE  
////////////////////////////////////////////////////////////////////////////////
BOOL CRegisterWIN32::Open(HKEY hKeyRoot, LPCTSTR szPath, BOOL bReadOnly)
{
	BOOL bResult = FALSE;
	try
	{
		//** If a Session Key Opened then close it
		if ( m_hKey )
			RegCloseKey(m_hKey);

		//** 5 Roots Key choice 
		if ( hKeyRoot == HKEY_CLASSES_ROOT || 
			 hKeyRoot == HKEY_CURRENT_USER ||
			 hKeyRoot == HKEY_LOCAL_MACHINE || 
			 hKeyRoot == HKEY_USERS ||
			 hKeyRoot == HKEY_CURRENT_CONFIG ) 
			{
				//** Save RootKey for reference
				m_RootKey = hKeyRoot;
				if ( _tcslen(szPath)* sizeof(TCHAR) > 0 )
				{
					//** We have a path so save it
					_tcscpy(m_pszPath, szPath);
					switch (bReadOnly)
					{
					case TRUE:
						//** Read Mode Only
						if ( VerifyKey() )
							bResult = TRUE;
						break;
					default:
						//** Else Read/Write
						if ( VerifyKey() == FALSE )
						{
							//** Key not Found so create it
							if( CreateKey() )
								bResult = TRUE;
						}
					}
				}
			}
	}
	catch(...)
	{
		bResult = FALSE;
	}
	return bResult;
}


////////////////////////////////////////////////////////////////////////////////
//  Function.......: Close 
//
//  Description....: Close a Registry Session  
//                   The destructor also Close the Key
//
//  Author, date...: Yves Lessard , 24-Avril-2001.
//
//  Modifications..:
//
//  Arguments
//  Name                Type     Acces   Description
//  ------------------  -------  ------  -------------------------------------
//  Close               BOOL        W    TRUE->OK else FALSE   
////////////////////////////////////////////////////////////////////////////////
BOOL CRegisterWIN32::Close()
{
	BOOL bResult = FALSE;
	try
	{
		if ( m_hKey )
			RegCloseKey(m_hKey);
		m_hKey = NULL;
	}
	catch(...)
	{
		bResult = FALSE;
	}

	return bResult;
}


////////////////////////////////////////////////////////////////////////////////
//  Function.......:  Open
//
//  Description....:  Open a Session with Path  and Key
//                    The KeyRoot = HKEY_LOCAL_MACHINE as Default
//
//  Author, date...: Yves Lessard , 24-Avril-2001.
//
//  Modifications..:
//
//  Arguments
//  Name                Type     Acces   Description
//  ------------------  -------  ------  -------------------------------------
//  szPath              LPCTSTR     R    The Path
//  bReadOnly           BOOL        R    TRUE-> (Default) Read Only
//                                       FALSE-> Read/Write           
////////////////////////////////////////////////////////////////////////////////
BOOL CRegisterWIN32::Open(LPCTSTR szPath, BOOL bReadOnly)
{
	BOOL bResult = FALSE;
	try
	{
		//** If Session Key Opened then close it
		if ( m_hKey )
		RegCloseKey(m_hKey);
		//** Always use this one
		m_RootKey = HKEY_LOCAL_MACHINE;

		if ( _tcslen(szPath)* sizeof(TCHAR) > 0 )
		{
			_tcscpy(m_pszPath, szPath);
			switch (bReadOnly)
			{
				case TRUE:
					//** Read only mode 
					if ( VerifyKey() )
						//** The Path exist
						bResult = TRUE;
					break;
				default:
					//** Read Write Mode
					if ( VerifyKey() == FALSE )
						{
							//** If Path not Found Create it
							if( CreateKey() )
								//** If success return TRUE
								bResult = TRUE;
						}
			}
		}
	}
	catch(...)
	{
		bResult = FALSE;
	}

	return bResult;
	
}

////////////////////////////////////////////////////////////////////////////////
//  Function.......: Write (int)
//
//  Description....: Write Integer value
//                  
//
//  Author, date...: Yves Lessard , 24-Avril-2001.
//
//  Modifications..:
//
//  Arguments
//  Name                Type     Acces   Description
//  ------------------  -------  ------  -------------------------------------
//  szKey               LPCTSTR    R     The Key
//  iVal                int        R     The Value
//  Write               BOOL       W     TRUE->Succes Else FALSE
////////////////////////////////////////////////////////////////////////////////
BOOL CRegisterWIN32::Write(LPCTSTR szKey, int iVal)
{
	BOOL bResult = FALSE;
	try
	{
		if ( SetValue(szKey, (DWORD)iVal) )
			bResult = TRUE;
	}
	catch(...)
	{
		bResult = FALSE;
	}
	return bResult;
}

////////////////////////////////////////////////////////////////////////////////
//  Function.......: Write (string)
//
//  Description....: Write a string into the Key value
//                  
//
//  Author, date...: Yves Lessard , 24-Avril-2001.
//
//  Modifications..:
//
//  Arguments
//  Name                Type     Acces   Description
//  ------------------  -------  ------  -------------------------------------
//  szKey               LPCTSTR    R     The Key
//  szValue             LPCTSTR    R     The String to Write 
//  Write               BOOL       W     TRUE-> Ok Else FALSE
////////////////////////////////////////////////////////////////////////////////
BOOL CRegisterWIN32::Write(LPCTSTR szKey, LPCTSTR szValue)
{
	BOOL bResult = FALSE;
	try
	{
		DWORD dLen = _tcslen(szValue) * sizeof(TCHAR);
		if ( dLen > 0)
		{
			//** Not empty string so proceed
			dLen++;
			if( RegSetValueEx(m_hKey, szKey,0,
							REG_SZ, (CONST BYTE*)szValue,
							dLen ) == ERROR_SUCCESS )
							//** Success
							bResult = TRUE;
		}
	}
	catch(...)
	{
		bResult = FALSE;
	}
	return bResult;
}


////////////////////////////////////////////////////////////////////////////////
//  Function.......: Write (DWORD)  
//
//  Description....: Write a DWORD to the Key
//                  
//
//  Author, date...: Yves Lessard , 24-Avril-2001.
//
//  Modifications..:
//
//  Arguments
//  Name                Type     Acces   Description
//  ------------------  -------  ------  -------------------------------------
//  szKey               LPCTSTR    R     The Key
//  dwVal               DWORD      R     The Value
//  Write               BOOL       W     TRUE->Succes Else FALSE
////////////////////////////////////////////////////////////////////////////////
BOOL CRegisterWIN32::Write(LPCTSTR szKey, DWORD dwVal)
{
	BOOL bResult = FALSE;
	try
	{
		if ( SetValue(szKey, dwVal) )
			bResult = TRUE;
	}
	catch(...)
	{
		bResult = FALSE;
	}
	return bResult;
}

////////////////////////////////////////////////////////////////////////////////
//  Function.......: Write (LPBYTE)  
//
//  Description....: Write a Binary value
//          Example: BYTE MonByte[10]={1,2,3,4,5,6,7,8,9,10}; 
//                   MyReg.Write("BinVal", MonByte, 10);
//                   We write the BYTE array with 10 values           
//                  
//
//  Author, date...: Yves Lessard , 24-Avril-2001.
//
//  Modifications..:
//
//  Arguments
//  Name                Type     Acces   Description
//  ------------------  -------  ------  -------------------------------------
//  szKey               LPCTSTR    R     The Key
//  dwVal               LPBYTE     R     The BYTE Array
//  nLen                DWORD      R     Number of Elements to Write 
//  Write               BOOL       W     TRUE->Succes Else FALSE
////////////////////////////////////////////////////////////////////////////////
BOOL CRegisterWIN32::Write(LPCTSTR szKey, LPBYTE pValue, DWORD nLen)
{
	BOOL bResult = FALSE;
	try
	{
		if ( RegSetValueEx(m_hKey, szKey, 0, REG_BINARY, pValue, nLen) == ERROR_SUCCESS )
			bResult = TRUE;
	}
	catch(...)
	{
		bResult = FALSE;
	}
   	return bResult;
}



////////////////////////////////////////////////////////////////////////////////
//  Function.......: Read
//
//  Description....: Read a string Value into a char or TCHAR
//			Example: TCHAR *szTemp=NULL;
//					 You don't need to allocate memory 
//                   but you are responsable to free up memmory 
//
//  Author, date...: Yves Lessard , 25-Avril-2001.
//
//  Modifications..:
//
//  Arguments
//  Name                Type     Acces   Description
//  ------------------  -------  ------  -------------------------------------
//  szKey               LPCTSTR     R    The key to read
//  pszValue            LPTSTR      R    Destination to store Data
//  Read                BOOL        W    TRUE-> OK Else FALSE       
////////////////////////////////////////////////////////////////////////////////
BOOL CRegisterWIN32::Read(LPCTSTR szKey, LPTSTR &pszValue)
{
	BOOL bResult = FALSE;
	try
	{
		if ( StringIN(szKey) )
		{
			//** The Key exist so proceed 
			if( pszValue != NULL )
				delete pszValue;
			//** We re-allocate memory on destination pointer
			pszValue = new _TCHAR[_tcslen(m_szTemp) + 1 * sizeof(TCHAR)];
			//** Transfert the reading result
			_tcscpy(pszValue, m_szTemp);
			bResult = TRUE;
		}
	}
	catch(...)
	{
		bResult = FALSE;
	}
	return bResult;
}


////////////////////////////////////////////////////////////////////////////////
//  Function.......: Read (DWORD)
//
//  Description....: We read a DWORD value
//	        Example: DWORD dVal;
//	                 MyReg.Read("LTop", dVal);                  
//
//  Author, date...: Yves Lessard , 25-Avril-2001.
//
//  Modifications..:
//
//  Arguments
//  Name                Type     Acces   Description
//  ------------------  -------  ------  -------------------------------------
//  szKey               LPCTSTR    R     The Key
//  pdwVal              DWORD      R     Destination
//  Read                BOOL       W     TRUE->Succes else FALSE
////////////////////////////////////////////////////////////////////////////////
BOOL CRegisterWIN32::Read(LPCTSTR szKey, DWORD &pdwVal)
{
	BOOL bResult = FALSE;
	try
	{
		if ( GetValue(szKey) )
		{
			//** Save result into destination 
			pdwVal = m_dTemp;
			bResult = TRUE;
		}
	}
	catch(...)
	{
		bResult = FALSE;
	}
	return bResult;
}


////////////////////////////////////////////////////////////////////////////////
//  Function.......: Read (int)
//
//  Description....: Integer reading
//                  
//
//  Author, date...: Yves Lessard , 24-Avril-2001.
//
//  Modifications..:
//
//  Arguments
//  Name                Type     Acces   Description
//  ------------------  -------  ------  -------------------------------------
//  szKey               LPCTSTR    R     The Key
//  *piVal              int        R     Int result
//  Read                BOOL       W     TRUE->Succes Else FALSE
////////////////////////////////////////////////////////////////////////////////
BOOL CRegisterWIN32::Read(LPCTSTR szKey, int &piVal)
{
	BOOL bResult = FALSE;
	try
	{
		if ( GetValue(szKey) )
		{
			//** Store reulst into destination
			piVal = (int)m_dTemp;
			bResult = TRUE;
		}
	}
	catch(...)
	{
		bResult = FALSE;
	}
	return bResult;
}


#ifdef USE_MFC
//** If we are using MFC CString

////////////////////////////////////////////////////////////////////////////////
//  Function.......:  Read
//
//  Description....:  Read a String key & store it in CString (MFC)
//                    CString automaticaly deal with memory
//
//  Author, date...: Yves Lessard , 24-Avril-2001.
//
//  Modifications..:
//
//  Arguments
//  Name                Type     Acces   Description
//  ------------------  -------  ------  -------------------------------------
//  szKey               LPCTSTR     R     The Key
//  szValue             CString     W     The read result
//  Read                BOOL        W     TRUE->OK else FALSE  
////////////////////////////////////////////////////////////////////////////////
BOOL CRegisterWIN32::Read(LPCTSTR szKey, CString &pszValue)
{
	BOOL bResult = FALSE;
	try
	{
		if( StringIN(szKey) )
		{
			//** Store result in CString destination
			pszValue = m_szTemp;
			bResult = TRUE;
		}
	}
	catch(...)
	{
		bResult = FALSE;
	}
	return bResult;
}
#endif


////////////////////////////////////////////////////////////////////////////////
//  Function.......: DeleteValue
//
//  Description....: Delete a Key Value 
//                  
//
//  Author, date...: Yves Lessard , 25-Avril-2001.
//
//  Modifications..:
//
//  Arguments
//  Name                Type     Acces   Description
//  ------------------  -------  ------  -------------------------------------
//  szKey               LPCTSTR     R    The Key Value
//  DeleteValue			BOOL		W	 TRUE->Success else FALSE
////////////////////////////////////////////////////////////////////////////////
BOOL CRegisterWIN32::DeleteValue(LPCTSTR szKey)
{
	BOOL bResult = FALSE;
	try
	{
		//** The Key Value to Delete
		if ( RegDeleteValue(m_hKey, szKey) == ERROR_SUCCESS )
			bResult= TRUE;
	}
	catch(...)
	{	
		bResult = FALSE;
	}
	return bResult;
}


////////////////////////////////////////////////////////////////////////////////
//  Function.......:  DeleteKey
//
//  Description....:  Delete Key and SubKey
//					  With WinNT we need a special Function 
//					  You don't need to use Open for this Function
//           Example: MyReg.DeleteKey(HKEY_CURRENT_USER, _T("Software\\SevySoft"));
//                    The Function will extract the Path = Software
//                    Then delete the Key SevySoft with all SubDir and Value Inside
//            	
//
//  Author, date...: Yves Lessard , 07-Fev-2001.
//
//  Modifications..:
//
//  Arguments
//  Name                Type     Acces   Description
//  ------------------  -------  ------  -------------------------------------
//  hKeyRoot            HKEY       R      The KeyRoot
//  szPath              LPCTSTR    R      Key or SubKey
//  DeleteKey           BOOL       W      TRUE->OK Else FALSE
////////////////////////////////////////////////////////////////////////////////
BOOL CRegisterWIN32::DeleteKey(HKEY hKeyRoot, LPCTSTR szPath)
{
	BOOL bResult  = FALSE;
	try
	{
		TCHAR *pszSubKey = new TCHAR[_MAX_PATH * sizeof(TCHAR)];
		TCHAR *pszPath = new TCHAR[_MAX_PATH * sizeof(TCHAR)];
		TCHAR *pDest=NULL;
		//** If Path empty we get out
		if( _tcslen(szPath) * sizeof(TCHAR) == 0)
			goto Exit_Point;
		
		pDest= _tcsrchr(szPath, '\\');
		if (pDest != NULL)
		{
			//** Must extract the Last Key
			pDest++;
			//** Copy the Key or SubKey to delete
			_tcscpy(pszSubKey, pDest);
			//** We must now extract the Path
			pDest--;
			int iCount=0;
			int Result=0;
			Result = pDest - szPath ;
			do
			{
				pszPath[iCount] = szPath[iCount];
				iCount++;
			}while ( iCount < Result );
			//** Add end of string mark
			pszPath[iCount]= '\0';
		}
		else
		{
			//** We want to delete a path
			_tcscpy(pszSubKey, szPath);
			//** There is no path
			_tcscpy(pszPath, _T(""));
		}
		HKEY hKey;
		//** Check if Path exist ...
		if ( RegOpenKeyEx(hKeyRoot, pszPath, 0L, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS )
			goto Exit_Point;

		//** Check NT or 2000
		if( GetOSversion() )
			if ( m_dTemp == 1 )
			{
				//** NT system must use Recursive Delete
				if( DeleteNTway(hKey, pszSubKey) )
					bResult = TRUE;
			}
			else
			{
				//** Windows 95 or 98
				if ( RegDeleteKey(hKey , pszSubKey) == ERROR_SUCCESS )
					bResult= TRUE;
			}

			RegCloseKey(hKey);
Exit_Point:
			delete pszSubKey, pszPath;
		}
	catch(...)
	{
		bResult = FALSE;
	}
	return bResult;
}

////////////////////////////////////////////////////////////////////////////////
//  Function.......: Read LPBYTE 
//
//  Description....: Read a Byte array  
//                   Most of the times we know how many Bytes we need
//          Example: BYTE MyBytes[25];
//                   DWORD dwByteRead;
//					 MyReg.Read("ArrayKey", MyBytes, dwByteRead);
//      
//
//  Author, date...: Yves Lessard , 24-Avril-2001.
//
//  Modifications..:
//
//  Arguments
//  Name                Type     Acces   Description
//  ------------------  -------  ------  -------------------------------------
//  szKey               LPCTSTR     R    The Key
//  pValue              LPBYTE      W    The Byte Array
//  pnLen               DWORD       W    The number of Bytes readed
//  Read				BOOL		W	 TRUE->Success else FALSE  
////////////////////////////////////////////////////////////////////////////////
BOOL CRegisterWIN32::Read(LPCTSTR szKey, LPBYTE pValue, DWORD &pnLen)
{
	BOOL bResult = FALSE;
	try
	{
		if ( VerifyKey() )
		{
			if ( RegQueryValueEx(m_hKey, szKey, NULL, NULL, pValue, &pnLen) == ERROR_SUCCESS )
				bResult = TRUE;
		}
	}
	catch(...)
	{
		bResult = FALSE;
	}
   
	return bResult;
}


//*********************
//  Private Functions
//*********************

////////////////////////////////////////////////////////////////////////////////
//  Function.......: VerifyKey
//
//  Description....: Private function  We check if a Key exist  
//					 We already have the RootKey & Path
//                  
//
//  Author, date...: Yves Lessard , 24-Avril-2001.
//
//  Modifications..:
//
//  Arguments
//  Name                Type     Acces   Description
//  ------------------  -------  ------  -------------------------------------
//  VerifyKey           BOOL        W    TRUE-> OK else FALSE 
////////////////////////////////////////////////////////////////////////////////
BOOL CRegisterWIN32::VerifyKey()
{
	BOOL bResult = FALSE;
	try
	{
		if ( RegOpenKeyEx(m_RootKey, m_pszPath, 0L, KEY_ALL_ACCESS, &m_hKey) == ERROR_SUCCESS )
			//** Success the Key exist
			bResult= TRUE;
	}
		catch(...)
	{
		bResult = FALSE;
	}

	return bResult;
}

////////////////////////////////////////////////////////////////////////////////
//  Function.......: StringIN
//
//  Description....: Private function for String reading
//                  
//
//  Author, date...: Yves Lessard , 24-Avril-2001.
//
//  Modifications..:
//
//  Arguments
//  Name                Type     Acces   Description
//  ------------------  -------  ------  -------------------------------------
//  szKey               LPCTSTR     R    The Key to read
//  StringIN            BOOL        W    TRUE->OK Else FALSE 
////////////////////////////////////////////////////////////////////////////////
BOOL CRegisterWIN32::StringIN(LPCTSTR szKey)
{
	BOOL bResult = FALSE;
	try
	{
		if ( _tcsclen(szKey)* sizeof(TCHAR) > 0 )
		{
			//** We can do the writing
			DWORD dwType;
			DWORD dwSize = MAX_PATH;
			if( RegQueryValueEx(m_hKey, szKey, 0, &dwType,
							(BYTE*)m_szTemp, &dwSize) == ERROR_SUCCESS)
							//** No error
							bResult = TRUE;
		}
	}
	catch(...)
	{
		bResult = FALSE;
	}
	return bResult;
}

////////////////////////////////////////////////////////////////////////////////
//  Function.......: SetValue
//
//  Description....: Private function for DWORD & Int writing
//                  
//
//  Author, date...: Yves Lessard , 24-Avril-2001.
//
//  Modifications..:
//
//  Arguments
//  Name                Type     Acces   Description
//  ------------------  -------  ------  -------------------------------------
//  SetValue            BOOL       W      TRUE->OK Else FALSE  
////////////////////////////////////////////////////////////////////////////////
BOOL CRegisterWIN32::SetValue(LPCTSTR szKey, DWORD dVal)
{
	BOOL bResult = FALSE;
	try
	{
		//** Write the Value
		if( RegSetValueEx(m_hKey, szKey,0,
						REG_DWORD, (CONST BYTE*)&dVal, sizeof(DWORD)) == ERROR_SUCCESS )
						//** No error
						bResult = TRUE;
	}
	catch(...)
	{
		bResult = FALSE;
	}
	return bResult;	
}

////////////////////////////////////////////////////////////////////////////////
//  Function.......: GetValue 
//
//  Description....: Private function to read a DWORD 
//                   The value is store in m_dTemp
//
//  Author, date...: Yves Lessard , 24-Avril-2001.
//
//  Modifications..:
//
//  Arguments
//  Name                Type     Acces   Description
//  ------------------  -------  ------  -------------------------------------
//  szKey               LPCTSTR    R     The Key
//  GetValue            BOOL       W     TRUE->OK Else FALSE  
////////////////////////////////////////////////////////////////////////////////
BOOL CRegisterWIN32::GetValue(LPCTSTR szKey)
{
	BOOL bResult = FALSE;
	try
	{
		DWORD dwType;
		DWORD dwSize = sizeof(DWORD);
		if( RegQueryValueEx(m_hKey, szKey, 0, &dwType,
						(BYTE*)&m_dTemp, &dwSize) == ERROR_SUCCESS )
						//** Success
						bResult = TRUE;
	}
	catch(...)
	{
		bResult = FALSE;
	}
	return bResult;
}

////////////////////////////////////////////////////////////////////////////////
//  Function.......: GetOSversion 
//
//  Description....: We want to know if NT4 ou Win2000 
//                   Private Fucntion 
//
//  Author, date...: Yves Lessard , 26-Avril-2001.
//
//  Modifications..:
//
//  Arguments
//  Name                Type     Acces   Description
//  ------------------  -------  ------  -------------------------------------
//  GetOSversion        BOOL        W    TRUE->OK 
//                                       We should check m_dTemp after called
//                                       m_dTemp = 1 NT4 ou Win2000
//                                       m_dTemp = 0 Win95, Win98
////////////////////////////////////////////////////////////////////////////////
BOOL CRegisterWIN32::GetOSversion()
{
	BOOL bResult = FALSE;
	m_dTemp = 0;
	try
	{
		OSVERSIONINFO osv;
		osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if (GetVersionEx(&osv))
		{
			// note: szCSDVersion =  service pack  release  
			TCHAR *ServiceRelease = osv.szCSDVersion;
			switch(osv.dwPlatformId)
			{
				case VER_PLATFORM_WIN32_NT: 
					m_dTemp=1;
					bResult = TRUE;
					break;
				default:
					m_dTemp=0;
					bResult = TRUE;
					break;
			}   
		}
	}
	catch(...)
	{
		bResult = FALSE;
	}
	return bResult;
}

////////////////////////////////////////////////////////////////////////////////
//  Function.......:  CreateKey
//
//  Description....:  (Private Fucntion) From m_RootKey and m_pszPath and m_hKey
//                    We create the Key path 
//                  
//
//  Author, date...: Yves Lessard , 24-Avril-2001.
//
//  Modifications..:
//
//  Arguments
//  Name                Type     Acces   Description
//  ------------------  -------  ------  -------------------------------------
//  CreateKey            BOOL      W     TRUE-> OK else FALSE 
////////////////////////////////////////////////////////////////////////////////
BOOL CRegisterWIN32::CreateKey()
{
	BOOL bResult = FALSE;
	try
	{	
		if ( m_hKey )
			RegCloseKey(m_hKey);
		LONG lresult=0;
		lresult= RegCreateKeyEx(m_RootKey, m_pszPath, 0, 0,
							REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,
							0, &m_hKey, 0);
		if ( lresult == ERROR_SUCCESS ) 
			//** Creating the key was successfull
			bResult= TRUE;
	}
		catch(...)
	{
		bResult = FALSE;
	}

	return bResult;
}

////////////////////////////////////////////////////////////////////////////////
//  Function.......: DeleteNTway 
//
//  Description....: Delete each Subkey  
//                   Thsi is a recursive Function 
//
//  Author, date...: Yves Lessard , 17-Apr-2001.
//
//  Modifications..:
//
//  Arguments
//  Name                Type     Acces   Description
//  ------------------  -------  ------  -------------------------------------
//  hKey				HKEY		R    The RootKey
//  szSubKey            LPCTSTR		R    The Subkey to Delete
//  DeleteNTway			BOOL		W	 TRUE-> Success else FALSE 
////////////////////////////////////////////////////////////////////////////////
BOOL CRegisterWIN32::DeleteNTway(HKEY hKey, LPCTSTR szSubKey)
{
	BOOL bResult = FALSE;
	try
	{
		HKEY lhKey;
		FILETIME ft;
		LONG lResult;
		DWORD dwKeyLen;
		TCHAR *pszTemp = new TCHAR[_MAX_PATH * sizeof(TCHAR)];

		//** If Path empty we get out
		if( _tcslen(szSubKey) * sizeof(TCHAR) == 0)
			goto Exit_Point;
		lResult =RegOpenKeyEx(hKey, szSubKey, 0L, KEY_ENUMERATE_SUB_KEYS, &lhKey);
		if (lResult == ERROR_SUCCESS )
		{
			//** So far the Key exist
			do
			{
				dwKeyLen = _MAX_PATH * sizeof(TCHAR);
				lResult = RegEnumKeyEx(lhKey, 0, pszTemp, &dwKeyLen, NULL, NULL,
						  NULL, &ft);
				switch (lResult)
				{
					case ERROR_NO_MORE_ITEMS:
						//** No more Subkey so delete the base
						if ( RegDeleteKey(hKey , szSubKey) == ERROR_SUCCESS )
						{
							bResult= TRUE;
							goto Exit_Point;
						}
						break;
					case ERROR_SUCCESS:
						if( DeleteNTway(lhKey, pszTemp) )
							bResult = TRUE;
						break;
				}
			}while( lResult == ERROR_SUCCESS);
			RegCloseKey(lhKey);
		}

Exit_Point:
		delete pszTemp;
	}
	catch(...)
	{
		bResult = FALSE;
	}
	return bResult;
}
