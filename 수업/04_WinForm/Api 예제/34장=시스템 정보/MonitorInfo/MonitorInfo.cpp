#define WINVER 0x0500
#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("MonitorInfo");

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
	hWndMain=hWnd;
	
	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

int y;
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor,HDC hdcMonitor,LPRECT lprcMonitor,LPARAM dwData)
{
	TCHAR sInfo[256];
	MONITORINFOEX mi;

	mi.cbSize=sizeof(MONITORINFOEX);
	GetMonitorInfo(hMonitor,&mi);
	wsprintf(sInfo,"모니터 핸들 = %x, 좌표=(%d,%d)-(%d,%d) %s",hMonitor,
		lprcMonitor->left,lprcMonitor->top,lprcMonitor->right,lprcMonitor->bottom,
		(mi.dwFlags & MONITORINFOF_PRIMARY)==0 ? "":"주 모니터");
	TextOut((HDC)dwData,10,y,sInfo,lstrlen(sInfo));
	y+=20;
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR Mes[256];
	int cMon;
	RECT rcVirt;

	switch (iMessage) {
	case WM_LBUTTONDOWN:
	case WM_DISPLAYCHANGE:
		InvalidateRect(hWnd,NULL,TRUE);
		UpdateWindow(hWnd);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		cMon=GetSystemMetrics(SM_CMONITORS);
		if (cMon == 0) {
			lstrcpy(Mes,"다중 모니터를 지원하지 않는 시스템입니다");
			TextOut(hdc,10,10,Mes,lstrlen(Mes));
		} else {
			wsprintf(Mes,"모니터 개수 = %d, 색상 포맷=%s",cMon, 
				GetSystemMetrics(SM_SAMEDISPLAYFORMAT) ? "모두 동일":"다름");
			TextOut(hdc,10,10,Mes,lstrlen(Mes));

			rcVirt.left=GetSystemMetrics(SM_XVIRTUALSCREEN);
			rcVirt.top=GetSystemMetrics(SM_YVIRTUALSCREEN);
			rcVirt.right=rcVirt.left+GetSystemMetrics(SM_CXVIRTUALSCREEN);
			rcVirt.bottom=rcVirt.top+GetSystemMetrics(SM_CYVIRTUALSCREEN);
			wsprintf(Mes,"가상 화면=(%d,%d)-(%d,%d)",rcVirt.left,rcVirt.top,
				rcVirt.right,rcVirt.bottom);
			TextOut(hdc,10,30,Mes,lstrlen(Mes));

			y=50;
			EnumDisplayMonitors(NULL,NULL,MonitorEnumProc,(LPARAM)hdc);
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
