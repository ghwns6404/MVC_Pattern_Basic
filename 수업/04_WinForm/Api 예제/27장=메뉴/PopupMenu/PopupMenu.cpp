#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("PopupMenu");

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

#include "resource.h"
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HMENU hMenu, hPopup;
	static COLORREF Color=RGB(255,0,0);
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush;
	RECT crt;

	switch (iMessage) {
/* 마우스만 지원하는 형태
	case WM_CONTEXTMENU:
		hMenu=LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU1));
		hPopup=GetSubMenu(hMenu, 0);

		TrackPopupMenu(hPopup, TPM_LEFTALIGN, (short)LOWORD(lParam), 
			(short)HIWORD(lParam), 0, hWnd, NULL);
		DestroyMenu(hMenu);
		return 0;
//*/

/* 오른쪽 버튼 놓음 메시지로 좌표 변환하기
	case WM_RBUTTONUP:
		hMenu=LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU1));
		hPopup=GetSubMenu(hMenu, 0);

		POINT pt;
		pt.x=LOWORD(lParam);
		pt.y=HIWORD(lParam);
		ClientToScreen(hWnd,&pt);
		TrackPopupMenu(hPopup, TPM_LEFTALIGN, pt.x, pt.y, 0, hWnd, NULL);
		DestroyMenu(hMenu);
		return 0;
//*/

/* 키보드 지원
	case WM_CONTEXTMENU:
		hMenu=LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU1));
		hPopup=GetSubMenu(hMenu, 0);

		if ((int)(short)LOWORD(lParam) == -1) {
			POINT pt;
			GetCursorPos(&pt);
			lParam=MAKELPARAM(pt.x,pt.y);
		}
		TrackPopupMenu(hPopup, TPM_LEFTALIGN, (short)LOWORD(lParam), 
			(short)HIWORD(lParam), 0, hWnd, NULL);
		DestroyMenu(hMenu);
		return 0;
//*/

/* 명령을 바로 처리하기
	case WM_CONTEXTMENU:
		hMenu=LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU1));
		hPopup=GetSubMenu(hMenu, 0);

		if ((int)(short)LOWORD(lParam) == -1) {
			POINT pt;
			GetCursorPos(&pt);
			lParam=MAKELPARAM(pt.x,pt.y);
		}
		UINT id;
		id=TrackPopupMenu(hPopup, TPM_LEFTALIGN | TPM_RETURNCMD, 
			(short)LOWORD(lParam), (short)HIWORD(lParam), 0, hWnd, NULL);
		switch (id) {
		case ID_POPUP_RED:
			Color=RGB(255,0,0);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_POPUP_GREEN:
			Color=RGB(0,255,0);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_POPUP_BLUE:
			Color=RGB(0,0,255);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		DestroyMenu(hMenu);
		return 0;
//*/
//* 항목의 선택 상태 표시
	case WM_CONTEXTMENU:
		hMenu=LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU1));
		hPopup=GetSubMenu(hMenu, 0);

		if (Color == RGB(255,0,0))
			CheckMenuItem(hPopup, ID_POPUP_RED, MF_BYCOMMAND | MF_CHECKED);
		if (Color == RGB(0,255,0))
			CheckMenuItem(hPopup, ID_POPUP_GREEN, MF_BYCOMMAND | MF_CHECKED);
		if (Color == RGB(0,0,255))
			CheckMenuItem(hPopup, ID_POPUP_BLUE, MF_BYCOMMAND | MF_CHECKED);

		TrackPopupMenu(hPopup, TPM_LEFTALIGN, (short)LOWORD(lParam), 
			(short)HIWORD(lParam), 0, hWnd, NULL);
		DestroyMenu(hMenu);
		return 0;
//*/
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_POPUP_RED:
			Color=RGB(255,0,0);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_POPUP_GREEN:
			Color=RGB(0,255,0);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_POPUP_BLUE:
			Color=RGB(0,0,255);
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		hBrush=CreateSolidBrush(Color);
		GetClientRect(hWnd, &crt);
		FillRect(hdc, &crt, hBrush);
		DeleteObject(hBrush);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
