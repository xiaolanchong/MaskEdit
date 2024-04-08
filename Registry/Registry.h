#pragma once

// ≈сли кака¤ либо функци¤ возвращает FALSE то она автоматически пишет в TRACE 
// описание ошибки, вызыва¤ REGTRACE

class CRegistry
{
public:
	CRegistry(void);
	~CRegistry(void);

	// «агружает значение из реестра. ≈сли такого значени¤ нет загружает dwDefault
	static BOOL LoadDWORD(LPCTSTR szKey, LPCTSTR szValueName, LPDWORD lpdwData, DWORD dwDefault = 0);
	// —охран¤ет значение в реестре, перезаписыва¤ существующее или создава¤ новое
	static BOOL SaveDWORD(LPCTSTR szKey, LPCTSTR szValueName, DWORD dwData);
	// «агружает значение в реестре, перезаписыва¤ существующее или создава¤ новое
	static BOOL LoadString(LPCTSTR szKey, LPCTSTR szValueName, CString& csData, LPCTSTR szDefault);
	// —охран¤ет значение в реестре, перезаписыва¤ существующее или создава¤ новое
	static BOOL SaveString(LPCTSTR szKey, LPCTSTR szValueName, LPCTSTR szData);
	// «агружает значение в реестре, перезаписыва¤ существующее или создава¤ новое
	static BOOL LoadBOOL(LPCTSTR szKey, LPCTSTR szValueName, LPBOOL lpbData, BOOL bDefault);
	// —охран¤ет значение в реестре, перезаписыва¤ существующее или создава¤ новое
	static BOOL SaveBOOL(LPCTSTR szKey, LPCTSTR szValueName, BOOL bData);
	

	// Ёквивалент API только принимает нормальный путь и возвращает BOOL
	static BOOL RegOpenKeyEx(LPCTSTR szKey,DWORD ulOptions, REGSAM samDesired, PHKEY phkResult);
	// Ёквивалент API только принимает нормальный путь и возвращает BOOL
	static BOOL RegOpenKey(LPCTSTR szKey, PHKEY phkResult);
	// Ёквивалент API только возвращает BOOL  
	static BOOL RegCloseKey(HKEY hKey);
	// Ёквивалент API только возвращает BOOL 
	static BOOL RegQueryInfoKey(HKEY hKey, LPTSTR lpClass, LPDWORD lpcClass, LPDWORD lpReserved, LPDWORD lpcSubKeys, LPDWORD lpcMaxSubKeyLen, LPDWORD lpcMaxClassLen, LPDWORD lpcValues, LPDWORD lpcMaxValueNameLen, LPDWORD lpcMaxValueLen, LPDWORD lpcbSecurityDescriptor, PFILETIME lpftLastWriteTime);
	// Ёквивалент API только возвращает BOOL
	static BOOL RegEnumKeyEx(HKEY hKey, DWORD dwIndex, LPTSTR lpName, LPDWORD lpcName, LPDWORD lpReserved, LPTSTR lpClass, LPDWORD lpcClass, PFILETIME lpftLastWriteTime);
	// Ёквивалент API только возвращает BOOL
	static BOOL RegCreateKeyEx(LPCTSTR szKey, DWORD Reserved, LPTSTR lpClass, DWORD dwOptions, REGSAM samDesired, LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition);
	// Ёквивалент API только возвращает BOOL
	static BOOL RegEnumValue(HKEY hKey, DWORD dwIndex, LPTSTR lpValueName, LPDWORD lpcValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
	// Ёквивалент API только возвращает BOOL
	static BOOL RegDeleteKey(LPCTSTR szKey);


private:
	// ¬ыводит информацию об ошибках реестра
	static void REGTRACE(LONG lErrorCode);
	// –азбивает путь на дескриптор главного ключа и относительный путь
	static BOOL SplitPath(LPCTSTR szKey, PHKEY phKey, CString& csSubKey);

};
