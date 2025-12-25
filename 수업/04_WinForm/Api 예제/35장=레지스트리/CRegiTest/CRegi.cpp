#include <windows.h>
#include <shlwapi.h>
#include "CRegi.h"

#pragma comment(lib,"shlwapi")

// 정수값을 읽는다.
int CRegi::ReadInt(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, INT nDefault)
{
	HKEY key;
	DWORD dwDisp;
	UINT Result;
	DWORD Size;
	if (RegCreateKeyEx(hKey, lpKey,0,NULL,
		REG_OPTION_NON_VOLATILE, KEY_READ,NULL,&key,&dwDisp)
		!=ERROR_SUCCESS) 
		return 0;
	Size=sizeof(LONG);
	if (RegQueryValueEx(key, lpValue, 0, NULL,(LPBYTE)&Result, &Size)
		!=ERROR_SUCCESS)
		Result=nDefault;
	RegCloseKey(key);
	return Result;
}

// 문자열을 읽는다.
bool CRegi::ReadString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpDefault, LPTSTR lpRet, DWORD nSize)
{
	HKEY key;
	DWORD dwDisp;
	DWORD Size;
	if (RegCreateKeyEx(hKey, lpKey,0,NULL,
		REG_OPTION_NON_VOLATILE, KEY_READ,NULL,&key,&dwDisp)
		!=ERROR_SUCCESS) 
		return false;
	Size=nSize;
	if (RegQueryValueEx(key, lpValue, 0, NULL,(LPBYTE)lpRet, &Size)
		!=ERROR_SUCCESS) {
		lstrcpy(lpRet, lpDefault);
		return FALSE;
	}
	RegCloseKey(key);
	return true;
}

// 정수값을 쓴다.
bool CRegi::WriteInt(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, UINT nData)
{
	HKEY key;
	DWORD dwDisp;
	if (RegCreateKeyEx(hKey, lpKey,0,NULL,
		REG_OPTION_NON_VOLATILE, KEY_WRITE,NULL,&key,&dwDisp)
		!=ERROR_SUCCESS) {
		return false;
	}
	if (RegSetValueEx(key, lpValue,0,REG_DWORD,(LPBYTE)&nData,sizeof(UINT))
		!=ERROR_SUCCESS) 
		return FALSE;
	RegCloseKey(key);
	return true;
}

// 문자열을 쓴다.
bool CRegi::WriteString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData)
{
	HKEY key;
	DWORD dwDisp;
	if (RegCreateKeyEx(hKey, lpKey,0,NULL,
		REG_OPTION_NON_VOLATILE, KEY_WRITE,NULL,&key,&dwDisp)
		!=ERROR_SUCCESS) 
		return false;
	if (RegSetValueEx(key, lpValue,0,REG_SZ,(LPBYTE)lpData,lstrlen(lpData)+1)
		!=ERROR_SUCCESS) 
		return FALSE;
	RegCloseKey(key);
	return true;
}

// 레지스트리 키를 삭제한다.
bool CRegi::DeleteKey(HKEY hKey, LPCTSTR lpKey)
{
	return (SHDeleteKey(hKey,lpKey) == ERROR_SUCCESS);
}

// 멤버 함수들. 생성자에 등록한 하이브와 루트키 아래의 서브키만 액세스한다.
int CRegi::ReadInt(LPCTSTR lpKey, LPCTSTR lpValue, INT nDefault)
{
	TCHAR Key[MAX_PATH];

	wsprintf(Key,"%s\\%s",Root,lpKey);
	return ReadInt(HiveKey,Key,lpValue,nDefault);
}

bool CRegi::ReadString(LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpDefault, LPTSTR lpRet, DWORD nSize)
{
	TCHAR Key[MAX_PATH];

	wsprintf(Key,"%s\\%s",Root,lpKey);
	return ReadString(HiveKey,Key,lpValue,lpDefault,lpRet,nSize);
}

bool CRegi::WriteInt(LPCTSTR lpKey, LPCTSTR lpValue, UINT nData)
{
	TCHAR Key[MAX_PATH];

	wsprintf(Key,"%s\\%s",Root,lpKey);
	return WriteInt(HiveKey,Key,lpValue,nData);
}

bool CRegi::WriteString(LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData)
{
	TCHAR Key[MAX_PATH];

	wsprintf(Key,"%s\\%s",Root,lpKey);
	return WriteString(HiveKey,Key,lpValue,lpData);
}

