#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("MoveCursor");

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

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int delta=1;
	BOOL bPress;
	POINT pt;
	TCHAR *Mes="커서키로 마우스를 이동합니다.";
	switch (iMessage) {
	case WM_CREATE:
		SetTimer(hWnd,1,20,NULL);
		return 0;
	case WM_TIMER:
		bPress=FALSE;
		GetCursorPos(&pt);
		if (GetKeyState(VK_LEFT) & 0x8000) {
			pt.x-=delta;
			bPress=TRUE;
		}
		if (GetKeyState(VK_RIGHT) & 0x8000) {
			pt.x+=delta;
			bPress=TRUE;
		}
		if (GetKeyState(VK_UP) & 0x8000) {
			pt.y-=delta;
			bPress=TRUE;
		}
		if (GetKeyState(VK_DOWN) & 0x8000) {
			pt.y+=delta;
			bPress=TRUE;
		}
		if (bPress) {
			delta=min(delta+1,32);
		} else {
			delta=1;
		}
		SetCursorPos(pt.x, pt.y);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd,&ps);
		TextOut(hdc,10,10,Mes,lstrlen(Mes));
		EndPaint(hWnd,&ps);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd,1);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
