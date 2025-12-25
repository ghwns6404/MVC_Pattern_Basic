#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("FieldScroll");

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
		CW_USEDEFAULT,CW_USEDEFAULT,1000,480,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	
	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

void TransBlt(HDC hdc, int x, int y, HBITMAP hbitmap, COLORREF clrMask)
{
	BITMAP bm;
	COLORREF cColor;
	HBITMAP bmAndBack, bmAndObject, bmAndMem, bmSave;
	HBITMAP bmBackOld, bmObjectOld, bmMemOld, bmSaveOld;
	HDC		hdcMem, hdcBack, hdcObject, hdcTemp, hdcSave;
	POINT	ptSize;
	
	hdcTemp = CreateCompatibleDC(hdc);
	SelectObject(hdcTemp, hbitmap);
	GetObject(hbitmap, sizeof(BITMAP), (LPSTR)&bm);
	ptSize.x = bm.bmWidth;
	ptSize.y = bm.bmHeight;
	DPtoLP(hdcTemp, &ptSize,1);
	
	hdcBack   = CreateCompatibleDC(hdc);
	hdcObject = CreateCompatibleDC(hdc);
	hdcMem    = CreateCompatibleDC(hdc);
	hdcSave   = CreateCompatibleDC(hdc);
	
	bmAndBack   = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
	bmAndObject = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
	bmAndMem    = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
	bmSave      = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
	
	bmBackOld   = (HBITMAP) SelectObject(hdcBack, bmAndBack);
	bmObjectOld = (HBITMAP) SelectObject(hdcObject, bmAndObject);
	bmMemOld    = (HBITMAP) SelectObject(hdcMem, bmAndMem);
	bmSaveOld   = (HBITMAP) SelectObject(hdcSave, bmSave);
	
	SetMapMode(hdcTemp, GetMapMode(hdc));
	
	BitBlt(hdcSave, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);
	
	cColor = SetBkColor(hdcTemp, clrMask);
	
	BitBlt(hdcObject, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);
	
	SetBkColor(hdcTemp, cColor);
	
	BitBlt(hdcBack, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, NOTSRCCOPY);
	BitBlt(hdcMem , 0, 0, ptSize.x, ptSize.y, hdc      , x, y, SRCCOPY);
	BitBlt(hdcMem , 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, SRCAND);
	BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcBack  , 0, 0, SRCAND);
	BitBlt(hdcMem , 0, 0, ptSize.x, ptSize.y, hdcTemp  , 0, 0, SRCPAINT);
	BitBlt(hdc    , x, y, ptSize.x, ptSize.y, hdcMem   , 0, 0, SRCCOPY);
	BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcSave  , 0, 0, SRCCOPY);
	
	DeleteObject(SelectObject(hdcBack, bmBackOld));
	DeleteObject(SelectObject(hdcObject, bmObjectOld));
	DeleteObject(SelectObject(hdcMem, bmMemOld));
	DeleteObject(SelectObject(hdcSave, bmSaveOld));
	
	DeleteDC(hdcMem);
	DeleteDC(hdcBack);
	DeleteDC(hdcObject);
	DeleteDC(hdcSave);
	DeleteDC(hdcTemp);
}

HBITMAP hField, hBaby, hToilet;
int x=0;
int bx=100,by=100,tx=-1,ty;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int w;
	HDC hMemDC,hMemDC2;
	HBITMAP hBackBit,hOldBitmap;
	
	switch (iMessage) {
	case WM_CREATE:
		hField=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_FIELD));
		hBaby=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BABY));
		hToilet=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_TOILET));
		hWndMain=hWnd;
		SetTimer(hWnd,1,50,NULL);
		return 0;
	case WM_TIMER:
		x++;
		if (x>2048) x=0;
		if (tx!= -1) {
			tx+=10;
			if (tx > 1000) {
				tx=-1;
			}
		}
		InvalidateRect(hWnd,NULL,FALSE);
		return 0;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
			bx-=5;
			break;
		case VK_RIGHT:
			bx+=5;
			break;
		case VK_UP:
			by-=5;
			break;
		case VK_DOWN:
			by+=5;
			break;
		case VK_SPACE:
			if (tx == -1) {
				tx=bx+100;
				ty=by;
			}
			break;
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		hMemDC=CreateCompatibleDC(hdc);
		hBackBit=CreateCompatibleBitmap(hdc,1000,480);
		hOldBitmap=(HBITMAP)SelectObject(hMemDC,hBackBit);

		// 배경 그림
		hMemDC2=CreateCompatibleDC(hdc);
		SelectObject(hMemDC2,hField);
		w=min(2048-x,1000);
		BitBlt(hMemDC,0,0,w,480,hMemDC2,x,0,SRCCOPY);
		if (w < 1000) {
			BitBlt(hMemDC,w,0,1000-w,480,hMemDC2,0,0,SRCCOPY);
		}
		DeleteDC(hMemDC2);

		// 아기 그림
		TransBlt(hMemDC,bx,by,hBaby,RGB(255,0,255));

		// 총알 그림
		if (tx!= -1) {
			TransBlt(hMemDC,tx,ty,hToilet,RGB(255,0,255));
		}

		// 완성된 그림 전송
		BitBlt(hdc,0,0,1000,480,hMemDC,0,0,SRCCOPY);

		SelectObject(hMemDC,hOldBitmap);
		DeleteObject(hBackBit);
		DeleteDC(hMemDC);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		DeleteObject(hField);
		DeleteObject(hBaby);
		DeleteObject(hToilet);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
