
#include "stdafx.h"
#include "Registry\registry.h"


CRegistry::CRegistry(void)
{
}

CRegistry::~CRegistry(void)
{
}

// Загружает значение из реестра. Если такого значения нет загружает dwDefault
BOOL CRegistry::LoadDWORD(LPCTSTR szKey, LPCTSTR szValueName, LPDWORD lpdwData, DWORD dwDefault)
{
	
	CString csKey(szKey);
	if(csKey.IsEmpty() || csKey.GetLength() == 0)
		return FALSE;


	CString csSubKey;
	HKEY hRootKey = NULL;
	if(!CRegistry::SplitPath(szKey, &hRootKey, csSubKey))
		return FALSE;


	// Set default
	if(lpdwData)
	{
		(*lpdwData) = dwDefault;
	}

	// Open key
	HKEY hKey = NULL;
	if(::RegOpenKey(hRootKey, csSubKey, &hKey) != ERROR_SUCCESS)
		return TRUE;

	// Read value
	DWORD cbData = sizeof(DWORD);
	DWORD dwData = 0;
	if(::RegQueryValueEx(hKey, szValueName, NULL, NULL, (LPBYTE)&dwData, &cbData) == ERROR_SUCCESS)
	{
		if(lpdwData)
			(*lpdwData) = dwData;
	}

	// Close key
	LONG retCode = ::RegCloseKey(hKey);
	if(retCode != ERROR_SUCCESS)
	{
		CRegistry::REGTRACE(retCode);
		return FALSE;
	}

	return TRUE;
}

// Сохраняет значение в реестре, перезаписывая существующее или создавая новое
BOOL CRegistry::SaveDWORD(LPCTSTR szKey, LPCTSTR szValueName, DWORD dwData)
{

	CString csKey(szKey);
	if(csKey.IsEmpty() || csKey.GetLength() == 0)
		return FALSE;


	CString csSubKey;
	HKEY hRootKey = NULL;
	if(!CRegistry::SplitPath(szKey, &hRootKey, csSubKey))
		return FALSE;


	// Create or open key
	HKEY rgKey;
	LONG retCode = ::RegCreateKeyEx(hRootKey, csSubKey, 0, NULL, 0,
		KEY_ALL_ACCESS, NULL, &rgKey, NULL);
	if(retCode != ERROR_SUCCESS)
	{
		CRegistry::REGTRACE(retCode);
		return FALSE;
	}

	// Create or modify option
	retCode = ::RegSetValueEx(rgKey, szValueName, 0, REG_DWORD, 
		(LPBYTE)&dwData, sizeof(DWORD));    
	if(retCode != ERROR_SUCCESS)
	{
		CRegistry::REGTRACE(retCode);
		return FALSE;
	}

	return TRUE;
}


// Сохраняет значение в реестре, перезаписывая существующее или создавая новое
BOOL CRegistry::LoadString(LPCTSTR szKey, LPCTSTR szValueName, CString& csData, LPCTSTR szDefault)
{
	
	CString csKey(szKey);
	if(!szKey || !szValueName || csKey.IsEmpty() || csKey.GetLength() == 0 )
		return FALSE;

	CString csSubKey;
	HKEY hRootKey = NULL;
	if(!CRegistry::SplitPath(szKey, &hRootKey, csSubKey))
		return FALSE;

	// Set default
	if(szDefault)
		csData.Format(_T("%s"), szDefault);

	// Open key
	HKEY hKey = NULL;
	if(::RegOpenKey(hRootKey, csSubKey, &hKey) != ERROR_SUCCESS)
		return TRUE;


	// Read value
	DWORD cbData = 0;
	DWORD dwType = 0;
	LONG retCode = ::RegQueryValueEx(hKey, szValueName, NULL, &dwType, (LPBYTE)NULL, &cbData);
	if(retCode == ERROR_SUCCESS)
	{
		if((dwType == REG_SZ || dwType == REG_MULTI_SZ || dwType == REG_EXPAND_SZ) && cbData)
		{
 
			LPTSTR szBuffer = new TCHAR[cbData];
			szBuffer[cbData-1] = _T('\0');

			if(szBuffer)
			{
				// 
				// При чтении cтроки указывается количество символов которое надо 
				// загрузить, включая терминантный ноль 
				//
				
				if(::RegQueryValueEx(hKey, szValueName, NULL, NULL, (LPBYTE)szBuffer, &cbData) == ERROR_SUCCESS)
				{
					csData.Format(_T("%s"), (LPCTSTR)szBuffer);
				}	 

				// Переполнение буффера
				if(szBuffer[cbData-1] != _T('\0'))
					ASSERT(FALSE);
				
				SAFE_DELETE_ARRAY(szBuffer);
			}
		}
	}
	


	// Close key
	retCode = ::RegCloseKey(hKey);
	if(retCode != ERROR_SUCCESS)
	{
		CRegistry::REGTRACE(retCode);
		return FALSE;
	}



	return TRUE;
}

