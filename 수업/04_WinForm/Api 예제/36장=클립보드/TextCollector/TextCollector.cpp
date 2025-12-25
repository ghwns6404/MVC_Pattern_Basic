#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("TextCollector");

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

HWND hNext;
HWND hEdit;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes="클립보드의 텍스트를 수집합니다.";
	HGLOBAL hmem;
	TCHAR *ptr;
	int len;

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		hEdit=CreateWindowEx(WS_EX_CLIENTEDGE,"edit",NULL,WS_CHILD | 
			WS_VISIBLE | WS_BORDER | ES_MULTILINE | WS_VSCROLL,
			0,0,0,0,hWnd,(HMENU)0,g_hInst,NULL);
		hNext=SetClipboardViewer(hWnd);
		return 0;
	case WM_SIZE:
		if (wParam != SIZE_MINIMIZED) {
			MoveWindow(hEdit,0,50,LOWORD(lParam),HIWORD(lParam)-50,TRUE);
		}
		return 0;
	case WM_CHANGECBCHAIN:
		if ((HWND)wParam==hNext) {
			hNext=(HWND)lParam;
		}
		if (hNext) {
			SendMessage(hNext,WM_CHANGECBCHAIN,wParam,lParam);
		}
		return 0;
	case WM_DRAWCLIPBOARD:
		if (hNext) {
			SendMessage(hNext,WM_DRAWCLIPBOARD,wParam,lParam);
		}
		if (!IsClipboardFormatAvailable(CF_TEXT)) return 0;
		if (!IsWindowVisible(hWnd)) return 0;
		OpenClipboard(hWnd);
		hmem=GetClipboardData(CF_TEXT);
		ptr=(TCHAR *)GlobalLock(hmem);
		len=GetWindowTextLength(hEdit);
		SendMessage(hEdit,EM_SETSEL,len,len);
		SendMessage(hEdit,EM_REPLACESEL,(WPARAM)TRUE,(LPARAM)ptr);
		SendMessage(hEdit,EM_REPLACESEL,(WPARAM)TRUE,(LPARAM)"\r\n");
		GlobalUnlock(hmem);
		CloseClipboard();
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		SetBkMode(hdc,TRANSPARENT);
		TextOut(hdc,5,5,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		ChangeClipboardChain(hWnd,hNext);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
