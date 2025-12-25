#include <windows.h>
#include "resource.h"

// 프레임 윈도우와 차일드의 윈도우 프로시저
LRESULT CALLBACK MDIWndProc(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK MDIDrawProc(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK MDIEditProc(HWND,UINT,WPARAM,LPARAM);

// 전역 변수들
LPSTR lpszClass=TEXT("MultiMDI");
HINSTANCE g_hInst;
HWND g_hFrameWnd;
HWND g_hMDIClient;
HMENU hMenu1, hMenu2, hMenu3;
HMENU hMenu1W,hMenu2W,hMenu3W;
int EditNum=1;
int DrawNum=1;

// 텍스트 에디터 차일드의 개별 정보 구조체
struct tagEditData {
	int Num;
	HWND hEdit;
};

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
		  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;

	// 메인 윈도우(프레임 윈도우) 클래스 등록
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)COLOR_APPWORKSPACE+1;
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=MDIWndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=NULL;
	WndClass.style=0;
	RegisterClass(&WndClass);

	// 프레임 윈도우 만듬
	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	g_hFrameWnd=hWnd;

	// 텍스트 에디터 차일드 윈도우 클래스 등록
	WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszClassName=TEXT("MDIExamEdit");
	WndClass.lpfnWndProc=MDIEditProc;
	WndClass.hIcon=LoadIcon(NULL,IDI_ASTERISK);
	WndClass.lpszMenuName=NULL;
	WndClass.cbWndExtra=sizeof(DWORD_PTR);
	RegisterClass(&WndClass);

	// 그래픽 에디터 차일드 윈도우 클래스 등록
	WndClass.lpszClassName=TEXT("MDIExamDraw");
	WndClass.lpfnWndProc=MDIDrawProc;
	WndClass.hIcon=LoadIcon(NULL,IDI_ERROR);
	WndClass.lpszMenuName=NULL;
	WndClass.cbWndExtra=sizeof(DWORD);
	WndClass.style=CS_DBLCLKS;
	RegisterClass(&WndClass);

	while (GetMessage(&Message,NULL,0,0)) {
		if (!TranslateMDISysAccel(g_hMDIClient, &Message)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}
	return Message.wParam;
}

LRESULT CALLBACK MDIWndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	CLIENTCREATESTRUCT ccs;
	MDICREATESTRUCT mcs;
	switch (iMessage) {
	case WM_CREATE:
		// 각 차일드의 메뉴 핸들과 윈도우 메뉴를 미리 구해 놓는다.
		hMenu1=LoadMenu(g_hInst,MAKEINTRESOURCE(IDR_MENU1));
		hMenu2=LoadMenu(g_hInst,MAKEINTRESOURCE(IDR_MENU2));
		hMenu3=LoadMenu(g_hInst,MAKEINTRESOURCE(IDR_MENU3));
		hMenu1W=GetSubMenu(hMenu1,0);
		hMenu2W=GetSubMenu(hMenu2,1);
		hMenu3W=GetSubMenu(hMenu3,1);

		// 차일드가 없을 때는 hMenu1을 사용한다.
		SetMenu(hWnd,hMenu1);
		
		// 프레임 윈도우 만듬
		ccs.hWindowMenu=hMenu1W;
		ccs.idFirstChild=IDM_WINDOWCHILD;
		g_hMDIClient=CreateWindow(TEXT("MDICLIENT"), NULL, WS_CHILD | WS_VSCROLL | 
			WS_HSCROLL | WS_CLIPCHILDREN,
			0,0,0,0,hWnd,(HMENU)NULL, g_hInst, (LPSTR)&ccs);
		ShowWindow(g_hMDIClient, SW_SHOW);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_FILE_NEWEDIT:
			mcs.szClass=TEXT("MDIExamEdit");
			mcs.szTitle=TEXT("Edit");
			mcs.hOwner=g_hInst;
			mcs.x=mcs.y=CW_USEDEFAULT;
			mcs.cx=mcs.cy=CW_USEDEFAULT;
			mcs.style=MDIS_ALLCHILDSTYLES;
			SendMessage(g_hMDIClient, WM_MDICREATE, 0, 
				(LPARAM)(LPMDICREATESTRUCT)&mcs);
			break;
		case ID_FILE_NEWDRAW:
			mcs.szClass=TEXT("MDIExamDraw");
			mcs.szTitle=TEXT("Draw");
			mcs.hOwner=g_hInst;
			mcs.x=mcs.y=CW_USEDEFAULT;
			mcs.cx=mcs.cy=CW_USEDEFAULT;
			mcs.style=MDIS_ALLCHILDSTYLES;
			SendMessage(g_hMDIClient, WM_MDICREATE, 0, 
				(LPARAM)(LPMDICREATESTRUCT)&mcs);
			break;
		case ID_WIN_TILE:
			SendMessage(g_hMDIClient, WM_MDITILE, 
				(WPARAM)MDITILE_HORIZONTAL, 0);
			break;
		case ID_WIN_CASCADE:
			SendMessage(g_hMDIClient, WM_MDICASCADE, 
				(WPARAM)MDITILE_SKIPDISABLED, 0);
			break;
		case ID_WIN_ARRANGE:
			SendMessage(g_hMDIClient, WM_MDIICONARRANGE, 0, 0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefFrameProc(hWnd,g_hMDIClient,iMessage,wParam,lParam));
}

// 텍스트 에디터의 윈도우 프로시저
LRESULT CALLBACK MDIEditProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	TCHAR str[128];
	HWND hEdit;
	tagEditData *pED;
	switch (iMessage) {
	case WM_CREATE:
		// 캡션에 차일드 번호를 출력한다.
		wsprintf(str, TEXT("Edit %d"), EditNum);
		SetWindowText(hWnd, str);

		// 클라이언트 영역에 에디트 컨트롤을 배치한다.
		hEdit=CreateWindow(TEXT("edit"),NULL,WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL 
			| ES_MULTILINE | ES_AUTOVSCROLL | WS_HSCROLL | WS_VSCROLL,
			0,0,0,0,hWnd,(HMENU)100,g_hInst,NULL);

		// 에디트 컨트롤의 핸들과 차일드 번호를 구조체에 작성한 후
		// 여분 메모리에 이 포인터를 저장한다.
		pED=(tagEditData *)malloc(sizeof(tagEditData));
		pED->hEdit=hEdit;
		pED->Num=EditNum;
		SetWindowLongPtr(hWnd,0,(LONG)pED);
		EditNum++;
		return 0;
	case WM_MDIACTIVATE:
		// 활성화될 때 자신의 메뉴를 프레임에 부착시킨다.
		if (lParam==(LPARAM)hWnd)
			SendMessage(g_hMDIClient,WM_MDISETMENU,
			(WPARAM)hMenu3,(LPARAM)hMenu3W);
		else
			SendMessage(g_hMDIClient,WM_MDISETMENU,
			(WPARAM)hMenu1,(LPARAM)hMenu1W);
		DrawMenuBar(g_hFrameWnd);
		return 0;
	case WM_SIZE:
		pED=(tagEditData *)GetWindowLongPtr(hWnd,0);
		MoveWindow(pED->hEdit, 0, 0, LOWORD(lParam), HIWORD(lParam),TRUE);
		return 0;
	case WM_SETFOCUS:
		pED=(tagEditData *)GetWindowLongPtr(hWnd,0);
		SetFocus(pED->hEdit);
		return 0;
	case WM_DESTROY:
		pED=(tagEditData *)GetWindowLongPtr(hWnd,0);
		free(pED);
		break;
	}
	return(DefMDIChildProc(hWnd,iMessage,wParam,lParam));
}

