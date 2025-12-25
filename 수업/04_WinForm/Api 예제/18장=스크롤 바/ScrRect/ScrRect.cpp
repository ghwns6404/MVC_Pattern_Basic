#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ScrRect");

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

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW | WS_VSCROLL,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);

	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

int yPos;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int i;
	TCHAR str[128];
	int yInc;
	RECT crt,rt;

	switch (iMessage) {
	case WM_CREATE:
		CreateWindow(TEXT("button"),TEXT("버튼1"),WS_CHILD | WS_VISIBLE | 
			BS_PUSHBUTTON,20,20,100,25,hWnd,(HMENU)0,g_hInst,NULL);
		CreateWindow(TEXT("button"),TEXT("버튼2"),WS_CHILD | WS_VISIBLE | 
			BS_PUSHBUTTON,20,50,100,25,hWnd,(HMENU)1,g_hInst,NULL);
		SetScrollRange(hWnd, SB_VERT, 0, 100*20, TRUE);
		SetScrollPos(hWnd, SB_VERT, 0, TRUE);
		return 0;
	case WM_VSCROLL:
		yInc=0;
		switch (LOWORD(wParam)) {
		case SB_LINEUP:
			yInc=-20;
			break;
		case SB_LINEDOWN:
			yInc=20;
			break;
		case SB_PAGEUP:
			yInc=-200;
			break;
		case SB_PAGEDOWN:
			yInc=200;
			break;
		case SB_THUMBTRACK:
			yInc=HIWORD(wParam)-yPos;
			break;
		}
		if (yPos+yInc < 0)
			yInc=-yPos;
		if (yPos+yInc > 20*100)
			yInc=20*100-yPos;
		yPos=yPos+yInc;
		ScrollWindow(hWnd, 0, -yInc, NULL, NULL);
/*  버튼 영역은 제외
		GetClientRect(hWnd,&crt);
		SetRect(&rt,150,0,crt.right,crt.bottom);
		ScrollWindow(hWnd, 0, -yInc, &rt, NULL);
//*/
/*  일정 부분만 스크롤
		GetClientRect(hWnd,&crt);
		SetRect(&rt,150,100,crt.right,200);
		ScrollWindow(hWnd, 0, -yInc, &rt, NULL);
//*/
/*  클리핑 지정
		GetClientRect(hWnd,&crt);
		SetRect(&rt,150,100,crt.right,200);
		ScrollWindow(hWnd, 0, -yInc, &rt, &rt);
//*/
		SetScrollPos(hWnd, SB_VERT, yPos, TRUE);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		for (i=0;i<100;i++) {
			wsprintf(str,TEXT("Line Number : %d"),i);
			TextOut(hdc,150,i*20-yPos,str,lstrlen(str));
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
