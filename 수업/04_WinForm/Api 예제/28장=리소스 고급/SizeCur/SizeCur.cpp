#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("SizeCur");

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
HCURSOR hCurSizeWe, hCurSizeNs, hCurSizeNesw, hCurSizeNwse;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage) {
	case WM_CREATE:
		hCurSizeWe=  LoadCursor(g_hInst,MAKEINTRESOURCE(IDC_CURSIZEWE));
		hCurSizeNs=  LoadCursor(g_hInst,MAKEINTRESOURCE(IDC_CURSIZENS));
		hCurSizeNesw=LoadCursor(g_hInst,MAKEINTRESOURCE(IDC_CURSIZENESW));
		hCurSizeNwse=LoadCursor(g_hInst,MAKEINTRESOURCE(IDC_CURSIZENWSE));
		return 0;
	case WM_SETCURSOR:
		switch (LOWORD(lParam)) {
		case HTTOP:
		case HTBOTTOM:
			SetCursor(hCurSizeNs);
			return TRUE;
		case HTLEFT:
		case HTRIGHT:
			SetCursor(hCurSizeWe);
			return TRUE;
		case HTBOTTOMLEFT:
		case HTTOPRIGHT:
			SetCursor(hCurSizeNesw);
			return TRUE;
		case HTBOTTOMRIGHT:
		case HTTOPLEFT:
			SetCursor(hCurSizeNwse);
			return TRUE;
		case HTCAPTION:
			SetCursor(LoadCursor(NULL,IDC_SIZEALL));
			return TRUE;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
