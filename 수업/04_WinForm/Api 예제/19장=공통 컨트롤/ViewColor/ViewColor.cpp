#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ViewColor");

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;
	
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
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

struct tag_Tools {
	int type;			// 유형 0=사각, 1=원
	COLORREF Col;		// 색상
	RECT rt;			// 영역
	int xoff, yoff;		// 툴팁 위치
	TCHAR *szTip;		// 툴 텍스트
} Tools[]={
	{0,RGB(255,0,0),{10,10,100,100},10,10,"빨간색"},
	{1,RGB(255,255,0),{150,10,300,140},50,50,"노란색"},
	{0,RGB(0,255,0),{10,150,200,200},20,20,"초록색"},
	{0,RGB(0,0,255),{10,220,300,350},120,50,"파란색"}
};

#include <commctrl.h>
HWND hTip, hBtn;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	INITCOMMONCONTROLSEX iccex;
	TOOLINFO ti;
	COLORREF Col;
	static COLORREF OldCol=0xffffffff;
	HBRUSH Brush,OldBrush;
	int i;
	POINT pt;

	switch (iMessage) {
	case WM_CREATE:
		iccex.dwICC=ICC_WIN95_CLASSES;
		iccex.dwSize=sizeof(INITCOMMONCONTROLSEX);
		InitCommonControlsEx(&iccex);

		hTip=CreateWindowEx(WS_EX_TOPMOST,TOOLTIPS_CLASS,NULL,0,
			CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
			hWnd,NULL,g_hInst,NULL);

		ti.cbSize=sizeof(TOOLINFO);
		ti.uFlags=TTF_TRACK | TTF_ABSOLUTE;
		ti.hwnd=hWnd;
		ti.uId=0;
		ti.lpszText="dummy";
		SetRect(&ti.rect,0,0,0,0);
		SendMessage(hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti);
		return 0;
	case WM_MOUSEMOVE:
		hdc=GetDC(hWnd);
		Col=GetPixel(hdc,LOWORD(lParam),HIWORD(lParam));
		if (Col != OldCol) {
			OldCol=Col;
			ti.cbSize=sizeof(TOOLINFO);
			ti.uFlags=0;
			ti.hwnd=hWnd;
			ti.uId=0;
			if (Col != RGB(255,255,255) && Col != RGB(0,0,0)) {
				for (i=0;i<sizeof(Tools)/sizeof(Tools[0]);i++) {
					if (Col == 	Tools[i].Col) {
						ti.lpszText=Tools[i].szTip;
						SendMessage(hTip,TTM_UPDATETIPTEXT,0,(LPARAM)&ti);
						pt.x=Tools[i].rt.left+Tools[i].xoff;
						pt.y=Tools[i].rt.top+Tools[i].yoff;
						ClientToScreen(hWnd,&pt);
						SendMessage(hTip,TTM_TRACKPOSITION,0,(LPARAM)MAKELPARAM(pt.x,pt.y));
						SendMessage(hTip, TTM_TRACKACTIVATE,(WPARAM)TRUE,(LPARAM)&ti);
						break;
					}
				}
			} else {
				SendMessage(hTip, TTM_TRACKACTIVATE,(WPARAM)FALSE,(LPARAM)&ti);
			}
		}
		ReleaseDC(hWnd,hdc);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		for (i=0;i<sizeof(Tools)/sizeof(Tools[0]);i++) {
			Brush=CreateSolidBrush(Tools[i].Col);
			OldBrush=(HBRUSH)SelectObject(hdc,Brush);
			if (Tools[i].type == 0) {
				Rectangle(hdc,Tools[i].rt.left,Tools[i].rt.top,
					Tools[i].rt.right,Tools[i].rt.bottom);
			} else {
				Ellipse(hdc,Tools[i].rt.left,Tools[i].rt.top,
					Tools[i].rt.right,Tools[i].rt.bottom);
			}
			SelectObject(hdc,OldBrush);
			DeleteObject(Brush);
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_ACTIVATEAPP:
		if (wParam == FALSE) {
			ti.cbSize=sizeof(TOOLINFO);
			ti.uFlags=0;
			ti.hwnd=hWnd;
			ti.uId=0;
			SendMessage(hTip, TTM_TRACKACTIVATE,(WPARAM)FALSE,(LPARAM)&ti);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
