#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("BmpBk");

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
	HDC hdc, MemDC;
	PAINTSTRUCT ps;
	static HBITMAP MyBitmap;
	HBITMAP OldBitmap;
	int x,y;
	RECT rt;
	BITMAP bit;
	static int ex,ey;

	switch (iMessage) {
	case WM_CREATE:
		MyBitmap=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		ex=100;
		ey=100;
		return 0;
	case WM_LBUTTONDOWN:
		ex=LOWORD(lParam);
		ey=HIWORD(lParam);
		InvalidateRect(hWnd,NULL,TRUE);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		Ellipse(hdc,ex-30,ey-30,ex+30,ey+30);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_ERASEBKGND:
		hdc=(HDC)wParam;
		MemDC=CreateCompatibleDC(hdc);
		OldBitmap=(HBITMAP)SelectObject(MemDC, MyBitmap);
		GetObject(MyBitmap,sizeof(BITMAP), &bit);
		GetClientRect(hWnd,&rt);
		for (x=0;x<rt.right;x+=bit.bmWidth)
			for (y=0;y<rt.bottom;y+=bit.bmHeight)
				BitBlt(hdc,x,y,x+bit.bmWidth,y+bit.bmHeight,MemDC,0,0,SRCCOPY);
		SelectObject(MemDC,OldBitmap);
		DeleteDC(MemDC);
		return TRUE;
	case WM_DESTROY:
		DeleteObject(MyBitmap);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