// Сохраняет значение в реестре, перезаписывая существующее или создавая новое
BOOL CRegistry::SaveString(LPCTSTR szKey, LPCTSTR szValueName, LPCTSTR szData)
{

	CString csKey(szKey);
	if(!szKey || !szValueName || csKey.IsEmpty() || csKey.GetLength() == 0 )
		return FALSE;

	CString csSubKey;
	HKEY hRootKey = NULL;
	if(!CRegistry::SplitPath(szKey, &hRootKey, csSubKey))
		return FALSE;

	// Create or open key
	HKEY rgKey;
	LONG retCode = ::RegCreateKeyEx(hRootKey, csSubKey, 0, NULL, 0,
		KEY_ALL_ACCESS, NULL, &rgKey, NULL);
	if(retCode != ERROR_SUCCESS)
	{
		CRegistry::REGTRACE(retCode);
		return FALSE;
	}

	// Create or modify option
	if(szData)
	{
		// 
		// При записи cтроки указывается размер буффера 
		// выделенного под строку в байтах 
		//
		DWORD cbBufferSize = (lstrlen(szData) * sizeof(TCHAR)) + sizeof(TCHAR);
		
		retCode = ::RegSetValueEx(rgKey, szValueName, 0, REG_SZ, 
			(LPBYTE)szData, cbBufferSize);    
		if(retCode != ERROR_SUCCESS)
		{
			CRegistry::REGTRACE(retCode);
			return FALSE;
		}
	}
	else
	{
		retCode = ::RegSetValueEx(rgKey, szValueName, 0, REG_SZ, 
			NULL, 0);    
		if(retCode != ERROR_SUCCESS)
		{
			CRegistry::REGTRACE(retCode);
			return FALSE;
		}
	}


	return TRUE;
}

// Загружает значение в реестре, перезаписывая существующее или создавая новое
BOOL CRegistry::LoadBOOL(LPCTSTR szKey, LPCTSTR szValueName, LPBOOL lpbData, BOOL bDefault)
{
	CString csKey(szKey);
	if(!szKey || !szValueName || csKey.IsEmpty() || csKey.GetLength() == 0 )
		return FALSE;

	CString csSubKey;
	HKEY hRootKey = NULL;
	if(!CRegistry::SplitPath(szKey, &hRootKey, csSubKey))
		return FALSE;

	// Set default
	
	if(lpbData)
		(*lpbData) = bDefault;
		

	// Open key
	HKEY hKey = NULL;
	if(::RegOpenKey(hRootKey, csSubKey, &hKey) != ERROR_SUCCESS)
		return TRUE;

	if(hKey && lpbData)
	{
		// Boolean saved in registry as DWORD, so Load DWORD
		DWORD cbData = sizeof(DWORD);
		DWORD Data = 0;
		LONG retCode = RegQueryValueEx(hKey, szValueName, NULL, NULL, (LPBYTE)&Data, &cbData);
		if(retCode == ERROR_SUCCESS)
		{
			// Convert DWORD to BOOL
			if(lpbData)
				(*lpbData) = (BOOL)Data;
		}
		else
		{
			CRegistry::REGTRACE(retCode);
			return FALSE;
		}
		
		// Close registry key
		retCode = ::RegCloseKey(hKey);

		if(retCode != ERROR_SUCCESS)
		{
			CRegistry::REGTRACE(retCode);
			return FALSE;
		}
	}


	return TRUE;
}

