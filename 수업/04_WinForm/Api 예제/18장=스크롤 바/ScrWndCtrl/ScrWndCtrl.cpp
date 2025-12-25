#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ScrWndCtrl");

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
		CW_USEDEFAULT,CW_USEDEFAULT,640,480,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);

	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

// 전역변수 및 매크로 상수
HWND hHorzScroll;			// 수평 스크롤 바 컨트롤
HWND hVertScroll;			// 수직 스크롤 바 컨트롤
int yPos,xPos;				// 현재 위치
int yMax,xMax;				// 최대 크기
int SW;						// 스크롤 바의 높이

// 수평 스크롤 바 왼쪽에 현재 위치를 출력하는 함수
// 스크롤 바 영역은 무효영역에서 제외되기 때문에 WM_PAINT에서 
// 현재 위치를 출력할 수 없어서 별도의 함수로 분리하였다.
void PrintPosition()
{
	HDC hdc;
	TCHAR str[128];
	RECT crt;
	hdc=GetDC(hWndMain);

	GetClientRect(hWndMain,&crt);
	SelectObject(hdc,GetStockObject(BLACK_BRUSH));
	Rectangle(hdc,0,crt.bottom-SW,200,crt.bottom);
	SetTextColor(hdc,RGB(255,255,255));
	SetBkColor(hdc,RGB(0,0,0));
	wsprintf(str,TEXT("현재 위치 => X:%d, Y:%d"),xPos,yPos);
	TextOut(hdc,5,crt.bottom-SW,str,lstrlen(str));
	ReleaseDC(hWndMain,hdc);
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	int i;
	TCHAR str[128];
	HDC hdc;
	PAINTSTRUCT ps;
	int xInc, yInc;
	SCROLLINFO si;
	RECT crt,rt;
	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		// 두 개의 스크롤 바 컨트롤을 만든다.
		hHorzScroll=CreateWindow(TEXT("scrollbar"),NULL,WS_CHILD | WS_VISIBLE | 
			SBS_HORZ,0,0,0,0,hWnd,(HMENU)0,g_hInst,NULL);
		hVertScroll=CreateWindow(TEXT("scrollbar"),NULL,WS_CHILD | WS_VISIBLE | 
			SBS_VERT,0,0,0,0,hWnd,(HMENU)1,g_hInst,NULL);
		xPos=0;
		yPos=0;
		xMax=1024;
		yMax=768;
		// 스크롤 바의 높이를 구해 놓는다.
		SW=GetSystemMetrics(SM_CYHSCROLL);
		return 0;
	case WM_SIZE:
		si.cbSize=sizeof(SCROLLINFO);
		si.fMask=SIF_ALL | SIF_DISABLENOSCROLL;
		si.nMin=0;
		si.nMax=768;
		si.nPage=HIWORD(lParam)-SW;
		si.nPos=yPos;
		SetScrollInfo(hVertScroll, SB_CTL, &si, TRUE);

		si.nMax=1024;
		si.nPage=LOWORD(lParam)-SW;
		si.nPos=xPos;
		SetScrollInfo(hHorzScroll, SB_CTL, &si, TRUE);

		// 스크롤 바가 별도의 컨트롤이므로 메인 윈도우의 크기가 변경
		// 되면 스크롤 바의 위치도 따라서 변경되어야 한다.
		MoveWindow(hHorzScroll,200,HIWORD(lParam)-SW,
			LOWORD(lParam)-SW-200,SW,TRUE);
		MoveWindow(hVertScroll,LOWORD(lParam)-SW,0,SW,
			HIWORD(lParam)-SW,TRUE);
		return 0;
	case WM_HSCROLL:
		xInc=0;
		switch (LOWORD(wParam)) {
		case SB_LINELEFT:
			xInc=-1;
			break;
		case SB_LINERIGHT:
			xInc=1;
			break;
		case SB_PAGELEFT:
			xInc=-20;
			break;
		case SB_PAGERIGHT:
			xInc=20;
			break;
		case SB_THUMBTRACK:
			xInc=HIWORD(wParam)-xPos;
			break;
		}
		if (xPos+xInc < 0)
			xInc=-xPos;
		if (xPos+xInc > xMax)
			xInc=xMax-xPos;
		xPos=xPos+xInc;
		// 스크롤 바 영역을 제외한 부분만 스크롤시킨다.
		GetClientRect(hWnd,&crt);
		SetRect(&rt,0,0,crt.right-SW,crt.bottom-SW);
		ScrollWindow(hWnd, -xInc, 0, &rt, &rt);
		SetScrollPos(hHorzScroll, SB_CTL, xPos, TRUE);
		// 스크롤한 후 현재 위치를 출력한다.
		PrintPosition();
		return 0;
	case WM_VSCROLL:
		yInc=0;
		switch (LOWORD(wParam)) {
		case SB_LINEUP:
			yInc=-1;
			break;
		case SB_LINEDOWN:
			yInc=1;
			break;
		case SB_PAGEUP:
			yInc=-20;
			break;
		case SB_PAGEDOWN:
			yInc=20;
			break;
		case SB_THUMBTRACK:
			yInc=HIWORD(wParam)-yPos;
			break;
		}
		if (yPos+yInc < 0)
			yInc=-yPos;
		if (yPos+yInc > yMax)
			yInc=yMax-yPos;
		yPos=yPos+yInc;
		// 스크롤 바 영역을 제외한 부분만 스크롤시킨다.
		GetClientRect(hWnd,&crt);
		SetRect(&rt,0,0,crt.right-SW,crt.bottom-SW);
		ScrollWindow(hWnd, 0, -yInc, &rt, &rt);
		SetScrollPos(hVertScroll, SB_CTL, yPos, TRUE);
		// 스크롤한 후 현재 위치를 출력한다.
		PrintPosition();
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		for (i=0;i<1024;i+=50) {
			wsprintf(str,TEXT("%d"),i);
			TextOut(hdc,i-xPos,0-yPos,str,lstrlen(str));
			TextOut(hdc,i-xPos,750-yPos,str,lstrlen(str));
		}
		for (i=0;i<768;i+=50) {
			wsprintf(str,TEXT("%d"),i);
			TextOut(hdc,0-xPos,i-yPos,str,lstrlen(str));
			TextOut(hdc,1000-xPos,i-yPos,str,lstrlen(str));
		}
		Ellipse(hdc,300-xPos,280-yPos,700-xPos,480-yPos);
		MoveToEx(hdc,500-xPos,100-yPos,NULL);
		LineTo(hdc,500-xPos,668-yPos);
		MoveToEx(hdc,100-xPos,380-yPos,NULL);
		LineTo(hdc,924-xPos,380-yPos);
		PrintPosition();
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