// 드로우의 윈도우 프로시저
LRESULT CALLBACK MDIDrawProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	TCHAR str[128];
	static int x;
	static int y;
	static BOOL bNowDraw=FALSE;
	switch (iMessage) {
	case WM_CREATE:
		wsprintf(str, TEXT("Draw %d"), DrawNum);
		SetWindowLong(hWnd, 0, DrawNum);
		DrawNum++;
		SetWindowText(hWnd, str);
		return 0;
	case WM_MDIACTIVATE:
		if (lParam==(LPARAM)hWnd)
			SendMessage(g_hMDIClient,WM_MDISETMENU,
			(WPARAM)hMenu2,(LPARAM)hMenu2W);
		else
			SendMessage(g_hMDIClient,WM_MDISETMENU,
			(WPARAM)hMenu1,(LPARAM)hMenu1W);
		DrawMenuBar(g_hFrameWnd);
		return 0;
	case WM_LBUTTONDOWN:
		x=LOWORD(lParam);
		y=HIWORD(lParam);
		bNowDraw=TRUE;
		return 0;
	case WM_MOUSEMOVE:
		if (bNowDraw==TRUE) {
			hdc=GetDC(hWnd);
			MoveToEx(hdc,x,y,NULL);
			x=LOWORD(lParam);
			y=HIWORD(lParam);
			LineTo(hdc,x,y);
			ReleaseDC(hWnd,hdc);
		}
		return 0;
	case WM_LBUTTONUP:
		bNowDraw=FALSE;
		return 0;
	case WM_LBUTTONDBLCLK:
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	}
	return(DefMDIChildProc(hWnd,iMessage,wParam,lParam));
}