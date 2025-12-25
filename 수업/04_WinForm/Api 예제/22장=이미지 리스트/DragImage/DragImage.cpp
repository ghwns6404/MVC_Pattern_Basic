#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("DragImage");

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

#include <commctrl.h>
#include "resource.h"
HIMAGELIST IL;
BOOL bDrag;				// TRUE=현재 드래그 중
int x=100,y=100;		// 현재 위치
int hotx,hoty;			// 핫 스팟 좌표
int bx,by,cap;			// 경계선 두께, 타이틀 바의 높이
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int mx, my;
	HBRUSH hBrush;
	RECT rt;

	switch (iMessage) {
	case WM_CREATE:
		bDrag=FALSE;
		InitCommonControls();
		IL=ImageList_LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_DRAGIMAGE), 
			32, 1, RGB(255,255,255));
		// 드래그에 필요한 시스템 값
		bx=GetSystemMetrics(SM_CXSIZEFRAME);
		by=GetSystemMetrics(SM_CYSIZEFRAME);
		cap=GetSystemMetrics(SM_CYCAPTION);
		return 0;
	case WM_LBUTTONDOWN:
		mx=LOWORD(lParam);
		my=HIWORD(lParam);
		if (mx >= x && mx <=x+32 && my >= y && my <= y+32) {
			// 출력되어 있는 이미지를 먼저 지운다.
			hBrush=CreateSolidBrush(GetSysColor(COLOR_WINDOW));
			SetRect(&rt,x,y,x+32,y+32);
			hdc=GetDC(hWnd);
			FillRect(hdc,&rt,hBrush);
			ReleaseDC(hWnd,hdc);
			DeleteObject(hBrush);

			// 핫 스팟을 계산하고 드래그를 시작한다.
			hotx=mx-x;
			hoty=my-y;
			ImageList_BeginDrag(IL,0,hotx,hoty);
			bDrag=TRUE;
			ImageList_DragEnter(hWnd,mx+bx,my+by+cap);
			SetCapture(hWnd);
		}
		return 0;
	case WM_MOUSEMOVE:
		if (bDrag) {
			ImageList_DragMove(LOWORD(lParam)+bx, HIWORD(lParam)+by+cap);
		}
		return 0;
	case WM_LBUTTONUP:
		if (bDrag) {
			ImageList_DragLeave(hWnd);
			ImageList_EndDrag();
			bDrag=FALSE;
			x=LOWORD(lParam)-hotx;
			y=HIWORD(lParam)-hoty;
			InvalidateRect(hWnd,NULL,TRUE);
			ReleaseCapture();
		}
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		ImageList_Draw(IL,0,hdc,x,y,ILD_NORMAL);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		ImageList_Destroy(IL);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
