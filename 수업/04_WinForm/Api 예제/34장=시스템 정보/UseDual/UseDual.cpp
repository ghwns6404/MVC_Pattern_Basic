#define WINVER 0x0500
#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK ChildProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("UseDual");

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

	WndClass.hbrBackground=(HBRUSH)(COLOR_BTNFACE+1);
	WndClass.lpfnWndProc=ChildProc;
	WndClass.lpszClassName="ChildWnd";
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	hWndMain=hWnd;
	
	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes="메인 윈도우, 주 모니터에 나타납니다.";

	switch (iMessage) {
	case WM_CREATE:
		CreateWindow("ChildWnd","ChildWnd",WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
			NULL,(HMENU)NULL,g_hInst,NULL);
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

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor,HDC hdcMonitor,LPRECT lprcMonitor,LPARAM dwData)
{
	MONITORINFOEX mi;

	mi.cbSize=sizeof(MONITORINFOEX);
	GetMonitorInfo(hMonitor,&mi);
	if ((mi.dwFlags & MONITORINFOF_PRIMARY)==0) {
		(*(LPRECT)dwData)=*lprcMonitor;
		return FALSE;
	} else {
		return TRUE;
	}
}

LRESULT CALLBACK ChildProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes="보조 윈도우, 추가 모니터에 나타납니다.";
	TCHAR szTime[32];
	SYSTEMTIME st;
	RECT rt;

	switch (iMessage) {
	case WM_CREATE:
		if (GetSystemMetrics(SM_CMONITORS) > 1) {
			EnumDisplayMonitors(NULL,NULL,MonitorEnumProc,(LPARAM)&rt);
			MoveWindow(hWnd,rt.left,rt.top,500,300,FALSE);
		}
		ShowWindow(hWnd,SW_SHOW);
		SetTimer(hWnd,1,1000,NULL);
		return 0;
	case WM_TIMER:
		InvalidateRect(hWnd,NULL,TRUE);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		SetBkMode(hdc,TRANSPARENT);
		TextOut(hdc,10,10,Mes,lstrlen(Mes));
		GetLocalTime(&st);
		wsprintf(szTime,"현재 시간 = %d:%d:%d",st.wHour,st.wMinute,st.wSecond);
		TextOut(hdc,10,30,szTime,lstrlen(szTime));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd,1);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
