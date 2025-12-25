#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("StockFont");

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

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR str[128];
	switch (iMessage) {
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		lstrcpy(str, "Korea is my Home Contury, 1234!@#$ iWiWiWiW 나 너 대한민국");
		SelectObject(hdc,GetStockObject(ANSI_FIXED_FONT));
		TextOut(hdc,50,50,str,lstrlen(str));
		SelectObject(hdc,GetStockObject(ANSI_VAR_FONT));
		TextOut(hdc,50,100,str,lstrlen(str));
		SelectObject(hdc,GetStockObject(DEVICE_DEFAULT_FONT));
		TextOut(hdc,50,150,str,lstrlen(str));
		SelectObject(hdc,GetStockObject(OEM_FIXED_FONT));
		TextOut(hdc,50,200,str,lstrlen(str));
		SelectObject(hdc,GetStockObject(SYSTEM_FONT));
		TextOut(hdc,50,250,str,lstrlen(str));
		SelectObject(hdc,GetStockObject(SYSTEM_FIXED_FONT));
		TextOut(hdc,50,300,str,lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

