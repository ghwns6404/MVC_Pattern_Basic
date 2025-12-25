#define WINVER 0x0500
#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("PaintMulti");

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

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor,HDC hdcMonitor,LPRECT lprcMonitor,LPARAM dwData)
{
	HBRUSH Brush,OldBrush;
	HPEN Pen,OldPen;
	RECT rtRect,rtPaint;

	if (GetDeviceCaps(hdcMonitor,BITSPIXEL) == 16) {
		Brush=CreateSolidBrush(RGB(0,0,255));
		Pen=CreatePen(PS_SOLID,1,RGB(0,0,255));
	} else {
		Brush=CreateSolidBrush(RGB(255,0,0));
		Pen=CreatePen(PS_SOLID,1,RGB(255,0,0));
	}
	SetRect(&rtRect,10,50,1000,150);
	IntersectRect(&rtPaint,&rtRect,lprcMonitor);
	OldBrush=(HBRUSH)SelectObject(hdcMonitor,Brush);
	OldPen=(HPEN)SelectObject(hdcMonitor,Pen);
	Rectangle(hdcMonitor,rtPaint.left,rtPaint.top,rtPaint.right,rtPaint.bottom);
	DeleteObject(SelectObject(hdcMonitor,OldBrush));
	DeleteObject(SelectObject(hdcMonitor,OldPen));
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes="16비트 색상 모니터에서는 파란색, 그외의 모니터에서는 빨간색";

	switch (iMessage) {
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,10,10,Mes,lstrlen(Mes));
		EnumDisplayMonitors(hdc,NULL,MonitorEnumProc,NULL);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
