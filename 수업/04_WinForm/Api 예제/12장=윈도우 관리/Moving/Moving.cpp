#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("Moving");

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

//*
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int sx,sy;
	int t;
	LPWINDOWPOS wp;
	TCHAR *Mes=TEXT("화면 가장자리에 밀착합니다.");
	switch (iMessage) {
	case WM_CREATE:
		sx=GetSystemMetrics(SM_CXSCREEN);
		sy=GetSystemMetrics(SM_CYSCREEN);
		return 0;
	case WM_WINDOWPOSCHANGING:
		wp=(LPWINDOWPOS)lParam;
		t=wp->x;
		if (t < 30 && t > -30) t=0;
		if (t+wp->cx > sx-30 && t+wp->cx < sx+30)
			t=sx-wp->cx;
		wp->x=t;

		t=wp->y;
		if (t < 30 && t > -30) t=0;
		if (t+wp->cy > sy-30 && t+wp->cy < sy+30)
			t=sy-wp->cy;
		wp->y=t;
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,100,100,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
//*/

/*
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes=TEXT("16픽셀 단위로만 움직입니다.");
	switch (iMessage) {
	case WM_WINDOWPOSCHANGING:
		((LPWINDOWPOS)lParam)->x &= 0xfffffff0;
		((LPWINDOWPOS)lParam)->y &= 0xfffffff0;
		((LPWINDOWPOS)lParam)->cx &= 0xfffffff0;
		((LPWINDOWPOS)lParam)->cy &= 0xfffffff0;
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,100,100,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
//*/