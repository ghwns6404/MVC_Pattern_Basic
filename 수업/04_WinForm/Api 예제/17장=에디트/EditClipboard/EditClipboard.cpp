#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("EditClipboard");

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
	WndClass.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1);
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

#define ID_EDIT 100
HWND hEdit;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	TCHAR text[128];
	DWORD Start, End;
	switch (iMessage) {
	// 에디트 컨트롤을 만든다.
	case WM_CREATE:
		hEdit=CreateWindow(TEXT("edit"),NULL,
			WS_CHILD | WS_BORDER | WS_VISIBLE | ES_MULTILINE | 
			ES_AUTOVSCROLL | WS_VSCROLL | WS_HSCROLL,
			10,10,400,400,hWnd,(HMENU)ID_EDIT,g_hInst,NULL);
		return 0;
	// 선택한 메뉴 항목에 따라 에디트로 적당한 메시지를 보낸다.
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDM_UNDO:
			SendMessage(hEdit, EM_UNDO,0,0);
			return 0;
		case IDM_CUT:
			SendMessage(hEdit, WM_CUT,0,0);
			break;
		case IDM_COPY:
			SendMessage(hEdit, WM_COPY,0,0);
			break;
		case IDM_PASTE:
			SendMessage(hEdit, WM_PASTE,0,0);
			break;
		case IDM_CLEAR:
			SendMessage(hEdit, WM_CLEAR,0,0);
			break;
		case IDM_SETTEXT:
			SendMessage(hEdit, WM_SETTEXT,0,(LPARAM)"SetText Message");
			break;
		case IDM_GETTEXT:
			SendMessage(hEdit, WM_GETTEXT,128,(LPARAM)text);
			SetWindowText(hWnd, text);
			break;
		}
		return 0;
	case WM_INITMENU:
		// 취소가 가능한가에 따라 Undo 메뉴 항목을 사용금지/허가한다.
		if (SendMessage(hEdit, EM_CANUNDO,0,0)==TRUE)
			EnableMenuItem(GetMenu(hWnd),IDM_UNDO,MF_ENABLED | MF_BYCOMMAND);
		else 
			EnableMenuItem(GetMenu(hWnd),IDM_UNDO,MF_GRAYED | MF_BYCOMMAND);

		// 선택 영역이 없으면 Cut, Copy, Clear 메뉴 항목은 사용할 수 없다.
		SendMessage(hEdit,EM_GETSEL, (WPARAM)&Start, (LPARAM)&End);
		if (Start != End) {
			EnableMenuItem(GetMenu(hWnd),IDM_CUT,MF_ENABLED | MF_BYCOMMAND);
			EnableMenuItem(GetMenu(hWnd),IDM_COPY,MF_ENABLED | MF_BYCOMMAND);
			EnableMenuItem(GetMenu(hWnd),IDM_CLEAR,MF_ENABLED | MF_BYCOMMAND);
		} else {
			EnableMenuItem(GetMenu(hWnd),IDM_CUT,MF_GRAYED | MF_BYCOMMAND);
			EnableMenuItem(GetMenu(hWnd),IDM_COPY,MF_GRAYED | MF_BYCOMMAND);
			EnableMenuItem(GetMenu(hWnd),IDM_CLEAR,MF_GRAYED | MF_BYCOMMAND);
		}

		// 클립보드에 텍스트 자료가 없으면 Paste 항목은 사용할 수 없다.
		if (IsClipboardFormatAvailable(CF_TEXT)) 
			EnableMenuItem(GetMenu(hWnd),IDM_PASTE,MF_ENABLED | MF_BYCOMMAND);
		else 
			EnableMenuItem(GetMenu(hWnd),IDM_PASTE,MF_GRAYED | MF_BYCOMMAND);

		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

