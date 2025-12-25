#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("GdiObject");

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

//* 디폴트로 그리기
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (iMessage) {
	case WM_PAINT:
		hdc=BeginPaint(hWnd,&ps);
		Rectangle(hdc,50,50,300,200);
		EndPaint(hWnd,&ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
//*/

/* 스톡 브러시 예제
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH MyBrush,OldBrush;
	switch (iMessage) {
	case WM_PAINT:
		hdc=BeginPaint(hWnd,&ps);
		MyBrush=(HBRUSH)GetStockObject(GRAY_BRUSH);
		OldBrush=(HBRUSH)SelectObject(hdc,MyBrush);
		Rectangle(hdc,50,50,300,200);
		SelectObject(hdc,OldBrush);
		EndPaint(hWnd,&ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
//*/

/* 펜 오브젝트
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HPEN MyPen, OldPen;
	switch (iMessage) {
	case WM_PAINT:
		hdc=BeginPaint(hWnd,&ps);
		MyPen = CreatePen(PS_SOLID, 5, RGB(0,0,255));
		OldPen = (HPEN)SelectObject(hdc, MyPen);
		Rectangle(hdc,50,50,300,200);
		SelectObject(hdc, OldPen);
		DeleteObject(MyPen);
		EndPaint(hWnd,&ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
//*/

/* 브러시 오브젝트
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH MyBrush,OldBrush;
	HPEN MyPen, OldPen;
	switch (iMessage) {
	case WM_PAINT:
		hdc=BeginPaint(hWnd,&ps);
		MyBrush=CreateHatchBrush(HS_BDIAGONAL, RGB(255,0,255));
		OldBrush=(HBRUSH)SelectObject(hdc,MyBrush);
		MyPen=CreatePen(PS_SOLID, 5, RGB(0,0,255));
		OldPen=(HPEN)SelectObject(hdc, MyPen);
		Rectangle(hdc,50,50,300,200);
		SelectObject(hdc,OldBrush);
		SelectObject(hdc, OldPen);
		DeleteObject(MyBrush);
		DeleteObject(MyPen);
		EndPaint(hWnd,&ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
//*/