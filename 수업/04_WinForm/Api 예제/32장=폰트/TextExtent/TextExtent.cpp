#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("TextExtent");

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

TCHAR *str[3] = {"Times New Roman", "Arial Black-Italic", "Wide Latin"};
COLORREF col[3]={RGB(255,0,0), RGB(0,255,0), RGB(0,0,255)};
HFONT hFont[3];
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	int x;
	SIZE sz;
	int i;
	switch (iMessage) {
	case WM_CREATE:
		hFont[0]=CreateFont(20,0,0,0,0,0,0,0,ANSI_CHARSET,3,2,1,
			VARIABLE_PITCH | FF_ROMAN,"Times New Roman");
		hFont[1]=CreateFont(25,0,0,0,0,TRUE,0,0,ANSI_CHARSET,3,2,1,
			VARIABLE_PITCH | FF_SWISS,"Arial Black");
		hFont[2]=CreateFont(30,0,0,0,0,0,0,0,ANSI_CHARSET,3,2,1,
			VARIABLE_PITCH | FF_ROMAN,"Wide Latin");
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		x=0;
		SetTextAlign(hdc, TA_BASELINE | TA_LEFT);

		for (i=0;i<3;i++) {
			SelectObject(hdc, hFont[i]);
			SetTextColor(hdc, col[i]);
			TextOut(hdc,x,50,str[i],lstrlen(str[i]));
			GetTextExtentPoint32(hdc,str[i],lstrlen(str[i]),&sz);
			x+=sz.cx;
		}

		SelectObject(hdc, GetStockObject(SYSTEM_FONT));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		DeleteObject(hFont[0]);
		DeleteObject(hFont[1]);
		DeleteObject(hFont[2]);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

