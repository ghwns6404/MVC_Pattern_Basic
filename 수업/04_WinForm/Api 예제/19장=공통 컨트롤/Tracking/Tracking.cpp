#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("Tracking");

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
HWND hTip, hBtn;
BOOL gTrackOn=TRUE;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	INITCOMMONCONTROLSEX iccex;
	TOOLINFO ti;
	POINT pt;
	TCHAR *Mes="¸¶¿ì½º ¿ÞÂÊ ¹öÆ° = ÅøÆÁ º¸ÀÓ/¼û±è";

	switch (iMessage) {
	case WM_CREATE:
		iccex.dwICC=ICC_WIN95_CLASSES;
		iccex.dwSize=sizeof(INITCOMMONCONTROLSEX);
		InitCommonControlsEx(&iccex);

		hTip=CreateWindowEx(WS_EX_TOPMOST,TOOLTIPS_CLASS,NULL,0,
			CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
			hWnd,NULL,g_hInst,NULL);

		ti.cbSize=sizeof(TOOLINFO);
		ti.uFlags=TTF_IDISHWND | TTF_TRACK | TTF_ABSOLUTE;
		ti.hwnd=hWnd;
		ti.uId=(WPARAM)hWnd;
		ti.lpszText="¸¶¿ì½º¸¦ µû¶ó ´Ù´Ï´Â ÅøÆÁ";
		SendMessage(hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti);
		SendMessage(hTip, TTM_TRACKACTIVATE,(WPARAM)TRUE,(LPARAM)&ti);
		return 0;
	case WM_MOUSEMOVE:
		if (gTrackOn) {
			GetCursorPos(&pt);
			SendMessage(hTip,TTM_TRACKPOSITION,0,(LPARAM)MAKELPARAM(pt.x,pt.y+25));
		}
		return 0;
	case WM_LBUTTONDOWN:
		gTrackOn=!gTrackOn;
		ti.cbSize=sizeof(TOOLINFO);
		ti.uFlags=TTF_IDISHWND;
		ti.hwnd=hWnd;
		ti.uId=(WPARAM)hWnd;
		SendMessage(hTip, TTM_TRACKACTIVATE,(WPARAM)gTrackOn,(LPARAM)&ti);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,10,10,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
