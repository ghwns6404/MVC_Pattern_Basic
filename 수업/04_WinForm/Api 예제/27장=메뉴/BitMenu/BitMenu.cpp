#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("BitMenu");

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
	WndClass.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1);
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

#define IDM_MONITOR 41000
#define IDM_MOUSE 41001
#define IDM_KEYBOARD 41002
HBITMAP hMon, hMou, hKey;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HMENU MenuBar;
	switch (iMessage) {
	case WM_CREATE:
		hMon=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MONITOR));
		hMou=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_MOUSE));
		hKey=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_KEYBOARD));
		MenuBar=GetSubMenu(GetMenu(hWnd),0);
		AppendMenu(MenuBar, MF_BITMAP, IDM_MONITOR, (LPCTSTR)hMon);
		AppendMenu(MenuBar, MF_BITMAP, IDM_MOUSE, (LPCTSTR)hMou);
		AppendMenu(MenuBar, MF_BITMAP, IDM_KEYBOARD, (LPCTSTR)hKey);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_MENU_STRING:
			MessageBox(hWnd, "문자열을 선택했습니다", "비트맵 메뉴", MB_OK);
			break;
		case IDM_MONITOR:
			MessageBox(hWnd, "모니터를 선택했습니다", "비트맵 메뉴", MB_OK);
			break;
		case IDM_MOUSE:
			MessageBox(hWnd, "마우스를 선택했습니다", "비트맵 메뉴", MB_OK);
			break;
		case IDM_KEYBOARD:
			MessageBox(hWnd, "키보드를 선택했습니다", "비트맵 메뉴", MB_OK);
			break;
		}
		return 0;
	case WM_DESTROY:
		DeleteObject(hMon);
		DeleteObject(hMou);
		DeleteObject(hKey);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
