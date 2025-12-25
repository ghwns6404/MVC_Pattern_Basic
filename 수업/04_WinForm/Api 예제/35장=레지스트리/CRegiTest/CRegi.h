#define SHLM HKEY_LOCAL_MACHINE
#define SHCU HKEY_CURRENT_USER
#define SHCR HKEY_CLASSES_ROOT

class CRegi
{
private:
	HKEY HiveKey;
	TCHAR Root[MAX_PATH];

public:
	CRegi(LPCTSTR aRoot,HKEY aHive=SHCU) {
		HiveKey=aHive;
		lstrcpy(Root,aRoot);
		if (Root[lstrlen(Root)] == '\\') {
			Root[lstrlen(Root)]=0;
		}
	}

	static int ReadInt(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, INT nDefault);
	static bool ReadString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpDefault, LPTSTR lpRet, DWORD nSize);
	static bool WriteInt(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, UINT nData);
	static bool WriteString(HKEY hKey, LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData);
	static bool DeleteKey(HKEY hKey, LPCTSTR lpKey);

	int ReadInt(LPCTSTR lpKey, LPCTSTR lpValue, INT nDefault);
	bool ReadString(LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpDefault, LPTSTR lpRet, DWORD nSize);
	bool WriteInt(LPCTSTR lpKey, LPCTSTR lpValue, UINT nData);
	bool WriteString(LPCTSTR lpKey, LPCTSTR lpValue, LPCTSTR lpData);
};
