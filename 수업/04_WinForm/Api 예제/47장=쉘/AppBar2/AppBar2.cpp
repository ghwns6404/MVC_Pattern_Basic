#include <windows.h>
#include <shlwapi.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPSTR lpszClass=TEXT("AppBar2");

UINT gEdge;						// 현재 도킹된 면
BOOL gAlwaysOnTop=FALSE;		// 항상 위
BOOL gAutoHide=FALSE;			// 자동 숨김
BOOL gThick=50;					// 앱바의 두께

#define WM_MYAPPCALLBACK WM_USER+100

// 함수 원형들
BOOL MyABRegister(HWND hWnd,UINT uCallBack);
BOOL MyABUnRegister(HWND hWnd);
BOOL MyABSetPos(HWND hWnd,UINT uEdge,UINT nPixel,BOOL bSetPos);

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
		  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;
	
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)GetStockObject(LTGRAY_BRUSH);
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=NULL;
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	// 타이틀 바가 없는 윈도우를 만든다. 크기는 아무래도 상관없다.
	hWnd=CreateWindowEx(WS_EX_TOOLWINDOW,lpszClass,"",WS_POPUP | WS_THICKFRAME,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	
	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

// 앱바를 등록한다.
BOOL MyABRegister(HWND hWnd,UINT uCallBack)
{
	APPBARDATA abd;

	abd.cbSize = sizeof(APPBARDATA);
	abd.hWnd = hWnd;
    abd.uCallbackMessage = uCallBack;
	if (!SHAppBarMessage(ABM_NEW, &abd))
		return FALSE;
	return TRUE;
}

// 앱바를 해제한다.
BOOL MyABUnRegister(HWND hWnd)
{
	APPBARDATA abd;

	abd.cbSize = sizeof(APPBARDATA);
	abd.hWnd = hWnd;
	if (!SHAppBarMessage(ABM_REMOVE, &abd))
		return FALSE;
	return TRUE;
}

// 앱바의 위치를 지정한다.
// uEdge:고정할 위치. nPixel:앱바의 두께, bSetPos:위치 확정 여부
BOOL MyABSetPos(HWND hWnd,UINT uEdge,UINT nPixel,BOOL bSetPos)
{
	APPBARDATA abd;

	abd.cbSize = sizeof(APPBARDATA);     
	abd.hWnd = hWnd;

	// 사용 가능한 스크린 영역을 조사한다.
	abd.uEdge = uEdge;
	SetRect(&abd.rc, 0,0,GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN));
	SHAppBarMessage(ABM_QUERYPOS, &abd);

	// 원하는 영역을 계산하여 설정한다.
	switch (uEdge) {
	case ABE_LEFT:
		abd.rc.right = abd.rc.left + nPixel;
		break;
	case ABE_RIGHT:
		abd.rc.left = abd.rc.right - nPixel;
		break;
	case ABE_TOP:
		abd.rc.bottom = abd.rc.top + nPixel;
		break;
	case ABE_BOTTOM:
		abd.rc.top = abd.rc.bottom - nPixel;
		break;
	}

	// 엡바 영역의 위치를 확정한다.
	if (bSetPos) {
		gEdge = uEdge;
		SHAppBarMessage(ABM_SETPOS, &abd);
	}

	// 실제로 윈도우를 이동시킨다.
	MoveWindow(hWnd, abd.rc.left, abd.rc.top, 
		abd.rc.right - abd.rc.left, abd.rc.bottom - abd.rc.top, TRUE); 

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HMENU hMenu, hPopup;

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		MyABRegister(hWnd,WM_MYAPPCALLBACK);
		MyABSetPos(hWnd,ABE_RIGHT,gThick,TRUE);
		return 0;
	case WM_CONTEXTMENU:
		hMenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_POPUP));
		hPopup = GetSubMenu(hMenu, 0);
		if (gAlwaysOnTop) CheckMenuItem(hPopup,IDM_ALWAYSONTOP,MF_BYCOMMAND|MF_CHECKED);
		if (gAutoHide) CheckMenuItem(hPopup,IDM_AUTOHIDE,MF_BYCOMMAND|MF_CHECKED);
		CheckMenuItem(hPopup,IDM_LEFT+gEdge,MF_BYCOMMAND|MF_CHECKED);
		TrackPopupMenu(hPopup, TPM_LEFTALIGN, LOWORD(lParam), HIWORD(lParam), 
			0, hWnd, NULL);
		DestroyMenu(hMenu);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDM_LEFT:
			if (gAutoHide==FALSE)
				MyABSetPos(hWndMain,ABE_LEFT,gThick,TRUE);
			break;
		case IDM_TOP:
			if (gAutoHide==FALSE)
				MyABSetPos(hWndMain,ABE_TOP,gThick,TRUE);
			break;
		case IDM_RIGHT:
			if (gAutoHide==FALSE)
				MyABSetPos(hWndMain,ABE_RIGHT,gThick,TRUE);
			break;
		case IDM_BOTTOM:
			if (gAutoHide==FALSE)
				MyABSetPos(hWndMain,ABE_BOTTOM,gThick,TRUE);
			break;
		case IDM_ALWAYSONTOP:
			break;
		case IDM_AUTOHIDE:
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		}
		return 0;
	case WM_DESTROY:
		MyABUnRegister(hWnd);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

