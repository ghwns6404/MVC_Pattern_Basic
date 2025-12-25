#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("OwnMenu");

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


int Pen_Style[]={PS_SOLID, PS_DASH, PS_DOT};
int NowPen=0;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	LPMEASUREITEMSTRUCT lpmis;
	LPDRAWITEMSTRUCT lpdis;
	HPEN hPen, hOldPen;
	HBRUSH hBrush;
	PAINTSTRUCT ps;
	HDC hdc;
	switch (iMessage) {
	case WM_CREATE:
		ModifyMenu(GetMenu(hWnd), ID_MENU_SOLID, MF_BYCOMMAND | MF_OWNERDRAW,
			ID_MENU_SOLID, (LPCTSTR)Pen_Style[0]);
		ModifyMenu(GetMenu(hWnd), ID_MENU_DASH, MF_BYCOMMAND | MF_OWNERDRAW,
			ID_MENU_DASH, (LPCTSTR)Pen_Style[1]);
		ModifyMenu(GetMenu(hWnd), ID_MENU_DOT, MF_BYCOMMAND | MF_OWNERDRAW,
			ID_MENU_DOT, (LPCTSTR)Pen_Style[2]);
		return 0;
	case WM_MEASUREITEM:
		lpmis = (LPMEASUREITEMSTRUCT)lParam;
		lpmis->itemWidth=100;
		lpmis->itemHeight=20;
		return TRUE;
	case WM_DRAWITEM:
		lpdis=(LPDRAWITEMSTRUCT)lParam;
		if (lpdis->itemState & ODS_SELECTED) {
			hBrush=CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
			hPen=CreatePen(lpdis->itemData, 1, GetSysColor(COLOR_HIGHLIGHTTEXT));
			SetBkColor(lpdis->hDC, GetSysColor(COLOR_HIGHLIGHT));
		} else {
			hBrush=CreateSolidBrush(GetSysColor(COLOR_BTNFACE));
			hPen=CreatePen(lpdis->itemData, 1, RGB(0,0,0));
		}
		FillRect(lpdis->hDC, &lpdis->rcItem, hBrush);
		hOldPen=(HPEN)SelectObject(lpdis->hDC, hPen);
		MoveToEx(lpdis->hDC, lpdis->rcItem.left+10,lpdis->rcItem.top+10,NULL);
		LineTo(lpdis->hDC, lpdis->rcItem.left+90,lpdis->rcItem.top+10);
		SelectObject(lpdis->hDC, hOldPen);
		DeleteObject(hPen);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_MENU_SOLID:
			NowPen=0;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_MENU_DASH:
			NowPen=1;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_MENU_DOT:
			NowPen=2;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		hPen=CreatePen(Pen_Style[NowPen], 1, RGB(0,0,0));
		hOldPen=(HPEN)SelectObject(hdc, hPen);
		Rectangle(hdc, 50, 50, 300, 200);
		SelectObject(hdc, hOldPen);
		DeleteObject(hPen);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}


