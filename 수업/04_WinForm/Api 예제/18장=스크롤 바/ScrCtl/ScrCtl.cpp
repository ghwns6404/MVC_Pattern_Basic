#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ScrCtl");

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

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);

	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

HWND hScroll;
int Value;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR str[128];
	switch (iMessage) {
	case WM_CREATE:
		hScroll=CreateWindow(TEXT("scrollbar"),NULL,WS_CHILD | WS_VISIBLE | 
			SBS_HORZ,10,100,200,20,hWnd,(HMENU)0,g_hInst,NULL);
		Value=50;
		SetScrollRange(hScroll,SB_CTL,0,100,TRUE);
		SetScrollPos(hScroll,SB_CTL,Value,TRUE);
		return 0;
	case WM_HSCROLL:
		switch (LOWORD(wParam)) {
		case SB_LINELEFT:
			Value=max(0,Value-1);
			break;
		case SB_LINERIGHT:
			Value=min(100,Value+1);
			break;
		case SB_PAGELEFT:
			Value=max(0,Value-10);
			break;
		case SB_PAGERIGHT:
			Value=min(100,Value+10);
			break;
		case SB_THUMBTRACK:
			Value=HIWORD(wParam);
			break;
		}
		SetScrollPos((HWND)lParam,SB_CTL,Value,TRUE);
		InvalidateRect(hWnd,NULL,TRUE);
		return 0;
/* 보이기, 숨기기
	case WM_LBUTTONDOWN:
		ShowScrollBar(hScroll,SB_CTL,FALSE);
		return 0;
	case WM_RBUTTONDOWN:
		ShowScrollBar(hScroll,SB_CTL,TRUE);
		return 0;
//*/
//* 사용 금지, 허가
	case WM_LBUTTONDOWN:
		EnableScrollBar(hScroll,SB_CTL,ESB_DISABLE_LEFT);
		return 0;
	case WM_RBUTTONDOWN:
		EnableScrollBar(hScroll,SB_CTL,ESB_ENABLE_BOTH);
		return 0;
//*/
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		wsprintf(str,TEXT("Value is %d"),Value);
		TextOut(hdc,50,50,str,lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
