#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("Status1");

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
	hWndMain=hWnd;
	
	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

#include <commctrl.h>
HWND hState;

void SetStatusText(HWND hWnd)
{
	RECT wrt;
	TCHAR str[128];

	GetWindowRect(hWnd, &wrt);
	wsprintf(str, "Left : %d", wrt.left);
	SendMessage(hState, SB_SETTEXT, 0, (LPARAM)str);
	wsprintf(str, "Top : %d", wrt.top);
	SendMessage(hState, SB_SETTEXT, 1, (LPARAM)str);
	wsprintf(str, "Right : %d", wrt.right);
	SendMessage(hState, SB_SETTEXT, 2, (LPARAM)str);
	wsprintf(str, "Bottom : %d", wrt.bottom);
	SendMessage(hState, SB_SETTEXT, 3, (LPARAM)str);
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	int SBPart[4],i;

	switch (iMessage) {
	case WM_CREATE:
		InitCommonControls();
		hState=CreateStatusWindow(WS_CHILD | WS_VISIBLE, "", hWnd,0);
		return 0;
	case WM_SIZE:
		SendMessage(hState, WM_SIZE, wParam, lParam);

		for (i=0;i<4;i++) {
			SBPart[i]=LOWORD(lParam)/4*(i+1);
		}
		SendMessage(hState, SB_SETPARTS, 4, (LPARAM)SBPart);

		SetStatusText(hWnd);
		return 0;
	case WM_MOVE:
		SetStatusText(hWnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