// Сохраняет значение в реестре, перезаписывая существующее или создавая новое
BOOL CRegistry::SaveBOOL(LPCTSTR szKey, LPCTSTR szValueName, BOOL bData)
{
	CString csKey(szKey);
	if(!szKey || !szValueName || csKey.IsEmpty() || csKey.GetLength() == 0 )
		return FALSE;

	CString csSubKey;
	HKEY hRootKey = NULL;
	if(!CRegistry::SplitPath(szKey, &hRootKey, csSubKey))
		return FALSE;

	// Create or open key
	HKEY rgKey = NULL;
	LONG retCode = ::RegCreateKeyEx(hRootKey, csSubKey, 0, NULL, 0,
		KEY_ALL_ACCESS, NULL, &rgKey, NULL);
	if(retCode != ERROR_SUCCESS)
	{
		CRegistry::REGTRACE(retCode);
		return FALSE;
	}


	if(rgKey)
	{
		
		retCode = ::RegSetValueEx(rgKey, szValueName, 0, REG_BINARY, 
			(LPBYTE)&bData, sizeof(BOOL));    
		if(retCode != ERROR_SUCCESS)
		{
			CRegistry::REGTRACE(retCode);
			return FALSE;
		}
	}

	return TRUE;
}


// Эквивалент API только принимает нормальный путь и возвращает BOOL
BOOL CRegistry::RegOpenKey(LPCTSTR szKey, PHKEY phkResult)
{
	// Разбивает путь на дескриптор главного ключа и относительный путь
	HKEY hRootKey = NULL;
	CString csSubKey;
	BOOL bRes = CRegistry::SplitPath(szKey, &hRootKey, csSubKey);
	if(!bRes)
		return FALSE;

	// Native API
	LONG retCode = ::RegOpenKey(hRootKey, csSubKey, phkResult);
	if(retCode != ERROR_SUCCESS)
	{
		CRegistry::REGTRACE(retCode);
		return FALSE;
	}

	return TRUE;
}




// Эквивалент API только принимает нормальный путь  
BOOL CRegistry::RegOpenKeyEx(LPCTSTR szKey,DWORD ulOptions, REGSAM samDesired, PHKEY phkResult)
{
	// Разбивает путь на дескриптор главного ключа и относительный путь
	HKEY hRootKey = NULL;
	CString csSubKey;
	BOOL bRes = CRegistry::SplitPath(szKey, &hRootKey, csSubKey);
	if(!bRes)
		return FALSE;

	// Native API
	LONG retCode = ::RegOpenKeyEx(hRootKey, csSubKey, ulOptions, samDesired, phkResult);
	if(retCode != ERROR_SUCCESS)
	{
		CRegistry::REGTRACE(retCode);
		return FALSE;
	}

	return TRUE;
}

// Эквивалент API только возвращает BOOL  
BOOL CRegistry::RegCloseKey(HKEY hKey)
{
	LONG retCode = ::RegCloseKey(hKey);
	if(retCode != ERROR_SUCCESS)
	{
		CRegistry::REGTRACE(retCode);
		return FALSE;
	}

	return TRUE;
}

// Эквивалент API только возвращает BOOL 
BOOL CRegistry::RegQueryInfoKey(HKEY hKey, LPTSTR lpClass, LPDWORD lpcClass, LPDWORD lpReserved,
		LPDWORD lpcSubKeys, LPDWORD lpcMaxSubKeyLen, LPDWORD lpcMaxClassLen, LPDWORD lpcValues,
		LPDWORD lpcMaxValueNameLen, LPDWORD lpcMaxValueLen, LPDWORD lpcbSecurityDescriptor,
		PFILETIME lpftLastWriteTime)
{
	LONG retCode = ::RegQueryInfoKey(hKey, lpClass, lpcClass, lpReserved, lpcSubKeys, 
		lpcMaxSubKeyLen, lpcMaxClassLen, lpcValues, lpcMaxValueNameLen, lpcMaxValueLen, 
		lpcbSecurityDescriptor, lpftLastWriteTime);
	if(retCode != ERROR_SUCCESS)
	{
		CRegistry::REGTRACE(retCode);
		return FALSE;
	}

	return TRUE;
}

// Эквивалент API только возвращает BOOL
BOOL CRegistry::RegEnumKeyEx(HKEY hKey, DWORD dwIndex, LPTSTR lpName, LPDWORD lpcName, 
	LPDWORD lpReserved, LPTSTR lpClass, LPDWORD lpcClass, PFILETIME lpftLastWriteTime)
{
	LONG retCode = ::RegEnumKeyEx(hKey, dwIndex, lpName, lpcName, lpReserved, lpClass, 
		lpcClass, lpftLastWriteTime);
	if(retCode != ERROR_SUCCESS)
	{
		CRegistry::REGTRACE(retCode);
		return FALSE;
	}

	return TRUE;
}

