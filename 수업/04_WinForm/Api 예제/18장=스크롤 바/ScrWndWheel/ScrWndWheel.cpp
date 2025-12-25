#define _WIN32_WINNT 0x400
#define _WIN32_WINDOWS 0x401
#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ScrWndWheel");

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

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW | 
		WS_HSCROLL | WS_VSCROLL,
		CW_USEDEFAULT,CW_USEDEFAULT,640,480,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);

	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

int yPos,xPos;
int yMax,xMax;
int SumDelta;
int WheelUnit;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	int i;
	TCHAR str[128];
	HDC hdc;
	PAINTSTRUCT ps;
	int xInc, yInc;
	SCROLLINFO si;
	int Lines;
	int nScroll;
	
	switch (iMessage) {
	case WM_CREATE:
		SystemParametersInfo(SPI_GETWHEELSCROLLLINES,0,&Lines,0);
		WheelUnit=WHEEL_DELTA/Lines;
		xPos=0;
		yPos=0;
		xMax=1024;
		yMax=768;
		SetScrollRange(hWnd, SB_VERT, 0, yMax, TRUE);
		SetScrollPos(hWnd, SB_VERT, 0, TRUE);
		SetScrollRange(hWnd, SB_HORZ, 0, xMax, TRUE);
		SetScrollPos(hWnd, SB_HORZ, 0, TRUE);
		return 0;
	case WM_SIZE:
		si.cbSize=sizeof(SCROLLINFO);
		si.fMask=SIF_ALL | SIF_DISABLENOSCROLL;
		si.nMin=0;
		si.nMax=768;
		si.nPage=HIWORD(lParam);
		si.nPos=yPos;
		SetScrollInfo(hWnd, SB_VERT, &si, TRUE);

		si.nMax=1024;
		si.nPage=LOWORD(lParam);
		si.nPos=xPos;
		SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
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
		// 새로운 위치는 최소한 0 이상
		if (xPos+xInc < 0)
			xInc=-xPos;
		// 새로운 위치는 최대한 xMax 이하
		if (xPos+xInc > xMax)
			xInc=xMax-xPos;
		// 새로운 위치 계산
		xPos=xPos+xInc;
		// 스크롤시키고 썸 위치를 다시 계산한다.
		ScrollWindow(hWnd, -xInc, 0, NULL, NULL);
		SetScrollPos(hWnd, SB_HORZ, xPos, TRUE);
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
		//case SB_THUMBPOSITION:
			yInc=HIWORD(wParam)-yPos;
			break;
		}
		// 새로운 위치는 최소한 0 이상
		if (yPos+yInc < 0)
			yInc=-yPos;
		// 새로운 위치는 최대한 yMax 이하
		if (yPos+yInc > yMax)
			yInc=yMax-yPos;
		// yInc=max(-yPos, min(yInc, yMax-yPos));
		// 새로운 위치 계산
		yPos=yPos+yInc;
		// 스크롤시키고 썸 위치를 다시 계산한다.
		ScrollWindow(hWnd, 0, -yInc, NULL, NULL);
		SetScrollPos(hWnd, SB_VERT, yPos, TRUE);
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
		EndPaint(hWnd, &ps);
		return 0;
//* 원론적인 코드
	case WM_MOUSEWHEEL:
		nScroll=0;
		SumDelta += (short)HIWORD(wParam);
		while (abs(SumDelta) >= WheelUnit) {
			if (SumDelta > 0) {
				nScroll--;
				SumDelta-=WheelUnit;
			} else {
				nScroll++;
				SumDelta+=WheelUnit;
			}
		}
		while (nScroll != 0) {
			if (nScroll > 0) {
				SendMessage(hWnd,WM_VSCROLL,MAKELONG(SB_PAGEDOWN,0),0);
				nScroll--;
			} else {
				SendMessage(hWnd,WM_VSCROLL,MAKELONG(SB_PAGEUP,0),0);
				nScroll++;
			}
		}
		return 0;
//*/
/* 간단한 코드
	case WM_MOUSEWHEEL:
		SystemParametersInfo(SPI_GETWHEELSCROLLLINES,0,&Lines,0);
		for (i=0;i<Lines;i++) {
			if ((short)HIWORD(wParam) > 0) {
				SendMessage(hWnd,WM_VSCROLL,MAKELONG(SB_PAGEUP,0),0);
			} else {
				SendMessage(hWnd,WM_VSCROLL,MAKELONG(SB_PAGEDOWN,0),0);
			}
		}
		return 0;
//*/
/* 한꺼번에 스크롤하기
	case WM_MOUSEWHEEL:
		SystemParametersInfo(SPI_GETWHEELSCROLLLINES,0,&Lines,0);
		for (i=0;i<Lines;i++) {
			if ((short)HIWORD(wParam) > 0) {
				yInc=-Lines*20;
			} else {
				yInc=Lines*20;
			}
		}
		yInc=max(-yPos, min(yInc, yMax-yPos));
		yPos=yPos+yInc;
		// 스크롤시키고 썸 위치를 다시 계산한다.
		ScrollWindow(hWnd, 0, -yInc, NULL, NULL);
		SetScrollPos(hWnd, SB_VERT, yPos, TRUE);
		return 0;
//*/
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
