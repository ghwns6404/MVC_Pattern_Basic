#include <windows.h>
#include "GrpScrl.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("GrpScrlTest");

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

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	
	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

HWND hHorz,hVert;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static TCHAR Mes[256];

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		hHorz=CreateWindow("GrpScrl",NULL,WS_CHILD | WS_VISIBLE | GSS_HORZ,
			0,200,400,GetSystemMetrics(SM_CYHSCROLL),hWnd,(HMENU)1,g_hInst,NULL);
		SendMessage(hHorz,GSM_SETRANGE,0,400);
		hVert=CreateWindow("GrpScrl",NULL,WS_CHILD | WS_VISIBLE | GSS_VERT,
			400,0,GetSystemMetrics(SM_CXVSCROLL),200,hWnd,(HMENU)2,g_hInst,NULL);
		SendMessage(hVert,GSM_SETRANGE,0,200);
		SendMessage(hVert,GSM_SETBACKCOLOR,(WPARAM)RGB(255,255,0),0);
		SendMessage(hVert,GSM_SETTHUMBCOLOR,(WPARAM)RGB(255,0,0),0);
		return 0;
	case WM_HSCROLL:
		if (lParam == (LPARAM)hHorz) {
			InvalidateRect(hWnd,NULL,TRUE);
		}
		return 0;
	case WM_VSCROLL:
		if (lParam == (LPARAM)hVert) {
			InvalidateRect(hWnd,NULL,TRUE);
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		wsprintf(Mes,"수평 위치 = %d, 수직 위치=%d",
			SendMessage(hHorz,GSM_GETPOS,0,0),SendMessage(hVert,GSM_GETPOS,0,0));
		TextOut(hdc,10,10,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_KEYDOWN:
		switch (wParam) {
		case '1':
			SendMessage(hHorz,GSM_SETTSIZE,SendMessage(hHorz,GSM_GETTSIZE,0,0)+2,0);
			break;
		case '2':
			SendMessage(hHorz,GSM_SETTSIZE,SendMessage(hHorz,GSM_GETTSIZE,0,0)-2,0);
			break;
		case '3':
			SendMessage(hHorz,GSM_SETTGAP,SendMessage(hHorz,GSM_GETTGAP,0,0)+1,0);
			break;
		case '4':
			SendMessage(hHorz,GSM_SETTGAP,SendMessage(hHorz,GSM_GETTGAP,0,0)-1,0);
			break;
		case '5':
			SendMessage(hHorz,GSM_SETPOS,SendMessage(hHorz,GSM_GETPOS,0,0)-5,0);
			InvalidateRect(hWnd,NULL,TRUE);
			break;
		case '6':
			SendMessage(hHorz,GSM_SETPOS,SendMessage(hHorz,GSM_GETPOS,0,0)+5,0);
			InvalidateRect(hWnd,NULL,TRUE);
			break;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
