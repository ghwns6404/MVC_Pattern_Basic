#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("MakeDDB");

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
	HDC hdc,hMemDC;
	PAINTSTRUCT ps;
	BYTE Bits[]={0xc3,0xff,0xbd,0xff,0x66,0xff,0x5a,0xff,
		0x5a,0xff,0x66,0xff,0xbd,0xff,0xc3,0xff};
	HBITMAP hBitmap,hOldBitmap;
	BITMAP bit;

	switch (iMessage) {
	case WM_PAINT:
		hdc=BeginPaint(hWnd,&ps);
		hMemDC=CreateCompatibleDC(hdc);

//* 직접 생성
		hBitmap=CreateBitmap(8,8,1,1,Bits);
//*/

/* 구조체로 DDB 생성
		bit.bmType=0;
		bit.bmWidth=8;
		bit.bmHeight=8;
		bit.bmWidthBytes=2;
		bit.bmBitsPixel=1;
		bit.bmPlanes=1;
		bit.bmBits=Bits;
		hBitmap=CreateBitmapIndirect(&bit);
//*/

/* 래스터 데이터를 따로 설정하기
		hBitmap=CreateBitmap(8,8,1,1,NULL);
		SetBitmapBits(hBitmap,16,Bits);
//*/
		hOldBitmap=(HBITMAP)SelectObject(hMemDC,hBitmap);
		BitBlt(hdc,2,2,8,8,hMemDC,0,0,SRCCOPY);
		SelectObject(hMemDC,hOldBitmap);
		DeleteDC(hMemDC);
		DeleteObject(hBitmap);
		EndPaint(hWnd,&ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

