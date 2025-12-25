#include <windows.h>
#include <shlobj.h>
#include <shlwapi.h>
#include "ShReg.h"
#include "resource.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("AssoTest");

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;

	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=NULL;
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);

	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

// ext 확장자가 RegName과 연결되어 있는지 조사한다.
BOOL TestAssociate(LPCTSTR ext, LPCTSTR RegName)
{
	TCHAR OldProgID[MAX_PATH];
	TCHAR NewProgID[MAX_PATH];

	SHRegReadString(SHCR,ext,NULL,"",OldProgID,MAX_PATH);
	wsprintf(NewProgID,"%s%s",RegName,ext);
	return (lstrcmpi(OldProgID,NewProgID)==0);
}

// ext 확장자를 exe 실행 파일과 연결한다. RegName은 레지스트리 키, desc는 설명
void MakeAssociate(LPCTSTR ext, LPCTSTR RegName, LPCTSTR exe, LPCTSTR desc)
{
	TCHAR OldProgID[64];
	TCHAR OldDesc[64];
	TCHAR NewProgID[64];
	TCHAR NewDesc[64];
	TCHAR szKey[MAX_PATH];
	TCHAR Path[MAX_PATH];

	// 이전 ID와 설명을 미리 구해 놓는다.
	SHRegReadString(SHCR,ext,NULL,"",OldProgID,64);
	SHRegReadString(SHCR,OldProgID,NULL,"",OldDesc,64);

	// 새 ID와 설명을 작성한다. 설명이 없으면 이전 설명을 취한다.
	wsprintf(NewProgID,"%s%s",RegName,ext);
	lstrcpy(NewDesc,desc ? OldDesc:desc);

	// 새 ID로 열기 명령에 대한 레지스트리 키를 작성한다.
	SHRegWriteString(SHCR,NewProgID,NULL,NewDesc);
	wsprintf(szKey,"%s\\shell\\open\\command",NewProgID);
	wsprintf(Path, "\"%s\" \"%%1\"",exe);
	SHRegWriteString(SHCR,szKey,NULL,Path);

	// 아이콘 지정
	wsprintf(szKey,"%s\\DefaultIcon",NewProgID);
	wsprintf(Path, "\"%s\",0",exe);
	SHRegWriteString(SHCR,szKey,NULL,Path);

	// 탐색기의 확장자 정보도 같이 수정한다.
/*	TCHAR ExplorerKey[MAX_PATH];
	wsprintf(ExplorerKey,"Software\\Microsoft\\Windows\\CurrentVersion\\"
		"Explorer\\FileExts\\%s",ext);
	_splitpath(exe,NULL,NULL,Path,NULL);
	lstrcat(Path,".exe");
	SHRegWriteString(SHCU,ExplorerKey,"Application",Path);*/

	// 새 ID와 연결한다.
	SHRegWriteString(SHCR,ext,NULL,NewProgID);

	// 이전 ID를 저장해 놓는다. 단, 중복 변경하지는 않는다.
	if (lstrcmp(OldProgID, NewProgID)!=0) {
		wsprintf(szKey,"%sOld",RegName);
		SHRegWriteString(SHCR,NewProgID,szKey,OldProgID);
	}

	SHChangeNotify(SHCNE_ASSOCCHANGED,SHCNF_IDLIST,NULL,NULL);
}

// ext 확장자와의 연결을 해제한다.
void UnAssociate(LPCTSTR ext, LPCTSTR RegName)
{
	TCHAR OldProgID[64];
	TCHAR NewProgID[64];
	TCHAR szKey[MAX_PATH];

	if (TestAssociate(ext,RegName)) {
		wsprintf(NewProgID,"%s%s",RegName,ext);
		wsprintf(szKey,"%sOld",RegName);
		SHRegReadString(SHCR,NewProgID,szKey,"",OldProgID,64);
		SHRegWriteString(SHCR,ext,NULL,OldProgID);
		SHDeleteKey(SHCR,NewProgID);
		SHChangeNotify(SHCNE_ASSOCCHANGED,SHCNF_IDLIST,NULL,NULL);

/*		HKEY key;
		DWORD dwDisp;
		wsprintf(szKey,"Software\\Microsoft\\Windows\\CurrentVersion\\"
			"Explorer\\FileExts\\%s",ext);
		RegCreateKeyEx(SHCU, szKey,0,NULL,
			REG_OPTION_NON_VOLATILE, KEY_SET_VALUE,NULL,&key,&dwDisp);
		RegDeleteValue(key,"Application");
		RegCloseKey(key);*/
	}
}

// 확장자와 연결된 실행 파일의 경로를 변경한다.
void ModifyAssociate(LPCTSTR ext, LPCTSTR RegName, LPCTSTR exe)
{
	TCHAR szKey[MAX_PATH];
	TCHAR Path[MAX_PATH];

	// 연결되어 있으면 경로와 아이콘을 수정한다.
	if (TestAssociate(ext,RegName)) {
		wsprintf(szKey,"%s%s\\shell\\open\\command",RegName,ext);
		wsprintf(Path, "\"%s\" \"%%1\"",exe);
		SHRegWriteString(SHCR,szKey,NULL,Path);

		wsprintf(szKey,"%s%s\\DefaultIcon",RegName,ext);
		wsprintf(Path, "\"%s\",0",exe);
		SHRegWriteString(SHCR,szKey,NULL,Path);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes="왼쪽 클릭:txt 파일과 연결, 오른쪽 클릭:txt 파일 연결 해제";
	static HWND hEdit;
	HANDLE hFile;
	DWORD Size,dwRead;
	TCHAR *buf;
	TCHAR exe[MAX_PATH];
	TCHAR Status[128];

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		hEdit=CreateWindow("edit",NULL,WS_CHILD | WS_VISIBLE | WS_BORDER |
			WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
			0,0,0,0,hWnd,(HMENU)0,g_hInst,NULL);
		if (__argc >= 2) {
			hFile=CreateFile(__argv[1],GENERIC_READ,0,NULL,
				OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
			if (hFile!=INVALID_HANDLE_VALUE) {
				Size=GetFileSize(hFile,NULL);
				buf=(TCHAR *)malloc(Size+1);
				ReadFile(hFile,buf,Size,&dwRead,NULL);
				buf[dwRead]=0;
				SetWindowText(hEdit,buf);
				free(buf);
				CloseHandle(hFile);
			}
		}
		return 0;
	case WM_SIZE:
		if (wParam != SIZE_MINIMIZED) {
			MoveWindow(hEdit,0,50,LOWORD(lParam),HIWORD(lParam)-50,TRUE);
		}
		return 0;
	case WM_LBUTTONDOWN:
		GetModuleFileName(g_hInst,exe,MAX_PATH);
		MakeAssociate(".txt","AssoTest",exe,"텍스트 파일");
		InvalidateRect(hWnd,NULL,TRUE);
		return 0;
	case WM_RBUTTONDOWN:
		UnAssociate(".txt","AssoTest");
		InvalidateRect(hWnd,NULL,TRUE);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,10,10,Mes,lstrlen(Mes));
		if (TestAssociate(".txt","AssoTest")) {
			lstrcpy(Status,"txt 확장자와 연결되어 있습니다.");
		} else {
			lstrcpy(Status,"txt 확장자와 연결되어 있지 않습니다.");
		}
		TextOut(hdc,10,30,Status,lstrlen(Status));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
