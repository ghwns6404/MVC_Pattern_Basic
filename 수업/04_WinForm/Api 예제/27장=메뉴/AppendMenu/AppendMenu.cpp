#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("AppendMenu");

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

int MenuID=41001;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HMENU MenuBar;
	TCHAR str[128];
	switch (iMessage) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_MENU_ADD:
			if (MenuID > 41005) {
				MessageBox(hWnd, "5개까지 추가할 수 있습니다","초과",MB_OK);
			} else {
				MenuBar=GetSubMenu(GetMenu(hWnd), 0);
				wsprintf(str, "Add Item %d", MenuID-41000);
				AppendMenu(MenuBar, MF_STRING, MenuID, str);
				MenuID++;
			}
			break;
		case ID_MENU_DELETE:
			if (MenuID <= 41001) {
				MessageBox(hWnd, "삭제할 메뉴가 없습니다","초과",MB_OK);
			} else {
				MenuBar=GetSubMenu(GetMenu(hWnd), 0);
				DeleteMenu(MenuBar, MenuID-1,MF_BYCOMMAND);
				MenuID--;
			}
			break;
		}
		if (LOWORD(wParam) > 41000 && LOWORD(wParam) <= 41005) {
			wsprintf(str, "%d번째 메뉴를 선택했습니다",LOWORD(wParam)-41000);
			MessageBox(hWnd, str, "ApndMenu", MB_OK);
		}
		return 0;
	case WM_INITMENU:
		if (MenuID > 41005)
			EnableMenuItem((HMENU)wParam, ID_MENU_ADD, MF_BYCOMMAND | MF_GRAYED);
		else
			EnableMenuItem((HMENU)wParam, ID_MENU_ADD, MF_BYCOMMAND | MF_ENABLED);
		if (MenuID <= 41001)
			EnableMenuItem((HMENU)wParam, ID_MENU_DELETE, MF_BYCOMMAND | MF_GRAYED);
		else
			EnableMenuItem((HMENU)wParam, ID_MENU_DELETE, MF_BYCOMMAND | MF_ENABLED);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}


