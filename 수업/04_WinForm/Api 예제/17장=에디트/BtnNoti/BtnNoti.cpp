#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("BtnNoti");

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

#define ID_BTN 100
#define ID_LISTBOX 101
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	static HWND hList;
	switch (iMessage) {
	case WM_CREATE:
		CreateWindow(TEXT("button"),TEXT("Click Me"),WS_CHILD | WS_VISIBLE 
			| BS_PUSHBUTTON | BS_NOTIFY,
			20,20,100,25,hWnd,(HMENU)ID_BTN,g_hInst,NULL);
		hList=CreateWindow(TEXT("listbox"),TEXT("Click Me"),WS_CHILD | WS_VISIBLE 
			| WS_BORDER,
			20,50,300,500,hWnd,(HMENU)ID_LISTBOX,g_hInst,NULL);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_BTN:
			switch (HIWORD(wParam)) {
			case BN_CLICKED:
				SendMessage(hList,LB_ADDSTRING,0,(LPARAM)"BN_CLICKED");
				break;
			case BN_DISABLE:
				SendMessage(hList,LB_ADDSTRING,0,(LPARAM)"BN_DISABLE");
				break;
			case BN_DOUBLECLICKED:
				SendMessage(hList,LB_ADDSTRING,0,(LPARAM)"BN_DOUBLECLICKED");
				break;
			case BN_SETFOCUS:
				SendMessage(hList,LB_ADDSTRING,0,(LPARAM)"BN_SETFOCUS");
				break;
			case BN_KILLFOCUS:
				SendMessage(hList,LB_ADDSTRING,0,(LPARAM)"BN_KILLFOCUS");
				break;
			}
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
