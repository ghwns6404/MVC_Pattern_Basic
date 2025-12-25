#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("OwnerBtn");

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
HBITMAP hUp,hDown;
#define ID_OWNERBTN 100
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	LPDRAWITEMSTRUCT lpdis;
	switch (iMessage) {
	case WM_CREATE:
		CreateWindow(TEXT("button"),NULL,WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
			10,10,81,70,hWnd,(HMENU)ID_OWNERBTN,g_hInst,NULL);
		hUp=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BTNUP));
		hDown=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BTNDOWN));
		return 0;
	case WM_DRAWITEM:
		lpdis=(LPDRAWITEMSTRUCT)lParam;
		if (lpdis->itemState & ODS_SELECTED) {
			DrawBitmap(lpdis->hDC,0,0,hDown);
		} else {
			DrawBitmap(lpdis->hDC,0,0,hUp);
		}
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_OWNERBTN:
			MessageBox(hWnd,TEXT("오너 드로우 버튼을 선택했네요"),
				TEXT("확인"),MB_OK);
			break;
		}
		return 0;
	case WM_DESTROY:
		DeleteObject(hUp);
		DeleteObject(hDown);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
