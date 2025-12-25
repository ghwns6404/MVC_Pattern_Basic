#include <windows.h>
#include <shlobj.h>
#include "Util.h"
#include "ShReg.h"

int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData)
{
	switch (uMsg)
	{
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

BOOL IsMatch(TCHAR *Path, TCHAR *Pattern)
{
	TCHAR Pat[MAX_PATH];
	TCHAR *t,*p;
	TCHAR Ext[_MAX_EXT];
	TCHAR Name[MAX_PATH];
	TCHAR *p1,*p2;
	BOOL bOther;

	_splitpath(Path,NULL,NULL,Name,Ext);
	lstrcat(Name,Ext);
	CharUpper(Name);

	t=Pattern;
	for (;;) {
		p=Pat;
		while (*t!=';' && *t!=0) {
			*p=*t;
			p++;
			t++;
		}
		*p=0;

		CharUpper(Pat);
		// 패턴이 *.ext 인 경우 확장자만 비교한다. 이렇게 하지 않으면 01.name.ext 파일은 일치하지 않은 것으로 오판한다.
		// 근본적인 문제는 .이 더 이상 파일 이름과 확장자의 구분자가 아니라 파일명의 일부가 될 수 있다는 점이다.
		if (Pat[0]=='*' && Pat[1]=='.') {
			if (lstrcmpi(Pat+1,Ext)==0) {
				return TRUE;
			}
		}
		p1=Name;
		p2=Pat;
		for (;;) {
			if (*p2=='?') {
				p1++;
				p2++;
			} else if (*p2=='*') {
				p2++;
				while (*p1!=*p2 && *p1!=0) 
					p1++;
			} else {
				if (*p1!=*p2) {
					break;
				}
				p1++;
				p2++;
			}

			if (*p1==0 && *p2==0) {
				return TRUE;
			}
			if (*p1!=0 && *p2==0) {
				break;
			}
			if (*p1==0 && *p2!=0) {
				bOther=FALSE;
				while (*p2) {
					if (*p2!='.' && *p2!='*') {
						bOther=TRUE;
					}
					p2++;
				}
				if (bOther==FALSE) {
					return TRUE;
				}
				break;
			}
		}

		if (*t==0) {
			break;
		}
		t++;
	}
	return FALSE;
}

BOOL bContFIF=TRUE;
void FindInFiles(TCHAR *Path, TCHAR *Pattern, DWORD Flags, FIFCALLBACK pCallBack, LPVOID pCustom)
{
	TCHAR SrchPath[MAX_PATH];
	TCHAR szFinded[MAX_PATH];
	WIN32_FIND_DATA wfd;
	HANDLE hSrch;
	BOOL nResult=TRUE;

	lstrcpy(SrchPath, Path);
	if (SrchPath[lstrlen(SrchPath)-1] == '\\') {
		SrchPath[lstrlen(SrchPath)-1]=0;
	}
	lstrcat(SrchPath, "\\*.*");
	hSrch=FindFirstFile(SrchPath,&wfd);
	if (hSrch == INVALID_HANDLE_VALUE) {
		return;
	}

	while (nResult) {
		wsprintf(szFinded,"%s\\%s",Path,wfd.cFileName);
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			// 수정 : 자신 또는 부모인지 조사
			if (lstrcmp(wfd.cFileName,".") && lstrcmp(wfd.cFileName,"..")) {
				if (
					((wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)==0 || (Flags & FIF_INCHID))
					&& ((Flags & FIF_DIRFILTER)==0 || IsMatch(szFinded, Pattern))
					) {
					pCallBack(szFinded,wfd.dwFileAttributes,pCustom);
				}
				if (Flags & FIF_DEEP) {
					FindInFiles(szFinded,Pattern,Flags,pCallBack,pCustom);
				}
			}
		} else {
			if (((wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)==0 || (Flags & FIF_INCHID)) 
				&& IsMatch(szFinded, Pattern)) {
				pCallBack(szFinded,wfd.dwFileAttributes,pCustom);
			}
		}
		if (bContFIF==FALSE)
			break;
		nResult=FindNextFile(hSrch,&wfd);
	}
	FindClose(hSrch);
}

void SavePosition(HWND hWnd, TCHAR *Key)
{
	WINDOWPLACEMENT wndpl;

	wndpl.length=sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hWnd,&wndpl);
	if (wndpl.showCmd == SW_SHOWMAXIMIZED) {
		SHRegWriteInt(SHCU,Key,"Max", 1);
	} else {
		SHRegWriteInt(SHCU,Key,"Max", 0);
	}

	SHRegWriteInt(SHCU,Key,"left", wndpl.rcNormalPosition.left);
	SHRegWriteInt(SHCU,Key,"top", wndpl.rcNormalPosition.top);
	SHRegWriteInt(SHCU,Key,"right", wndpl.rcNormalPosition.right);
	SHRegWriteInt(SHCU,Key,"bottom",wndpl.rcNormalPosition.bottom);
}