// Эквивалент API только возвращает BOOL
BOOL CRegistry::RegCreateKeyEx(LPCTSTR szKey, DWORD Reserved, LPTSTR lpClass, DWORD dwOptions, REGSAM samDesired, LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition)
{
	// Разбивает путь на дескриптор главного ключа и относительный путь
	HKEY hRootKey = NULL;
	CString csSubKey;
	BOOL bRes = CRegistry::SplitPath(szKey, &hRootKey, csSubKey);
	if(!bRes)
		return FALSE;


	LONG retCode = ::RegCreateKeyEx(hRootKey, csSubKey, Reserved, lpClass, dwOptions, samDesired, 
		lpSecurityAttributes, phkResult, lpdwDisposition);
	if(retCode != ERROR_SUCCESS)
	{
		CRegistry::REGTRACE(retCode);
		return FALSE;
	}

	return TRUE;
}

// Эквивалент API только возвращает BOOL
BOOL CRegistry::RegEnumValue(HKEY hKey, DWORD dwIndex, LPTSTR lpValueName, LPDWORD lpcValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)
{
	LONG retCode = ::RegEnumValue(hKey, dwIndex, lpValueName, lpcValueName, lpReserved, lpType, lpData, lpcbData);
	if(retCode != ERROR_SUCCESS)
	{
		CRegistry::REGTRACE(retCode);
		return FALSE;
	}
	
	return TRUE;
}


// Эквивалент API только возвращает BOOL
BOOL CRegistry::RegDeleteKey(LPCTSTR szKey)
{
	// Разбивает путь на дескриптор главного ключа и относительный путь
	HKEY hRootKey = NULL;
	CString csSubKey;

	BOOL bRes = CRegistry::SplitPath(szKey, &hRootKey, csSubKey);
	
	if(!bRes)
		return FALSE;

	// Call native api
	LONG retCode = ::RegDeleteKey(hRootKey, csSubKey);
	
	if(retCode != ERROR_SUCCESS)
	{
		CRegistry::REGTRACE(retCode);
		return FALSE;
	}
	
	return TRUE;
}






// Разбивает путь на дескриптор главного ключа и относительный путь
BOOL CRegistry::SplitPath(LPCTSTR szKey, PHKEY phKey, CString& csSubKey)
{
	if(phKey)
	{
		(*phKey) = NULL;
	}

	CString csKey(szKey);
	if(csKey.IsEmpty() || csKey.GetLength() == 0)
		return FALSE;

	// Длина главной части
	int nLength = csKey.Find(_T('\\'));
	if(nLength == -1)
		nLength = csKey.GetLength();

	CString csSubKeyTmp = csKey.Right(csKey.GetLength() - nLength - 1);
	CString csHKEY = csKey.Left(nLength);
	

	HKEY hKey = NULL;
	if(csHKEY == _T("HKEY_CLASSES_ROOT") || csHKEY == _T("HKCR"))
		hKey = HKEY_CLASSES_ROOT;
	else if(csHKEY == _T("HKEY_LOCAL_MACHINE") || csHKEY == _T("HKLM"))
		hKey = HKEY_LOCAL_MACHINE;
	else if(csHKEY == _T("HKEY_CURRENT_USER") || csHKEY == _T("HKCU"))
		hKey = HKEY_CURRENT_USER;
	else if(csHKEY == _T("HKEY_LOCAL_MACHINE") || csHKEY == _T("HKLM"))
		hKey = HKEY_LOCAL_MACHINE;
	else if(csHKEY == _T("HKEY_USERS") || csHKEY == _T("HKU"))
		hKey = HKEY_USERS;
	else if(csHKEY == _T("HKEY_CURRENT_CONFIG") || csHKEY == _T("HKCC"))
		hKey = HKEY_CURRENT_CONFIG;

	if(!hKey)
		return FALSE;

	if(phKey)
	{
		(*phKey) = hKey;
	}

	if(csSubKey)
	{
		csSubKey = csSubKeyTmp;
	}

	return hKey ? TRUE : FALSE;
}

// Расшифровывает сообщение
void CRegistry::REGTRACE(LONG lErrorCode)
{
	LPVOID lpMsgBuf = NULL;
	if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS, NULL, lErrorCode, 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL))
	{
		return;
	}

	TRACE(_T("%s::Registry::%s\n"), APPNAME, (LPCTSTR)lpMsgBuf);

	// Free the buffer.
	LocalFree(lpMsgBuf);
}

