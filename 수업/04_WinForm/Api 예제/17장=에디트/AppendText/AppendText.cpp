#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("AppendText");

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

HWND hEdit, hEditSub;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR Text[128];
	int len;

	switch (iMessage) {
	case WM_CREATE:
		hEdit=CreateWindow(TEXT("edit"),NULL,
			WS_CHILD | WS_BORDER | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL,
			10,10,300,300,hWnd,(HMENU)100,g_hInst,NULL);
		hEditSub=CreateWindow(TEXT("edit"),NULL,WS_CHILD | WS_VISIBLE | WS_BORDER | 
			ES_AUTOVSCROLL,320,10,200,25,hWnd,(HMENU)101,g_hInst,NULL);
		CreateWindow(TEXT("button"),TEXT("Ãß°¡"),WS_CHILD | WS_VISIBLE | 
			BS_PUSHBUTTON,320,40,100,25,hWnd,(HMENU)102,g_hInst,NULL);		
		CreateWindow(TEXT("button"),TEXT("»ðÀÔ"),WS_CHILD | WS_VISIBLE | 
			BS_PUSHBUTTON,320,70,100,25,hWnd,(HMENU)103,g_hInst,NULL);		
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 102:
			len=GetWindowTextLength(hEdit);
			SendMessage(hEdit,EM_SETSEL,len,len);
		case 103:
			GetWindowText(hEditSub,Text,128);
			SendMessage(hEdit,EM_REPLACESEL,(WPARAM)TRUE,(LPARAM)Text);
			break;
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
