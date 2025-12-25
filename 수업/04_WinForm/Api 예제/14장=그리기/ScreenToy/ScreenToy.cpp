#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ScreenToy");

int sx,sy;
int cx,cy;
HDC hMemDC;
HBITMAP MyBitmap;

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;
	HDC hdc;
	
	sx=GetSystemMetrics(SM_CXSCREEN);
	sy=GetSystemMetrics(SM_CYSCREEN);
	cx=sx/2;
	cy=sy/2;
	
	// 메인 윈도우가 포커스를 받기 전에 바탕 화면 이미지를 구해 놓는다.
	hdc=GetDC(NULL);
	hMemDC=CreateCompatibleDC(hdc);
	MyBitmap=CreateCompatibleBitmap(hdc,sx,sy);
	SelectObject(hMemDC,MyBitmap);
	BitBlt(hMemDC,0,0,sx,sy,hdc,0,0,SRCCOPY);
	ReleaseDC(NULL,hdc);

	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=NULL;
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=NULL;
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd=CreateWindowEx(WS_EX_TOPMOST, lpszClass,lpszClass,WS_POPUP,
		0,0,0,0,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	
	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

// 수직으로 접기
void VertFolding(HDC hdc)
{
	int y;
	int step=25;

	for (y=step;y<=cy;y+=step) {
		StretchBlt(hdc,0,y,sx,sy-y*2,hMemDC,0,0,sx,sy,SRCCOPY);
		PatBlt(hdc,0,y-step,sx,step,BLACKNESS);
		PatBlt(hdc,0,sy-y,sx,step,BLACKNESS);
		Sleep(10);
		if (step > 5) step--;
	}
	PatBlt(hdc,0,0,sx,sy,BLACKNESS);
	GdiFlush();
}

// 수평으로 접기
void HorzFolding(HDC hdc)
{
	int x;
	int step=30;

	for (x=step;x<=cx;x+=step) {
		StretchBlt(hdc,x,0,sx-x*2,sy,hMemDC,0,0,sx,sy,SRCCOPY);
		PatBlt(hdc,x-step,0,step,sy,WHITENESS);
		PatBlt(hdc,sx-x,0,step,sy,WHITENESS);
		Sleep(10);
		if (step > 5) step--;
	}
	PatBlt(hdc,0,0,sx,sy,WHITENESS);
	GdiFlush();
}

// 중앙으로 작아지기
void Shrink(HDC hdc)
{
	int x,y;
	int xstep=cx/40,ystep=cy/40;

	for (x=xstep,y=ystep;x<=cx;x+=xstep,y+=ystep) {
		StretchBlt(hdc,x,y,sx-x*2,sy-y*2,hMemDC,0,0,sx,sy,SRCCOPY);
		Sleep(10);
	}
}

// 좌우로 이동하기
void ZigZag(HDC hdc)
{
	int x,y,i;
	int xstep=sx/100,ystep=sy/10;

	for (x=0;x<=sx;x+=xstep) {
		for (i=0,y=0;y<=sy;y+=ystep,i++) {
			if (i%2) {
				BitBlt(hdc,0,y,sx-x,ystep,hMemDC,x,y,SRCCOPY);
				PatBlt(hdc,sx-x,y,xstep,ystep,BLACKNESS);
			} else {
				BitBlt(hdc,x,y,sx-x,ystep,hMemDC,0,y,SRCCOPY);
				PatBlt(hdc,x-xstep,y,xstep,ystep,BLACKNESS);
			}
		}
		Sleep(20);
	}
	PatBlt(hdc,0,0,sx,sy,BLACKNESS);
	GdiFlush();
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		SetWindowPos(hWnd,NULL,0,0,sx,sy,SWP_NOZORDER);
		return 0;
	case WM_KEYDOWN:
		hdc=GetDC(hWnd);
		SetStretchBltMode(hdc,HALFTONE);
		switch (LOWORD(wParam)) {
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		case '1':
			VertFolding(hdc);
			break;
		case '2':
			HorzFolding(hdc);
			break;
		case '3':
			Shrink(hdc);
			break;
		case '4':
			ZigZag(hdc);
			break;
		}
		Sleep(500);
		BitBlt(hdc,0,0,sx,sy,hMemDC,0,0,SRCCOPY);
		ReleaseDC(hWnd,hdc);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		BitBlt(hdc,0,0,sx,sy,hMemDC,0,0,SRCCOPY);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		DeleteDC(hMemDC);
		DeleteObject(MyBitmap);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
