#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ToolBarText");

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
	WndClass.style=0;;
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	hWndMain=hWnd;
	
	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

#include <commctrl.h>
#include "resource.h"
#define ID_TOOLBAR 100
HWND hToolBar;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	TBBUTTON ToolBtn[5]={
		{0,10,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{1,11,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,1},
		{5,0,0,TBSTYLE_SEP,0,0,0,0},
		{2,12,TBSTATE_ENABLED | TBSTATE_CHECKED,TBSTYLE_CHECKGROUP,0,0,0,2},
		{3,13,TBSTATE_ENABLED,TBSTYLE_CHECKGROUP,0,0,0,0}
	};
	TCHAR *szString="첫번째\0두번째\0세번째\0";
	TBBUTTONINFO bi;

	switch (iMessage) {
	case WM_CREATE:
		InitCommonControls();
		hToolBar=CreateToolbarEx(hWnd, WS_CHILD | WS_VISIBLE | WS_BORDER
			| TBSTYLE_FLAT | TBSTYLE_TOOLTIPS,
			ID_TOOLBAR, 4, g_hInst, IDB_BITMAP1, ToolBtn, 5,
			16,16,16,16,sizeof(TBBUTTON));
		SendMessage(hToolBar,TB_ADDSTRING,NULL,(LPARAM)szString);

		bi.cbSize=sizeof(TBBUTTONINFO);
		bi.dwMask=TBIF_TEXT;
		bi.pszText="메롱";
		SendMessage(hToolBar,TB_SETBUTTONINFO,13,(LPARAM)&bi);

		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 10:
			MessageBox(hWnd,"첫번째 버튼을 선택했습니다","알림",MB_OK);
			break;
		case 11:
			MessageBox(hWnd,"두번째 버튼을 선택했습니다","알림",MB_OK);
			break;
		}
		return 0;
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code) {
		case TTN_GETDISPINFO:
			switch (wParam) {
			case 10:
				((LPNMTTDISPINFO)lParam)->lpszText="장남 버튼입니다.";
				break;
			case 11:
				((LPNMTTDISPINFO)lParam)->lpszText="귀염둥이 둘째 딸입니다.";
				break;
			case 12:
			case 13:
				((LPNMTTDISPINFO)lParam)->lpszText="쌍둥이 막내들입니다.";
				break;
			}
		}
		return 0;
	case WM_SIZE:
		SendMessage(hToolBar,TB_AUTOSIZE,0,0);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
