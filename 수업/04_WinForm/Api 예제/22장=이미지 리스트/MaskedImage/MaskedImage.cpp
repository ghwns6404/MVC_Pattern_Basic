#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("MaskedImage");

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
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	int i;
	HBITMAP bit, bit2;

	switch (iMessage) {
	case WM_CREATE:
		InitCommonControls();

//*
		IL=ImageList_LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_FRUIT), 
			32, 1, CLR_DEFAULT);
//*/
/* 실행중에 마스크 이미지 만들기
		IL=ImageList_Create(32,32,ILC_COLOR4 | ILC_MASK,1,1);
		bit=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_FRUIT));
		ImageList_AddMasked(IL,bit,RGB(0,0,255));
		DeleteObject(bit);
//*/
/* 마스크 이미지를 따로 만들기
		IL=ImageList_Create(32,32,ILC_COLOR4 | ILC_MASK,1,1);
		bit=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_IMAGE));
		bit2=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_MASK));
		ImageList_Add(IL,bit,bit2);
		DeleteObject(bit);
		DeleteObject(bit2);
//*/
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		for (i=0;i<=150;i+=5) {
			MoveToEx(hdc,0,i,NULL);
			LineTo(hdc,150,i);
		}
		for (i=0;i<=150;i+=5) {
			MoveToEx(hdc,i,0,NULL);
			LineTo(hdc,i,150);
		}

		ImageList_SetBkColor(IL,RGB(0,255,0));
		//ImageList_Draw(IL,0,hdc,50,50,ILD_NORMAL);
		ImageList_DrawEx(IL,0,hdc,50,50,32,32,RGB(0,255,0),CLR_DEFAULT,
			ILD_TRANSPARENT);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		COLORREF ddd;
		ddd=ImageList_GetBkColor(IL);
		return 0;
	case WM_DESTROY:
		ImageList_Destroy(IL);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

