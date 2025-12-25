#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ChangeCursor");

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

HCURSOR hCurAppStart, hCurCross, hCurIBeam, hCurUpArrow;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	POINT pt;
	switch (iMessage) {
	case WM_CREATE:
		hCurAppStart=LoadCursor(NULL, IDC_APPSTARTING);
		hCurCross=LoadCursor(NULL, IDC_CROSS);
		hCurIBeam=LoadCursor(NULL, IDC_IBEAM);
		hCurUpArrow=LoadCursor(NULL, IDC_UPARROW);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		Rectangle(hdc,0,0,200,200);
		MoveToEx(hdc,0,100,NULL);
		LineTo(hdc,200,100);
		MoveToEx(hdc,100,0,NULL);
		LineTo(hdc,100,200);
		TextOut(hdc,10,50,"AppStart",8);
		TextOut(hdc,110,50,"Cross",5);
		TextOut(hdc,10,150,"IBeam",5);
		TextOut(hdc,110,150,"UpArrow",7);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_SETCURSOR:
		GetCursorPos(&pt);
		ScreenToClient(hWnd, &pt);
		if (pt.x > 0 && pt.x < 100 && pt.y > 0 && pt.y < 100)
			SetCursor(hCurAppStart);
		else if (pt.x > 100 && pt.x < 200 && pt.y > 0 && pt.y < 100)
			SetCursor(hCurCross);
		else if (pt.x > 0 && pt.x < 100 && pt.y > 100 && pt.y < 200)
			SetCursor(hCurIBeam);
		else if (pt.x > 100 && pt.x < 200 && pt.y > 100 && pt.y < 200)
			SetCursor(hCurUpArrow);
		else 
			break;
		return TRUE;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

