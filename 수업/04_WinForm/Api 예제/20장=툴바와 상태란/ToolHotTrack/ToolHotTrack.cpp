#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ToolHotTrack");

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
	WndClass.style=0;
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	hWndMain=hWnd;
	
	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

#include <commctrl.h>
#include "resource.h"
#define ID_TOOLBAR 100
HWND hToolBar;
HIMAGELIST hNormal, hHot, hDisable;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	TBBUTTON ToolBtn[3]={
		{0,10,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{1,11,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{2,12,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0}
	};

	switch (iMessage) {
	case WM_CREATE:
		InitCommonControls();
		hToolBar=CreateWindow(TOOLBARCLASSNAME,NULL,WS_CHILD | WS_VISIBLE | 
			WS_BORDER | TBSTYLE_FLAT,0,0,0,0,
			hWnd,(HMENU)ID_TOOLBAR,g_hInst,NULL);
		SendMessage(hToolBar,TB_BUTTONSTRUCTSIZE,(WPARAM)sizeof(TBBUTTON),0);

		hNormal=ImageList_LoadImage(g_hInst, MAKEINTRESOURCE(IDB_NORMAL), 
			16, 1, CLR_NONE,IMAGE_BITMAP,0);
		hHot=ImageList_LoadImage(g_hInst, MAKEINTRESOURCE(IDB_HOT), 
			16, 1, CLR_NONE,IMAGE_BITMAP,0);
		hDisable=ImageList_LoadImage(g_hInst, MAKEINTRESOURCE(IDB_DISABLE), 
			16, 1, CLR_NONE,IMAGE_BITMAP,0);
		SendMessage(hToolBar,TB_SETIMAGELIST,0,(LPARAM)hNormal);
		SendMessage(hToolBar,TB_SETHOTIMAGELIST,0,(LPARAM)hHot);
		SendMessage(hToolBar,TB_SETDISABLEDIMAGELIST,0,(LPARAM)hDisable);
		SendMessage(hToolBar,TB_ADDBUTTONS,(WPARAM)3,(LPARAM)ToolBtn);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 10:
			SendMessage(hToolBar,TB_SETSTATE,12,MAKELONG(0,0));
			break;
		case 11:
			SendMessage(hToolBar,TB_SETSTATE,12,MAKELONG(TBSTATE_ENABLED,0));
			break;
		case 12:
			MessageBox(hWnd,"세번째 버튼을 선택했습니다","알림",MB_OK);
			break;
		}
		return 0;
	case WM_SIZE:
		SendMessage(hToolBar,TB_AUTOSIZE,0,0);
		return 0;
	case WM_DESTROY:
		ImageList_Destroy(hNormal);
		ImageList_Destroy(hHot);
		ImageList_Destroy(hDisable);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
