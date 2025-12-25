#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("TabImage");

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

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS,
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
HWND hTab;
HWND hStatic;
HIMAGELIST IL;

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	TCITEM tie;
	TCHAR *arNum[3]={"첫번째 페이지입니다. 탭 컨트롤로 페이지를 전환합니다.",
		"여기는 두번째 페이지입니다.",
		"마지막 페이지입니다."};

	switch (iMessage) {
	case WM_CREATE:
		InitCommonControls();
		hTab=CreateWindow(WC_TABCONTROL,"",WS_CHILD | WS_VISIBLE 
			| WS_CLIPSIBLINGS | TCS_FIXEDWIDTH | TCS_FORCEICONLEFT,
			0,0,0,0,hWnd,(HMENU)0,g_hInst,NULL);
		TabCtrl_SetItemSize(hTab,200,22);

		IL=ImageList_LoadImage(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1), 
			16, 1, RGB(255,255,255),IMAGE_BITMAP,0);
		TabCtrl_SetImageList(hTab,IL);
		hStatic=CreateWindow("static",arNum[0],WS_CHILD | WS_VISIBLE,
			0,0,0,0,hWnd,(HMENU)1,g_hInst,NULL);

		tie.mask=TCIF_TEXT | TCIF_IMAGE;
		tie.pszText="one";
		tie.iImage=0;
		TabCtrl_InsertItem(hTab,0,&tie);
		tie.pszText="two";
		tie.iImage=1;
		TabCtrl_InsertItem(hTab,1,&tie);
		tie.pszText="three";
		tie.iImage=2;
		TabCtrl_InsertItem(hTab,2,&tie);
		return 0;
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code) {
		case TCN_SELCHANGE:
			SetWindowText(hStatic,arNum[TabCtrl_GetCurSel(hTab)]);
			break;
		}
		return 0;
	case WM_SIZE:
		MoveWindow(hTab,0,0,LOWORD(lParam),HIWORD(lParam),TRUE);
		MoveWindow(hStatic,LOWORD(lParam)/2-250,HIWORD(lParam)/2,500,25,TRUE);
		return 0;
	case WM_DESTROY:
		ImageList_Destroy(IL);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
