#include <windows.h>
#include "resource.h"
#include "GrpBtn.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("GrpBtnTest");

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

HWND hBtn1,hBtn2,hBtn3;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes="E:사용금지, S:보이기, M:이동, C:체크";

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		hBtn1=CreateWindow("GrpBtn",NULL,WS_CHILD | WS_VISIBLE | GBS_PUSH,
			10,10,16,16,hWnd,(HMENU)1,g_hInst,NULL);
		SendMessage(hBtn1,GBM_SETIMAGE,MAKEWPARAM(IDB_BTN1NORMAL,IDB_BTN1HOT),
			MAKEWPARAM(IDB_BTN1DOWN,IDB_BTN1DISABLE));
		hBtn2=CreateWindow("GrpBtn",NULL,WS_CHILD | WS_VISIBLE | GBS_PUSH,
			30,10,32,32,hWnd,(HMENU)2,g_hInst,NULL);
		SendMessage(hBtn2,GBM_SETIMAGE,MAKEWPARAM(IDB_BTN2NORMAL,IDB_BTN2HOT),
			MAKEWPARAM(IDB_BTN2DOWN,IDB_BTN2DISABLE));
		hBtn3=CreateWindow("GrpBtn",NULL,WS_CHILD | WS_VISIBLE | GBS_CHECK,
			80,10,24,24,hWnd,(HMENU)3,g_hInst,NULL);
		SendMessage(hBtn3,GBM_SETIMAGE,MAKEWPARAM(IDB_BTN3NORMAL,IDB_BTN3NORMAL),
			MAKEWPARAM(IDB_BTN3DOWN,IDB_BTN3NORMAL));
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 1:
			MessageBeep(0);
			break;
		case 2:
			MessageBox(hWnd,"두 번째 버튼을 눌렀습니다.","알림",MB_OK);
			break;
		case 3:
			if (SendMessage(hBtn3,GBM_GETSTATE,0,0) == GB_DOWN) {
				SetWindowText(hWnd,"체크 되었음");
			} else {
				SetWindowText(hWnd,"체크 안되었음");
			}
			break;
		}
		return 0;
	case WM_KEYDOWN:
		switch (wParam) {
		case 'E':
			EnableWindow(hBtn2,!IsWindowEnabled(hBtn2));
			break;
		case 'S':
			ShowWindow(hBtn2,IsWindowVisible(hBtn2) ? SW_HIDE:SW_SHOW);
			break;
		case 'M':
			RECT rt;
			GetWindowRect(hBtn2,&rt);
			ScreenToClient(hWnd,(LPPOINT)&rt);
			SetWindowPos(hBtn2,NULL,rt.left,rt.top+10,0,0,SWP_NOSIZE | SWP_NOZORDER);
			break;
		case 'C':
			eState NowState;
			NowState=(eState)SendMessage(hBtn3,GBM_GETSTATE,0,0);
			SendMessage(hBtn3,GBM_SETSTATE,NowState==GB_DOWN ? GB_NORMAL:GB_DOWN,0);
			break;
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,10,100,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
