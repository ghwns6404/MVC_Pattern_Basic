#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ImageList");

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;

	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)(COLOR_BTNFACE+1);
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
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	int i;

	switch (iMessage) {
	case WM_CREATE:
		InitCommonControls();
//*
		IL=ImageList_LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_FRUIT), 
			32, 1, RGB(0,0,255));
//*/
/*
		IL=ImageList_LoadImage(g_hInst, MAKEINTRESOURCE(IDB_FRUIT), 
			32, 1, RGB(0,0,255),IMAGE_BITMAP,0);
//*/
/* 파일에서 읽기
		IL=ImageList_LoadImage(g_hInst, "Fruit.bmp", 
			32, 1, RGB(0,0,255),IMAGE_BITMAP,LR_LOADFROMFILE);
//*/
/* 실행중에 아이콘 추가하기
		IL=ImageList_Create(32,32,ILC_COLOR4,3,1);
		ImageList_ReplaceIcon(IL,-1,LoadIcon(g_hInst,MAKEINTRESOURCE(IDI_ICON1)));
		ImageList_ReplaceIcon(IL,-1,LoadIcon(g_hInst,MAKEINTRESOURCE(IDI_ICON2)));
		ImageList_ReplaceIcon(IL,-1,LoadIcon(g_hInst,MAKEINTRESOURCE(IDI_ICON3)));
//*/
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
//* Draw로 그리기
		ImageList_Draw(IL,0,hdc,50,50,ILD_NORMAL);
//*/
/* Draw의 옵션들
		for (i=0;i<3;i++) 
			ImageList_Draw(IL, i, hdc, i*50,0,ILD_NORMAL);

		ImageList_Draw(IL, 0, hdc,   0,50,ILD_BLEND25);
		ImageList_Draw(IL, 0, hdc,  50,50,ILD_BLEND50);
		ImageList_Draw(IL, 0, hdc, 100,50,ILD_MASK);
		ImageList_Draw(IL, 0, hdc, 150,50,ILD_NORMAL);
		ImageList_Draw(IL, 0, hdc, 200,50,ILD_TRANSPARENT);
//*/

// DrawEx로 그리기
//		ImageList_DrawEx(IL,0,hdc,0,0,25,25,CLR_DEFAULT,CLR_DEFAULT,ILD_NORMAL);

/* DrawIndirect로 그리기
		IMAGELISTDRAWPARAMS dp;
		dp.cbSize=sizeof(IMAGELISTDRAWPARAMS);
		dp.himl=IL;				// IL 이미지 리스트의
		dp.i=1;					// 두번째 이미지를
		dp.hdcDst=hdc;			// hdc의
		dp.x=10;				// (10,10)에 출력하되
		dp.y=10;
		dp.xBitmap=10;			// 이미지의 (10,10)에서
		dp.yBitmap=10;
		dp.cx=25;				// (25,25)까지만 출력한다.
		dp.cy=25;
		dp.rgbBk=CLR_DEFAULT;
		dp.rgbFg=CLR_DEFAULT;
		dp.fStyle=ILD_NORMAL;
		dp.dwRop=SRCCOPY;
		ImageList_DrawIndirect(&dp);
//*/
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		ImageList_Destroy(IL);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

