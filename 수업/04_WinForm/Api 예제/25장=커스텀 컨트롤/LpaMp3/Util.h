#define FIF_DEEP 1
#define FIF_DIRFILTER 2
#define FIF_INCHID 4
typedef int (*FIFCALLBACK)(TCHAR *, DWORD, LPVOID);
void FindInFiles(TCHAR *Path, TCHAR *Pattern, DWORD Flags, FIFCALLBACK pCallBack, LPVOID pCustom);
BOOL IsMatch(TCHAR *Path, TCHAR *Pattern);
extern BOOL bContFIF;