void LoadPosition(HWND hWnd, TCHAR *Key, RECT *Def/*=NULL*/)
{
	WINDOWPLACEMENT wndpl;
	RECT drt;

	if (Def==NULL) {
		SetRect(&drt,10,10,600,400);
	} else {
		CopyRect(&drt,Def);
	}
	wndpl.length=sizeof(WINDOWPLACEMENT);
	wndpl.flags=0;
	wndpl.rcNormalPosition.left=SHRegReadInt(SHCU,Key,"left",drt.left);
	wndpl.rcNormalPosition.top=SHRegReadInt(SHCU,Key,"top",drt.top);
	wndpl.rcNormalPosition.right=SHRegReadInt(SHCU,Key,"right",drt.right);
	wndpl.rcNormalPosition.bottom=SHRegReadInt(SHCU,Key,"bottom",drt.bottom);

	if (SHRegReadInt(SHCU,Key, "Max", 0) == 1) {
		wndpl.showCmd=SW_SHOWMAXIMIZED;
	} else {
		wndpl.showCmd=SW_RESTORE;
	}
	wndpl.ptMinPosition.x=wndpl.ptMinPosition.y=0;
	wndpl.ptMaxPosition.x=wndpl.ptMaxPosition.y=0;
	SetWindowPlacement(hWnd,&wndpl);
}

void MoveToParentCenter(HWND hWnd)
{
	RECT wrt,crt;
	HWND hParent;

	hParent=GetParent(hWnd);
	if (hParent == NULL) {
		return;
	}
	if (IsIconic(hParent)) {
		ShowWindow(hParent,SW_RESTORE);
	}

	GetWindowRect(hParent,&wrt);
	GetWindowRect(hWnd,&crt);
	SetWindowPos(hWnd,HWND_NOTOPMOST,wrt.left+(wrt.right-wrt.left)/2-(crt.right-crt.left)/2,
		wrt.top+(wrt.bottom-wrt.top)/2-(crt.bottom-crt.top)/2,0,0,SWP_NOSIZE);
}

int TreeView_GetDepth(HWND hTree,HTREEITEM hItem)
{
	HTREEITEM hParent;

	hParent=TreeView_GetParent(hTree,hItem);
	if (hParent == NULL) {
		return 0;
	} else {
		hParent=TreeView_GetParent(hTree,hParent);
		if (hParent == NULL) {
			return 1;
		} else {
			return 2;
		}
	}
}

int TreeView_GetChildCount(HWND hTree,HTREEITEM hItem)
{
	HTREEITEM hChild;
	int Count;

	hChild=TreeView_GetChild(hTree,hItem);
	if (hChild==NULL) return 0;
	for (Count=1;;Count++) {
		hChild=TreeView_GetNextSibling(hTree,hChild);
		if (hChild==NULL) {
			return Count;
		}
	}
}

