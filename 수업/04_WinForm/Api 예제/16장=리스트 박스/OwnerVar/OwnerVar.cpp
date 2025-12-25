#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("OwnerVar");

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

void DrawBitmap(HDC hdc,int x,int y,HBITMAP hBit)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx,by;
	BITMAP bit;

	MemDC=CreateCompatibleDC(hdc);
	OldBitmap=(HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit,sizeof(BITMAP),&bit);
	bx=bit.bmWidth;
	by=bit.bmHeight;

	BitBlt(hdc,x,y,bx,by,MemDC,0,0,SRCCOPY);

	SelectObject(MemDC,OldBitmap);
	DeleteDC(MemDC);
}

#include "resource.h"
#define ID_LISTBOX 100
HWND hList;
HBITMAP hbmp1, hbmp2, hbmp3;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	LPMEASUREITEMSTRUCT lpmis;
	LPDRAWITEMSTRUCT lpdis;
	HBITMAP hbmp;
	HBRUSH bkBrush;
	switch (iMessage) {
	case WM_CREATE:
		// 오너 드로우 리스트 박스를 만든다.
		hList=CreateWindow(TEXT("listbox"),NULL,WS_CHILD | WS_VISIBLE | WS_BORDER |
			WS_VSCROLL | LBS_NOTIFY | LBS_OWNERDRAWVARIABLE,
			10,10,110,200,hWnd,(HMENU)ID_LISTBOX,g_hInst,NULL);

		// 비트맵 세 장을 읽어온다.
		hbmp1=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		hbmp2=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		hbmp3=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));

		// 항목 세 개를 추가하고 항목 데이터에 비트맵 핸들을 등록한다.
		SendMessage(hList,LB_ADDSTRING,0,(LPARAM)hbmp1);
		SendMessage(hList,LB_ADDSTRING,0,(LPARAM)hbmp2);
		SendMessage(hList,LB_ADDSTRING,0,(LPARAM)hbmp3);
		return 0;
	case WM_MEASUREITEM:
		// 개별 항목의 높이를 계산해 준다.
		lpmis=(LPMEASUREITEMSTRUCT)lParam;
		switch (lpmis->itemID) {
		case 0:
			lpmis->itemHeight=48;
			break;
		case 1:
			lpmis->itemHeight=30;
			break;
		case 2:
			lpmis->itemHeight=10;
			break;
		}
		lpmis->itemHeight+=4;
		return TRUE;
	case WM_DRAWITEM:
		lpdis=(LPDRAWITEMSTRUCT)lParam;

		// 선택 상황에 따라 파란색 배경을 출력한다.
		if (lpdis->itemState & ODS_SELECTED) {
			bkBrush=GetSysColorBrush(COLOR_HIGHLIGHT);
		} else {
			bkBrush=GetSysColorBrush(COLOR_WINDOW);
		}
		FillRect(lpdis->hDC, &lpdis->rcItem, bkBrush);

		// 항목 데이터로부터 비트맵 핸들을 읽어 출력한다.
		hbmp=(HBITMAP)lpdis->itemData;
		DrawBitmap(lpdis->hDC,lpdis->rcItem.left+5,lpdis->rcItem.top+2,hbmp);
		return TRUE;
	case WM_DESTROY:
		DeleteObject(hbmp1);
		DeleteObject(hbmp2);
		DeleteObject(hbmp3);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
