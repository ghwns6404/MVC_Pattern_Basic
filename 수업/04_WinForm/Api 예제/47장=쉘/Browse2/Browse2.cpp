#include <windows.h>
#include <shlobj.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("Browse2");

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
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
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

int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData)
{
	switch (uMsg) {
	case BFFM_INITIALIZED:
		SetWindowText(hwnd, "폴더 고르기");
		SetWindowPos(hwnd,NULL,10,10,0,0,SWP_NOSIZE | SWP_NOZORDER);
/*
		// 시작 폴더 지정하기
		TCHAR WinDir[MAX_PATH];
		GetWindowsDirectory(WinDir,MAX_PATH);
		SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)WinDir);
//*/
		break;
	case BFFM_SELCHANGED:
		TCHAR Sel[MAX_PATH];
		SHGetPathFromIDList((LPITEMIDLIST)lParam,Sel);
		if (lstrlen(Sel) == 3) {
			SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)"루트 폴더는 선택할 수 없습니다");
			SendMessage(hwnd,BFFM_ENABLEOK,0,FALSE);
		} else {
			SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)"선택 가능한 폴더입니다.");
			SendMessage(hwnd,BFFM_ENABLEOK,0,TRUE);
		}
		break;
	case BFFM_VALIDATEFAILED:
		MessageBox(hwnd,"에디트에 입력한 폴더는 존재하지 않습니다. 다시 입력해 주십시요",
			"알림",MB_OK);
		return 1;
	}
	return 0;
}

BOOL SHGetPidlFromPath(TCHAR *Path,LPITEMIDLIST *ppidl)
{
	LPSHELLFOLDER pShellFolder;
	OLECHAR wszPath[MAX_PATH]={0,};

	if (FAILED(SHGetDesktopFolder(&pShellFolder)))
		return FALSE;
	MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,Path,-1,wszPath,MAX_PATH);
	pShellFolder->ParseDisplayName(NULL,NULL,wszPath,NULL,ppidl,NULL);
	pShellFolder->Release();
	return TRUE;
}

BOOL BrowseFolder(HWND hWnd, TCHAR *szFolder)
{
	LPMALLOC pMalloc;
	LPITEMIDLIST pidl;
	BROWSEINFO bi;
	LPITEMIDLIST root;

	bi.hwndOwner = hWnd;
/*
	//내 컴퓨터를 루트로 만들기
	SHGetSpecialFolderLocation(hWnd,CSIDL_DRIVES,&root);
	bi.pidlRoot = root;
//*/

/*
	// 윈도우즈 디렉토리를 루트로 만들기
	TCHAR WinDir[MAX_PATH];
	GetWindowsDirectory(WinDir,MAX_PATH);
	SHGetPidlFromPath(WinDir, &root);
	bi.pidlRoot = root;
//*/
	bi.pidlRoot = NULL;
	bi.pszDisplayName = NULL;
	bi.lpszTitle = "아래 리스트에서 폴더를 선택하세요." ;
	bi.ulFlags = BIF_EDITBOX | BIF_STATUSTEXT | BIF_VALIDATE;
	bi.lpfn = BrowseCallbackProc;
	bi.lParam = 0;
						
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

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR Folder[MAX_PATH];
	TCHAR *Mes="왼쪽 버튼을 눌러 폴더를 선택하십시요";

	switch (iMessage) {
	case WM_LBUTTONDOWN:
		if (BrowseFolder(hWnd,Folder))
			MessageBox(hWnd, Folder, "선택한 폴더",MB_OK);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,50,50,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}


