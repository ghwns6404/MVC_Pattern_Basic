#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ToolBtnStyle");

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;
	
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=NULL;
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=NULL;
	WndClass.style=0;
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

#define _WIN32_WINNT 0x400
#define _WIN32_WINDOWS 0x401
#include <commctrl.h>
#include "resource.h"
#define ID_TOOLBAR 100
HWND hToolBar;
COLORREF Back;
DWORD iMode;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT crt;
	HBRUSH hBrush;
	TBBUTTON ToolBtn[]={
		{0,10,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{5,0,0,TBSTYLE_SEP,0,0,0,0},
		{1,11,TBSTATE_ENABLED | TBSTATE_CHECKED,TBSTYLE_CHECKGROUP,0,0,0,1},
		{2,12,TBSTATE_ENABLED,TBSTYLE_CHECKGROUP,0,0,0,2},
		{3,13,TBSTATE_ENABLED,TBSTYLE_CHECKGROUP,0,0,0,3},
		{5,0,0,TBSTYLE_SEP,0,0,0,0},
		{4,14,TBSTATE_ENABLED,TBSTYLE_CHECK | TBSTYLE_AUTOSIZE,0,0,0,4},
		{5,15,TBSTATE_ENABLED,TBSTYLE_DROPDOWN | TBSTYLE_NOPREFIX,0,0,0,5},
		{6,16,TBSTATE_ENABLED,TBSTYLE_DROPDOWN | 0x80/*BTNS_WHOLEDROPDOWN*/,0,0,0,6}
	};
	TCHAR *Mes="툴 버튼의 여러 속성들을 테스트합니다.";
	TCHAR *szString="명령버튼\0빨간색\0초록색\0파란색\0투명\0&Drop\0&Drop2\0";

	switch (iMessage) {
	case WM_CREATE:
		InitCommonControls();
		hToolBar=CreateToolbarEx(hWnd, WS_CHILD | WS_VISIBLE | WS_BORDER
			| TBSTYLE_FLAT,
			ID_TOOLBAR, 10, g_hInst, IDB_BITMAP1, ToolBtn, 9,
			16,16,16,16,sizeof(TBBUTTON));
		SendMessage(hToolBar,TB_ADDSTRING,NULL,(LPARAM)szString);
		SendMessage(hToolBar,TB_SETEXTENDEDSTYLE,0,(LPARAM)TBSTYLE_EX_DRAWDDARROWS);
		Back=RGB(255,0,0);
		iMode=OPAQUE;
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 10:
			MessageBox(hWnd,"푸쉬 버튼입니다","알림",MB_OK);
			break;
		case 11:
			Back=RGB(255,0,0);
			InvalidateRect(hWnd,NULL,TRUE);
			break;
		case 12:
			Back=RGB(0,255,0);
			InvalidateRect(hWnd,NULL,TRUE);
			break;
		case 13:
			Back=RGB(0,0,255);
			InvalidateRect(hWnd,NULL,TRUE);
			break;
		case 14:
			if (SendMessage(hToolBar,TB_ISBUTTONCHECKED,14,0)) {
				iMode=TRANSPARENT;
			} else {
				iMode=OPAQUE;
			}
			InvalidateRect(hWnd,NULL,TRUE);
		}
		return 0;
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code) {
		case TBN_DROPDOWN:
			HMENU hPopup;
			POINT pt;
			hPopup=CreatePopupMenu();
			AppendMenu(hPopup,MF_STRING,0,"팝업 하나");
			AppendMenu(hPopup,MF_STRING,1,"팝업 둘");
			AppendMenu(hPopup,MF_STRING,2,"팝업 셋");

			GetCursorPos(&pt);
			TrackPopupMenu(hPopup,TPM_LEFTALIGN,pt.x,pt.y,0,hWnd,NULL);
			DestroyMenu(hPopup);

			return TBDDRET_DEFAULT;
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		GetClientRect(hWnd,&crt);
		hBrush=CreateSolidBrush(Back);
		FillRect(hdc,&crt,hBrush);
		DeleteObject(hBrush);
		SetBkMode(hdc,iMode);
		TextOut(hdc,10,50,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
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
