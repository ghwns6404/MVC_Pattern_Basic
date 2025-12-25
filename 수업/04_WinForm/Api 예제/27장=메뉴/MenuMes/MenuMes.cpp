#define WINVER 0x500
#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("MenuMes");

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

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HMENU hMenu, hPopup;
	POINT pt;
	RECT crt;
	static TCHAR str[128]="메뉴를 선택하십시요";
	LPHELPINFO phi;
	
	switch (iMessage) {
	case WM_HELP:
		phi=(LPHELPINFO)lParam;
		if ((phi->iContextType == HELPINFO_MENUITEM) && (phi->iCtrlId==ID_FILE_OPEN1)) {
			MessageBox(hWnd,"이 항목을 선택하면 파일을 열수 있습니다","도움말",MB_OK);
		}
		return 0;
	case WM_MENUSELECT:
		switch (LOWORD(wParam)) {
		case ID_FILE_OPEN1:
			lstrcpy(str,"파일을 엽니다");
			break;
		case ID_FILE_SAVE1:
			lstrcpy(str,"파일을 저장합니다");
			break;
		case ID_FILE_EXIT:
			lstrcpy(str,"프로그램을 종료합니다.");
			break;
		case ID_POPUP_CUT:
			lstrcpy(str,"이 메뉴 항목을 자릅니다.");
			break;
		case ID_POPUP_COPY:
			lstrcpy(str,"이 메뉴 항목을 복사합니다.");
			break;
		case ID_POPUP_EXECUTE:
			lstrcpy(str,"이 메뉴 항목을 실행합니다.");
			break;
		}
		InvalidateRect(hWnd,NULL,TRUE);
		return 0;
	case WM_MENURBUTTONUP:
		GetCursorPos(&pt);
		hMenu=LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU2));
		hPopup=GetSubMenu(hMenu, 0);
		TrackPopupMenuEx(hPopup, TPM_LEFTALIGN | TPM_RECURSE, pt.x, pt.y, 
			hWnd, NULL);
		DestroyMenu(hMenu);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_FILE_OPEN1:
			MessageBox(hWnd,"파일 열기 선택","알림",MB_OK);
			break;
		case ID_FILE_SAVE1:
			MessageBox(hWnd,"파일 저장 선택","알림",MB_OK);
			break;
		case ID_FILE_EXIT:
			DestroyWindow(hWnd);
			break;
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		GetClientRect(hWnd,&crt);
		TextOut(hdc,10,crt.bottom-25,str,lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

