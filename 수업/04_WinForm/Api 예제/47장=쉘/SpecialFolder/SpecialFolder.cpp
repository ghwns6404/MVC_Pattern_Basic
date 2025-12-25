#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("SpecialFolder");

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

#include <shlobj.h>
void MyGetSpecialFolderPath(HWND hWnd, int nFolder, TCHAR *szPath)
{
	LPITEMIDLIST pidl;
	LPMALLOC pMalloc;

	SHGetSpecialFolderLocation(hWnd,nFolder,&pidl);
	SHGetPathFromIDList(pidl, szPath);
	SHGetMalloc(&pMalloc);
	pMalloc->Free(pidl);
	pMalloc->Release();
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	LPITEMIDLIST pidl;
	TCHAR szFolder[MAX_PATH];
	TCHAR str[256];
	LPMALLOC pMalloc;

	switch (iMessage) {
	case WM_LBUTTONDOWN:
		SHGetSpecialFolderLocation(hWnd,CSIDL_PROGRAMS,&pidl);
		SHGetPathFromIDList(pidl, szFolder);
		wsprintf(str,"프로그램 폴더는 \"%s\"입니다",szFolder);
		SHGetMalloc(&pMalloc);
		pMalloc->Free(pidl);
		pMalloc->Release();
		MessageBox(hWnd,str,"알림",MB_OK);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

