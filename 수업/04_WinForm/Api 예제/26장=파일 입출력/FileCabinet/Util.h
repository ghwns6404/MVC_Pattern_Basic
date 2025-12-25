#define FIF_DEEP 1
#define FIF_DIRFILTER 2
#define FIF_INCHID 4
typedef int (*FIFCALLBACK)(TCHAR *, DWORD, LPVOID);
BOOL BrowseFolder(HWND hParent, LPCTSTR szTitle, LPCTSTR StartPath, TCHAR *szFolder);
BOOL IsMatch(TCHAR *Path, TCHAR *Pattern);
void FindInFiles(TCHAR *Path, TCHAR *Pattern, DWORD Flags, FIFCALLBACK pCallBack, LPVOID pCustom);
extern BOOL bContFIF;
void SavePosition(HWND hWnd, TCHAR *Key);
void LoadPosition(HWND hWnd, TCHAR *Key, RECT *Def/*=NULL*/);
void MoveToParentCenter(HWND hWnd);
int TreeView_GetDepth(HWND hTree,HTREEITEM hItem);
int TreeView_GetChildCount(HWND hTree,HTREEITEM hItem);
LPARAM TreeView_GetParam(HWND hTree,HTREEITEM hItem);
void TreeView_GetText(HWND hTree,HTREEITEM hItem,TCHAR *Text,int len);
void TreeView_SetText(HWND hTree,HTREEITEM hItem,TCHAR *Text);
int ListView_GetItemImage(HWND hList,int idx,int sub);
int ListView_GetParam(HWND hList,int idx);
DWORD MyGetAbsSecond(SYSTEMTIME st);
void MyAbsSecondToSystem(DWORD Abs, SYSTEMTIME &st);
void MakeAssociate(TCHAR *ext, TCHAR *desc);
