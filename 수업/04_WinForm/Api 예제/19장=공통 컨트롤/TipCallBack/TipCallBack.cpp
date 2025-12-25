#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("TipCallBack");

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
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	INITCOMMONCONTROLSEX iccex;
	static HWND hTip, hBtn;
	RECT rt1={300,50,500,150};
	RECT rt2={550,50,750,150};
	TOOLINFO ti;
	SYSTEMTIME st;
	TCHAR sTime[128];
	LPNMHDR pNmHdr;
	LPNMTTDISPINFO di;

	switch (iMessage) {
	case WM_CREATE:
		iccex.dwICC=ICC_WIN95_CLASSES;
		iccex.dwSize=sizeof(INITCOMMONCONTROLSEX);
		InitCommonControlsEx(&iccex);

		hTip=CreateWindowEx(WS_EX_TOPMOST,TOOLTIPS_CLASS,NULL,0,
			CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
			hWnd,NULL,g_hInst,NULL);

		hBtn=CreateWindow("button","버튼",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			50,50,200,100,hWnd,(HMENU)0,g_hInst,NULL);

		ti.cbSize=sizeof(TOOLINFO);
		ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
		ti.hwnd=hWnd;
		ti.uId=(WPARAM)hBtn;
		ti.lpszText="empty";
		SendMessage(hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti);

		ti.uFlags=TTF_SUBCLASS;
		ti.hwnd=hWnd;
		ti.uId=0;
		ti.lpszText="empty";
		ti.rect=rt1;
		SendMessage(hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti);

		ti.uFlags=TTF_SUBCLASS;
		ti.hwnd=hWnd;
		ti.uId=1;
		ti.lpszText=LPSTR_TEXTCALLBACK;
		ti.rect=rt2;
		SendMessage(hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti);

		SetTimer(hWnd,0,1000,NULL);
		return 0;
	case WM_TIMER:
		GetLocalTime(&st);
		wsprintf(sTime,"현재시간 = %d:%d:%d",st.wHour,st.wMinute,st.wSecond);
		ti.cbSize=sizeof(TOOLINFO);
		ti.hwnd=hWnd;
		ti.uFlags=TTF_IDISHWND;
		ti.uId=(WPARAM)hBtn;
		ti.hinst=NULL;
		ti.lpszText=sTime;
		SendMessage(hTip,TTM_UPDATETIPTEXT,0,(LPARAM)&ti);
		return 0;
	case WM_NOTIFY:
		pNmHdr=(LPNMHDR)lParam;
		if (pNmHdr->hwndFrom == hTip) {
			switch (pNmHdr->code) {
			case TTN_SHOW:
				GetLocalTime(&st);
				wsprintf(sTime,"%d월 %d일 %d:%d:%d",st.wMonth, st.wDay,
					st.wHour,st.wMinute,st.wSecond);
				ti.cbSize=sizeof(TOOLINFO);
				ti.hwnd=hWnd;
				ti.uFlags=0;
				ti.uId=0;
				ti.hinst=NULL;
				ti.lpszText=sTime;
				SendMessage(hTip,TTM_UPDATETIPTEXT,0,(LPARAM)&ti);
				break;
			case TTN_GETDISPINFO:
				di=(LPNMTTDISPINFO)lParam;
				GetLocalTime(&st);
				wsprintf(sTime,"%d:%d:%d",st.wHour,st.wMinute,st.wSecond);
				// lstrcpy(di->szText,sTime);
				di->lpszText=sTime;
				di->hinst=NULL;
				break;
			}
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		Rectangle(hdc,rt1.left,rt1.top,rt1.right,rt1.bottom);
		SelectObject(hdc,GetStockObject(LTGRAY_BRUSH));
		Rectangle(hdc,rt2.left,rt2.top,rt2.right,rt2.bottom);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd,0);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
