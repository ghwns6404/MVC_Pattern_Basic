#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("MultiPopup");

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

#include "resource.h"
RECT rtChina={200,10,400,90};
RECT rtPizza={50,150,200,300};
RECT rtChicken={300,180,450,250};
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HMENU hMenu, hPopup;
	POINT pt;
	int Sub;

	switch (iMessage) {
	case WM_CONTEXTMENU:
		pt.x=(short)LOWORD(lParam);
		pt.y=(short)HIWORD(lParam);
		ScreenToClient(hWnd,&pt);
		Sub=-1;
		if (PtInRect(&rtChina,pt)) Sub=0;
		if (PtInRect(&rtPizza,pt)) Sub=1;
		if (PtInRect(&rtChicken,pt)) Sub=2;
		if (Sub==-1) return 0;

		hMenu=LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU1));
		hPopup=GetSubMenu(hMenu, Sub);

		TrackPopupMenu(hPopup, TPM_LEFTALIGN, (short)LOWORD(lParam), 
			(short)HIWORD(lParam), 0, hWnd, NULL);
		DestroyMenu(hMenu);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		SetBkMode(hdc,TRANSPARENT);
		FillRect(hdc,&rtChina,(HBRUSH)GetStockObject(GRAY_BRUSH));
		DrawText(hdc,"중국집",-1,&rtChina,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		FillRect(hdc,&rtPizza,(HBRUSH)GetStockObject(LTGRAY_BRUSH));
		DrawText(hdc,"피자집",-1,&rtPizza,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		FillRect(hdc,&rtChicken,(HBRUSH)GetStockObject(DKGRAY_BRUSH));
		DrawText(hdc,"치킨집",-1,&rtChicken,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
