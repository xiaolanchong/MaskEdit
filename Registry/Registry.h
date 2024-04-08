#pragma once

// ���� ����� ���� ������� ���������� FALSE �� ��� ������������� ����� � TRACE 
// �������� ������, ������� REGTRACE

class CRegistry
{
public:
	CRegistry(void);
	~CRegistry(void);

	// ��������� �������� �� �������. ���� ������ �������� ��� ��������� dwDefault
	static BOOL LoadDWORD(LPCTSTR szKey, LPCTSTR szValueName, LPDWORD lpdwData, DWORD dwDefault = 0);
	// ��������� �������� � �������, ������������� ������������ ��� �������� �����
	static BOOL SaveDWORD(LPCTSTR szKey, LPCTSTR szValueName, DWORD dwData);
	// ��������� �������� � �������, ������������� ������������ ��� �������� �����
	static BOOL LoadString(LPCTSTR szKey, LPCTSTR szValueName, CString& csData, LPCTSTR szDefault);
	// ��������� �������� � �������, ������������� ������������ ��� �������� �����
	static BOOL SaveString(LPCTSTR szKey, LPCTSTR szValueName, LPCTSTR szData);
	// ��������� �������� � �������, ������������� ������������ ��� �������� �����
	static BOOL LoadBOOL(LPCTSTR szKey, LPCTSTR szValueName, LPBOOL lpbData, BOOL bDefault);
	// ��������� �������� � �������, ������������� ������������ ��� �������� �����
	static BOOL SaveBOOL(LPCTSTR szKey, LPCTSTR szValueName, BOOL bData);
	

	// ���������� API ������ ��������� ���������� ���� � ���������� BOOL
	static BOOL RegOpenKeyEx(LPCTSTR szKey,DWORD ulOptions, REGSAM samDesired, PHKEY phkResult);
	// ���������� API ������ ��������� ���������� ���� � ���������� BOOL
	static BOOL RegOpenKey(LPCTSTR szKey, PHKEY phkResult);
	// ���������� API ������ ���������� BOOL  
	static BOOL RegCloseKey(HKEY hKey);
	// ���������� API ������ ���������� BOOL 
	static BOOL RegQueryInfoKey(HKEY hKey, LPTSTR lpClass, LPDWORD lpcClass, LPDWORD lpReserved, LPDWORD lpcSubKeys, LPDWORD lpcMaxSubKeyLen, LPDWORD lpcMaxClassLen, LPDWORD lpcValues, LPDWORD lpcMaxValueNameLen, LPDWORD lpcMaxValueLen, LPDWORD lpcbSecurityDescriptor, PFILETIME lpftLastWriteTime);
	// ���������� API ������ ���������� BOOL
	static BOOL RegEnumKeyEx(HKEY hKey, DWORD dwIndex, LPTSTR lpName, LPDWORD lpcName, LPDWORD lpReserved, LPTSTR lpClass, LPDWORD lpcClass, PFILETIME lpftLastWriteTime);
	// ���������� API ������ ���������� BOOL
	static BOOL RegCreateKeyEx(LPCTSTR szKey, DWORD Reserved, LPTSTR lpClass, DWORD dwOptions, REGSAM samDesired, LPSECURITY_ATTRIBUTES lpSecurityAttributes, PHKEY phkResult, LPDWORD lpdwDisposition);
	// ���������� API ������ ���������� BOOL
	static BOOL RegEnumValue(HKEY hKey, DWORD dwIndex, LPTSTR lpValueName, LPDWORD lpcValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData);
	// ���������� API ������ ���������� BOOL
	static BOOL RegDeleteKey(LPCTSTR szKey);


private:
	// ������� ���������� �� ������� �������
	static void REGTRACE(LONG lErrorCode);
	// ��������� ���� �� ���������� �������� ����� � ������������� ����
	static BOOL SplitPath(LPCTSTR szKey, PHKEY phKey, CString& csSubKey);

};
