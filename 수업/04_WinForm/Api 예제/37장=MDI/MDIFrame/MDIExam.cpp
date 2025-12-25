// 간단한 MDI 예제
#include <windows.h>
#include "resource.h"

// 함수 원형 및 전역 변수
LRESULT CALLBACK MDIWndProc(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK MDIChildProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;					// 인스턴스 핸들
HWND g_hFrameWnd;					// 메인 프레임 윈도우 핸들
HWND g_hMDIClient;					// 클라이언트 윈도우 핸들
LPSTR lpszClass=TEXT("MDIExam");	// 메인 윈도우의 윈도우 클래스
int ChildNum=1;
HWND hList,hEdit;

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
		  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;
	
	// MDI Frame 윈도우 클래스 등록
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=MDIWndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style=0;
	RegisterClass(&WndClass);

	// MDI Frame 윈도우 만듬
	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	g_hFrameWnd=hWnd;

	// MDI Child 윈도우 클래스 등록
	WndClass.lpszClassName="MDIExamChild";
	WndClass.lpfnWndProc=MDIChildProc;
	WndClass.hIcon=LoadIcon(NULL,IDI_ASTERISK);
	WndClass.lpszMenuName=NULL;
	WndClass.cbWndExtra=sizeof(DWORD);			// 여분의 메모리
	RegisterClass(&WndClass);

	// MDI의 메시지 루프
	while (GetMessage(&Message,NULL,0,0)) {
		if (!TranslateMDISysAccel(g_hMDIClient, &Message)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}
	return (int)Message.wParam;
}

// 프레임 윈도우의 메시지 프로시저
LRESULT CALLBACK MDIWndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	CLIENTCREATESTRUCT ccs;
	MDICREATESTRUCT mcs;
	switch (iMessage) {
	case WM_CREATE:
		// MDI Client 윈도우 만듬
		ccs.hWindowMenu=GetSubMenu(GetMenu(hWnd), 1);
		ccs.idFirstChild=IDM_WINDOWCHILD;
		g_hMDIClient=CreateWindow("MDICLIENT", NULL, WS_CHILD | WS_VSCROLL | 
			WS_HSCROLL | WS_CLIPCHILDREN,
			0,0,0,0,hWnd,(HMENU)NULL, g_hInst, (LPSTR)&ccs);
		ShowWindow(g_hMDIClient, SW_SHOW);
		hList=CreateWindow("listbox",NULL,WS_CHILD | WS_VISIBLE | WS_BORDER |
			LBS_NOINTEGRALHEIGHT,0,0,0,0,hWnd,(HMENU)1,g_hInst,NULL);
		hEdit=CreateWindow("edit",NULL,WS_CHILD | WS_VISIBLE | WS_BORDER | 
			ES_MULTILINE,10,10,200,25,hWnd,(HMENU)2,g_hInst,NULL);
		SendMessage(hList,LB_ADDSTRING,0,(LPARAM)"리스트 박스입니다.");
		SetWindowText(hEdit,"에디트 박스입니다.");
		return 0;
	case WM_SIZE:
		if (wParam != SIZE_MINIMIZED) {
			MoveWindow(hList,0,0,200,HIWORD(lParam),TRUE);
			MoveWindow(hEdit,200,HIWORD(lParam)-100,LOWORD(lParam)-200,100,TRUE);
			MoveWindow(g_hMDIClient,200,0,LOWORD(lParam)-200,HIWORD(lParam)-100,TRUE);
		}
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		// 새로운 차일드 윈도우를 만든다.
		case ID_FILENEW:
			mcs.szClass="MDIExamChild";
			mcs.szTitle="Child";
			mcs.hOwner=g_hInst;
			mcs.x=mcs.y=CW_USEDEFAULT;
			mcs.cx=mcs.cy=CW_USEDEFAULT;
			mcs.style=MDIS_ALLCHILDSTYLES;
			SendMessage(g_hMDIClient, WM_MDICREATE, 0, 
				(LPARAM)(LPMDICREATESTRUCT)&mcs);
			break;
		// 바둑판식 정렬
		case ID_WIN_TILE:
			SendMessage(g_hMDIClient, WM_MDITILE, 
				(WPARAM)MDITILE_HORIZONTAL, 0);
			break;
		// 계단식 정렬
		case ID_WIN_CASCADE:
			SendMessage(g_hMDIClient, WM_MDICASCADE, 
				(WPARAM)MDITILE_SKIPDISABLED, 0);
			break;
		// 아이콘 정렬
		case ID_WIN_ARRANGE:
			SendMessage(g_hMDIClient, WM_MDIICONARRANGE, 0, 0);
			break;
		}
		break;		// 여기서 "return 0"하면 안된다. 반드시 break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefFrameProc(hWnd,g_hMDIClient,iMessage,wParam,lParam));
}

// 차일드 윈도우의 메시지 프로시저
LRESULT CALLBACK MDIChildProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR str[128];
	switch (iMessage) {
	case WM_CREATE:
		wsprintf(str, "Child %d", ChildNum);
		SetWindowLong(hWnd, 0, ChildNum);
		ChildNum++;
		SetWindowText(hWnd, str);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		wsprintf(str,"This is a MDI %dth Child window", GetWindowLong(hWnd, 0));
		TextOut(hdc,0,0,str,lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;
	// WM_DESTROY 메시지를 처리하지 않아도 된다.
	}
	return(DefMDIChildProc(hWnd,iMessage,wParam,lParam));
}