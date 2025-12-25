#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("TimeLock");

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

#include "ShReg.h"
#define KEY "Software\\MiyoungSoft\\TimeLock\\"
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	SYSTEMTIME st;
	TCHAR *Mes = "이 프로그램은 25일동안만 사용할 수 있습니다";
	WORD y,m,d;
	int elapse;
	switch (iMessage) {
	case WM_CREATE:
		// 처음 실행할 때 오늘 날짜를 레지스트리에 기록한다.
		y = SHRegReadInt(SHCU, KEY"Time", "Year", 0);
		if (y == 0) {
			GetSystemTime(&st);
			SHRegWriteInt(SHCU, KEY"Time", "Year", st.wYear);
			SHRegWriteInt(SHCU, KEY"Time", "Month", st.wMonth);
			SHRegWriteInt(SHCU, KEY"Time", "Day", st.wDay);
		} else {
			GetSystemTime(&st);
			m = SHRegReadInt(SHCU, KEY"Time", "Month", 0);
			d = SHRegReadInt(SHCU, KEY"Time", "Day", 0);
			elapse = (y-st.wYear)*365 + (m-st.wMonth)*30+(d-st.wDay);
			if ((elapse > 25) || (elapse < 0)) {
				MessageBox(hWnd, "25일간의 평가 기간이 종료되었습니다.", 
					"돈 내고 써!",MB_OK);
				return -1;
			}
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,10,10,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
