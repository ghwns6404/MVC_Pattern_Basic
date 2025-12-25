#include <windows.h>
#include <SHLOBJ.H>
#include "util.h"

// Src 폴더의 모든 파일과 서브 디렉토리를 Dest로 복사한다. 
// XCopy(Dir1, Dir2)형태로 사용한다.
BOOL XCopy(LPCTSTR Src, LPCTSTR Dest)
{
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	BOOL bResult=TRUE;
	TCHAR WildCard[MAX_PATH];
	TCHAR SrcFile[MAX_PATH];
	TCHAR DestFile[MAX_PATH];

	wsprintf(WildCard,"%s\\*.*",Src);
	CreateDirectory(Dest,NULL);
	hSrch=FindFirstFile(WildCard,&wfd);
	if (hSrch == INVALID_HANDLE_VALUE) 
		return FALSE;
	while (bResult) {
		wsprintf(SrcFile,"%s\\%s",Src,wfd.cFileName);
		wsprintf(DestFile,"%s\\%s",Dest,wfd.cFileName);
		// 서브 디렉토리가 발견되면 서브 디렉토리를 복사한다.
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (lstrcmp(wfd.cFileName,".") && lstrcmp(wfd.cFileName,"..")) {
				XCopy(SrcFile, DestFile);
			}
		} else {
			CopyFile(SrcFile,DestFile,FALSE);
		}
		bResult=FindNextFile(hSrch,&wfd);
	}
	FindClose(hSrch);
	return TRUE;
}

// hWnd 윈도우를 화면 중앙으로 보낸다.
void CenterWindow(HWND hWnd)
{
	RECT rcTemp1, rcTemp2 ;
	LONG iX, iY, iWidth, iHeight ;

	GetWindowRect(GetDesktopWindow(),&rcTemp1) ;
	GetWindowRect(hWnd,&rcTemp2) ;
	iWidth = rcTemp2.right - rcTemp2.left ;
	iHeight = rcTemp2.bottom - rcTemp2.top ;
	iX = LONG((rcTemp1.right - iWidth) / 2) ;
	iY = LONG((rcTemp1.bottom - iHeight) / 2) ;
	MoveWindow(hWnd,iX,iY,iWidth,iHeight,TRUE) ;
}

int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData)
{
	switch (uMsg) {
	case BFFM_INITIALIZED:
		if (lpData != NULL) {
			SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)lpData);
		}
		break;
	}
	return 0;
}

BOOL BrowseFolder(HWND hParent, LPCTSTR szTitle, LPCTSTR StartPath, TCHAR *szFolder)
{
	LPMALLOC pMalloc;
	LPITEMIDLIST pidl;
	BROWSEINFO bi;

	bi.hwndOwner = hParent;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle = szTitle ;
	bi.ulFlags = 0;
	bi.lpfn = BrowseCallbackProc;;
	bi.lParam = (LPARAM)StartPath;
						
	pidl = SHBrowseForFolder(&bi);

	if (pidl == NULL) {
		return FALSE;
	}
	SHGetPathFromIDList(pidl, szFolder);

	if (SHGetMalloc(&pMalloc) != NOERROR) {
		return FALSE;
	}
	pMalloc->Free(pidl);
	pMalloc->Release();
	return TRUE;
}

