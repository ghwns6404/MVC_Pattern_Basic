#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("gang");

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

	hWnd=CreateWindow(lpszClass,lpszClass,WS_BORDER | WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT,CW_USEDEFAULT,400,300,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	hWndMain=hWnd;
	
	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

#include "resource.h"
HBITMAP hBit, hBaby;
TCHAR szGang[]="Gang Version 1.0\r\n\r\nÃÑ °¨µ¶ : ±è Á¤¼ö\r\n"
	"°³¹ßÀÚ : ±è »óÇü\r\n»çÁø ¸ðµ¨ : ±è ÇÑ½½\r\nÇùÂù : ¹Ú ¹Ì¿µ";
int my;

void DrawBitmap(HDC hdc,int x,int y,HBITMAP hBit)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx,by;
	BITMAP bit;

	MemDC=CreateCompatibleDC(hdc);
	OldBitmap=(HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit,sizeof(BITMAP),&bit);
	bx=bit.bmWidth;
	by=bit.bmHeight;

	BitBlt(hdc,x,y,bx,by,MemDC,0,0,SRCCOPY);

	SelectObject(MemDC,OldBitmap);
	DeleteDC(MemDC);
}

void OnTimer()
{
	RECT crt;
	HDC hdc,hMemDC;
	HBITMAP OldBit;
	HFONT font, oldfont;
	RECT grt;
	int i,j;

	GetClientRect(hWndMain,&crt);
	hdc=GetDC(hWndMain);

	if (hBit==NULL) {
		hBit=CreateCompatibleBitmap(hdc,crt.right,crt.bottom);
	}
	hMemDC=CreateCompatibleDC(hdc);
	OldBit=(HBITMAP)SelectObject(hMemDC,hBit);

	DrawBitmap(hMemDC,0,0,hBaby);
	SetBkMode(hMemDC,TRANSPARENT);

	font=CreateFont(30,0,0,0,0,0,0,0,HANGEUL_CHARSET,3,2,1,
		VARIABLE_PITCH | FF_ROMAN,"±Ã¼­");
	oldfont=(HFONT)SelectObject(hMemDC,font);

	my--;
	if (my==20) {
		KillTimer(hWndMain,1);
	}

	SetTextColor(hMemDC,RGB(192,192,192));
	for (i=-1;i<=1;i++) {
		for (j=-1;j<=1;j++) {
			SetRect(&grt,10+i,my+j,400+i,my+300+j);
			DrawText(hMemDC,szGang,-1,&grt,DT_WORDBREAK);
		}
	}

	SetTextColor(hMemDC,RGB(32,32,32));
	SetRect(&grt,10,my,400,my+300);
	DrawText(hMemDC,szGang,-1,&grt,DT_WORDBREAK);

	SelectObject(hMemDC,oldfont);
	DeleteObject(font);

	SelectObject(hMemDC,OldBit);
	DeleteDC(hMemDC);
	ReleaseDC(hWndMain,hdc);
	InvalidateRect(hWndMain,NULL,FALSE);
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMessage) {
	case WM_CREATE:
		hBaby=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BABY));
	case WM_LBUTTONDOWN:
		SetTimer(hWnd,1,50,NULL);
		my=310;
		return 0;
	case WM_TIMER:
		OnTimer();
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		if (hBit) DrawBitmap(hdc,0,0,hBit);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		if (hBit) {
			DeleteObject(hBit);
		}
		DeleteObject(hBaby);
		PostQuitMessage(0);
		KillTimer(hWnd,1);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
