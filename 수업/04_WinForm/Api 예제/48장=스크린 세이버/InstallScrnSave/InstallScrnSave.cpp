#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("InstallScrnSave");

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;

	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)(COLOR_BTNFACE+1);
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

void Install();
#define IDC_EDITTIME 100
#define IDC_BTNINSTALL 101
HWND hEdit;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage) {
	case WM_CREATE:
		hWndMain = hWnd;
		CreateWindow("static","설정시간(분)",WS_CHILD | WS_VISIBLE,
			20,20,100,25,hWnd,(HMENU)-1,g_hInst,NULL);
		hEdit = CreateWindow("edit","10",WS_CHILD | WS_VISIBLE | WS_BORDER,
			130,20,200,25,hWnd,(HMENU)IDC_EDITTIME,g_hInst,NULL);
		CreateWindow("button","설치",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			20,50,100,25,hWnd,(HMENU)IDC_BTNINSTALL,g_hInst,NULL);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BTNINSTALL:
			Install();
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

void Install()
{
	TCHAR ShortName[MAX_PATH];
	TCHAR Dest[MAX_PATH];
	TCHAR Src[MAX_PATH];
	int nTime;

	// 파일을 복사한다.
	GetWindowsDirectory(Dest, MAX_PATH);
	lstrcat(Dest, "\\ScrnSave1.scr");
	GetCurrentDirectory(MAX_PATH, Src);
	lstrcat(Src, "\\ScrnSave1.scr");
	if (CopyFile(Src, Dest, FALSE) == 0) {
		MessageBox(hWndMain, "파일을 복사할 수 없습니다.", "에러", MB_OK);
		return;
	}

	// 설정 시간을 기록한다.
	GetWindowText(hEdit, Src, MAX_PATH);
	nTime = atoi(Src);
	nTime = nTime * 60;
	SystemParametersInfo(SPI_SETSCREENSAVETIMEOUT, nTime, NULL, 0);

	// 짧은 파일명을 System.ini에 기록한다.
	GetShortPathName(Dest, ShortName, MAX_PATH);
	WritePrivateProfileString("boot", "SCRNSAVE.EXE", ShortName, "System.ini");

	MessageBox(hWndMain, "스크린 세이버가 설치되었습니다.", "알림", MB_OK);
}
