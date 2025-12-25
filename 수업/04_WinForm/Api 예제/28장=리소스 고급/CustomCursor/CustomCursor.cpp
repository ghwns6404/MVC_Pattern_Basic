#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("CustomCursor");

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

HCURSOR h1,h2,h3,hNow;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rt;
	TCHAR *Mes="마우스를 누르면 커서가 바뀝니다";

	switch (iMessage) {
	case WM_CREATE:
		h1=LoadCursor(g_hInst,MAKEINTRESOURCE(IDC_CURSOR1));
		h2=LoadCursor(g_hInst,MAKEINTRESOURCE(IDC_CURSOR2));
		h3=LoadCursorFromFile("dog.ani");
		hNow=h1;
		return 0;
	case WM_LBUTTONDOWN:
		if (hNow==h1) hNow=h2;
		else if (hNow==h2) hNow=h3;
		else if (hNow==h3) hNow=h1;
		return 0;
	case WM_SETCURSOR:
		if (LOWORD(lParam)==HTCLIENT) {
			SetCursor(hNow);
			return 0;
		} else {
			break;
		}
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		SetRect(&rt,10,10,100,50);
		FillRect(hdc,&rt,(HBRUSH)GetStockObject(BLACK_BRUSH));
		TextOut(hdc,10,100,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

