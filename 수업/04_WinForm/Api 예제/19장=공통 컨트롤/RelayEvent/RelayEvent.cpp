#include <windows.h>
#include <commctrl.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("RelayEvent");

static HWND hTip, hBtn;
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
	
	// 메시지 루프에서 툴팁으로도 메시지를 보내 준다.
	while (GetMessage(&Message,NULL,0,0)) {
		if (Message.message >= WM_MOUSEFIRST && Message.message <= WM_MOUSELAST)
			SendMessage(hTip,TTM_RELAYEVENT,0,(LPARAM)&Message);
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	INITCOMMONCONTROLSEX iccex;
	RECT rt={300,50,500,150};
	TOOLINFO ti;

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
		ti.uFlags=TTF_IDISHWND;
		ti.hwnd=hWnd;
		ti.uId=(WPARAM)hBtn;
		ti.lpszText="버튼입니다";
		SendMessage(hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti);

		ti.uFlags=0;
		ti.hwnd=hWnd;
		ti.uId=0;
		ti.lpszText="사각영역입니다";
		ti.rect=rt;
		SendMessage(hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti);

		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		Rectangle(hdc,rt.left,rt.top,rt.right,rt.bottom);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