LPARAM TreeView_GetParam(HWND hTree,HTREEITEM hItem)
{
	TVITEM Tv;

	Tv.mask=TVIF_PARAM;
	Tv.hItem=hItem;
	TreeView_GetItem(hTree,&Tv);
	return Tv.lParam;
}

void TreeView_GetText(HWND hTree,HTREEITEM hItem,TCHAR *Text,int len)
{
	TVITEM Tv;

	Tv.mask=TVIF_TEXT;
	Tv.hItem=hItem;
	Tv.pszText=Text;
	Tv.cchTextMax=len;
	TreeView_GetItem(hTree,&Tv);
}

void TreeView_SetText(HWND hTree,HTREEITEM hItem,TCHAR *Text)
{
	TVITEM Tv;

	Tv.mask=TVIF_TEXT;
	Tv.hItem=hItem;
	Tv.pszText=Text;
	TreeView_SetItem(hTree,&Tv);
}

// st 날짜의 절대 초를 구한다. 1970년 1월 1일 자정 기준
DWORD MyGetAbsSecond(SYSTEMTIME st)
{
	INT64 i64;
	FILETIME fst;

	SystemTimeToFileTime(&st,&fst);
	i64=(((INT64)fst.dwHighDateTime) << 32) + fst.dwLowDateTime;
	// 134774 상수는 1601년부터 1970년 1월 1일까지의 경과 날수
	i64 = i64 / 10000000 - (INT64)134774 * 86400;

	return (DWORD)i64;
}

// 절대초를 시스템 타임으로 바꾼다.
void MyAbsSecondToSystem(DWORD Abs, SYSTEMTIME &st)
{
	INT64 i64;
	FILETIME fst;

	i64=(Abs + (INT64)134774 * 86400)*10000000;
	fst.dwHighDateTime = (DWORD)(i64 >> 32);
	fst.dwLowDateTime = (DWORD)(i64 & 0xffffffff);
	FileTimeToSystemTime(&fst, &st);
}

int ListView_GetItemImage(HWND hList,int idx,int sub)
{
	LVITEM LI;

	LI.mask=LVIF_IMAGE;
	LI.iItem=idx;
	LI.iSubItem=sub;
	ListView_GetItem(hList,&LI);
	return LI.iImage;
}

int ListView_GetParam(HWND hList,int idx)
{
	LVITEM LI;

	LI.mask=LVIF_PARAM;
	LI.iItem=idx;
	LI.iSubItem=0;
	ListView_GetItem(hList,&LI);
	return LI.lParam;
}

extern HINSTANCE g_hInst;
void MakeAssociate(TCHAR *ext, TCHAR *desc)
{
	TCHAR OldProgID[64];
	TCHAR OldDesc[64];
	TCHAR NewProgID[64];
	TCHAR NewDesc[64];
	TCHAR szKey[MAX_PATH];
	TCHAR Path[MAX_PATH];
	
	SHRegReadString(SHCR,ext,NULL,"",OldProgID,64);
	SHRegReadString(SHCR,OldProgID,NULL,"",OldDesc,64);
	
	wsprintf(NewProgID,"FileCabinet_%s",ext);
	if (desc == NULL) {
		lstrcpy(NewDesc,OldDesc);
	} else {
		lstrcpy(NewDesc,desc);
	}
	
	SHRegWriteString(SHCR,NewProgID,NULL,NewDesc);
	wsprintf(szKey,"%s\\shell\\open\\command",NewProgID);
	GetModuleFileName(g_hInst,Path,MAX_PATH);
	lstrcat(Path, " \"%1\"");
	SHRegWriteString(SHCR,szKey,NULL,Path);
	
	SHRegWriteString(SHCR,ext,NULL,NewProgID);
	
	if (lstrcmp(OldProgID, NewProgID)!=0) {
		SHRegWriteString(SHCR,NewProgID,"OldProgID",OldProgID);
	}
	
	SHChangeNotify(SHCNE_ASSOCCHANGED,SHCNF_IDLIST,NULL,NULL);
}
