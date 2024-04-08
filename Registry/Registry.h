#pragma once

// Если какая либо функция возвращает FALSE то она автоматически пишет в TRACE 
// описание ошибки, вызывая REGTRACE

class CRegistry
{
public:
	CRegistry(void);
	~CRegistry(void);

	// Загружает значение из реестра. Если такого значения нет загружает dwDefault
	static BOOL LoadDWORD(LPCTSTR szKey, LPCTSTR szValueName, LPDWORD lpdwData, DWORD dwDefault = 0);
	// Сохраняет значение в реестре, перезаписывая существующее или создавая новое
	static BOOL SaveDWORD(LPCTSTR szKey, LPCTSTR szValueName, DWORD dwData);
	// Загружает значение в реестре, перезаписывая существующее или создавая новое
	static BOOL LoadString(LPCTSTR szKey, LPCTSTR szValueName, CString& csData, LPCTSTR szDefault);
	// Сохраняет значение в реестре, перезаписывая существующее или создавая новое
	static BOOL SaveString(LPCTSTR szKey, LPCTSTR szValueName, LPCTSTR szData);
	// Загружает значение в реестре, перезаписывая существующее или создавая новое
	static BOOL LoadBOOL(LPCTSTR szKey, LPCTSTR szValueName, LPBOOL lpbData, BOOL bDefault);
	// Сохраняет значение в реестре, перезаписывая существующее или создавая новое
	static BOOL SaveBOOL(LPCTSTR szKey, LPCTSTR szValueName, BOOL bData);
	

	// Эквивалент API только принимает нормальный путь и возвращает BOOL
	static BOOL RegOpenKeyEx(LPCTSTR szKey,DWORD ulOptions, REGSAM samDesired, PHKEY phkResult);
	// Эквивалент API только принимает нормальный путь и возвращает BOOL
	static BOOL RegOpenKey(LPCTSTR szKey, PHKEY phkResult);
	// Эквивалент API только возвращает BOOL  
	static BOOL RegCloseKey(HKEY hKey);
	// Эквивалент API только возвращает BOOL 
	static BOOL RegQueryInfoKey(HKEY hKey, LPTSTR lpClass, LPDWORD lpcClass, LPDWORD lpReserved, LPDWORD lpcSubKeys, LPDWORD lpcMaxSubKeyLen, LPDWORD lpcMaxClassLen, LPDWORD lpcValues, LPDWORD lpcMaxValueNameLen, LPDWORD lpcMaxValueLen, LPDWORD lpcbSecurityDescriptor, PFILETIME lpftLastWriteTime);
	// Эквивалент API только возвращает BOOL
	static BOOL RegEnumKeyEx(HKEY hKey, DWORD dwIndex, LPTSTR lpName, LPDWORD lpcName, LPDWORD lpReserved, LPTSTR lpClass, LPDWORD lpcClass, PFILETIME lpftLastWriteTime);
	// Эквивалент API только возвращает BOOL
	static BOOL RegCreateKeyEx(LPCTSTR szKey, DWORD Reserved, LPTSTR lpClass, DWORD dwOptions, REGSAM samDesired, LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition);
	// Эквивалент API только возвращает BOOL
	static BOOL RegEnumValue(HKEY hKey, DWORD dwIndex, LPTSTR lpValueName, LPDWORD lpcValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
	// Эквивалент API только возвращает BOOL
	static BOOL RegDeleteKey(LPCTSTR szKey);


private:
	// Выводит информацию об ошибках реестра
	static void REGTRACE(LONG lErrorCode);
	// Разбивает путь на дескриптор главного ключа и относительный путь
	static BOOL SplitPath(LPCTSTR szKey, PHKEY phKey, CString& csSubKey);

};
