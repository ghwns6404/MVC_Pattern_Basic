#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("StretchBlt");

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
	HDC hdc;
	PAINTSTRUCT ps;
	HDC MemDC;
	HBITMAP MyBitmap, OldBitmap;
	int bx,by;
	BITMAP bit;

	switch (iMessage) {
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		MemDC=CreateCompatibleDC(hdc);
		MyBitmap=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		OldBitmap=(HBITMAP)SelectObject(MemDC, MyBitmap);

		GetObject(MyBitmap,sizeof(BITMAP),&bit);
		bx=bit.bmWidth;
		by=bit.bmHeight;

		// 소스와 같은 비율로 출력
		StretchBlt(hdc,0,0,bx,by,MemDC,0,0,bx,by,SRCCOPY);

		// 미러링
		StretchBlt(hdc,100,105,-bx,by,MemDC,0,0,bx,by,SRCCOPY);

		// 2배 확대
		StretchBlt(hdc,200,0,bx*2,by*2,MemDC,0,0,bx,by,SRCCOPY);

		// 절반으로 축소
		SetStretchBltMode(hdc,BLACKONWHITE);
		StretchBlt(hdc,0,210,bx/2,by/2,MemDC,0,0,bx,by,SRCCOPY);

		SetStretchBltMode(hdc,WHITEONBLACK);
		StretchBlt(hdc,100,210,bx/2,by/2,MemDC,0,0,bx,by,SRCCOPY);

		SetStretchBltMode(hdc,COLORONCOLOR);
		StretchBlt(hdc,200,210,bx/2,by/2,MemDC,0,0,bx,by,SRCCOPY);

		SetStretchBltMode(hdc,HALFTONE);
		StretchBlt(hdc,300,210,bx/2,by/2,MemDC,0,0,bx,by,SRCCOPY);

		SelectObject(MemDC,OldBitmap);
		DeleteObject(MyBitmap);
		DeleteDC(MemDC);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

