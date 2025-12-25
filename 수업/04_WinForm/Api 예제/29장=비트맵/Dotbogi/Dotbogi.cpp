#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("Dotbogi");

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
	WndClass.hIcon=LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,400,300,
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
	HDC hdc;				// 화면 DC
	POINT pt;				// 마우스 위치
	static POINT oldpt;		// 이전 마우스 위치
	static int ratio=4;		// 확대 배율
	RECT drt;				// 작업 영역의 크기
	POINT spt;				// 캡처할 부분의 좌상단 좌표
	int ScrX, ScrY;			// 화면의 크기
	int sWidth, sHeight;	// 캡처할 부분의 폭과 높이
	int i;
	HMENU hPopup;

	switch (iMessage) {
	// 타이머를 설치한다.
	case WM_CREATE:
		hWndMain=hWnd;
		SetTimer(hWnd,0,200,NULL);
		return 0;
	case WM_TIMER:
		// 화면 크기를 구한다. 해상도가 바뀔 수 있으므로 매번 조사한다.
		ScrX=GetSystemMetrics(SM_CXSCREEN);
		ScrY=GetSystemMetrics(SM_CYSCREEN);
		GetClientRect(hWnd,&drt);
		
		// 커서가 있는 위치를 구한후 클라이언트 좌표로 변환한다.
		// 마우스 커서가 움직이지 않았으면 다시 그리지 않는다.
		GetCursorPos(&pt);
		if (oldpt.x == pt.x && oldpt.y == pt.y)
			return 0;
		oldpt.x=pt.x;
		oldpt.y=pt.y;
		ScreenToClient(hWnd,&pt);

		hdc=GetDC(hWnd);

		// 캡처할 소스 영역의 폭과 높이를 구한다.
		sWidth=drt.right/ratio;
		sHeight=drt.bottom/ratio;

		// 캡처할 소스 영역의 좌상단 좌표를 구한다. 마우스 위치가 중앙이 
		// 되도록 하기 위해 폭,높이 절반만큼 좌상으로 이동시킨다.
		spt.x=pt.x-sWidth/2;
		spt.y=pt.y-sHeight/2;

		// 화면 영역을 벗어나지 않도록 한다.
		ClientToScreen(hWnd,&spt);
		if (spt.x < 0) spt.x=0;
		if (spt.y < 0) spt.y=0;
		if (spt.x + sWidth > ScrX) spt.x=ScrX-sWidth;
		if (spt.y + sHeight > ScrY) spt.y=ScrY-sHeight;
		ScreenToClient(hWnd,&spt);

		// 커서 위치의 화면을 작업 영역으로 확대 복사한다.
		StretchBlt(hdc,0,0,drt.right,drt.bottom,
			hdc,spt.x,spt.y,sWidth,sHeight,SRCCOPY);
		ReleaseDC(hWnd, hdc);
		return 0;
	// 메뉴 항목이 선택되면 확대 비율을 조정한다.
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_MENU_1X:
			ratio=1;
			break;
		case ID_MENU_2X:
			ratio=2;
			break;
		case ID_MENU_3X:
			ratio=3;
			break;
		case ID_MENU_4X:
			ratio=4;
			break;
		case ID_MENU_5X:
			ratio=5;
			break;
		case ID_MENU_10X:
			ratio=10;
			break;
		case ID_MENU_HELP:
			MessageBox(hWnd, 
				"커서가 있는 화면의 일부분을 확대해서 보여줍니다.\n"
				"메뉴를 사용해서 확대 배율을 조정할 수 있습니다.\n"
				"김 상형 만듬", "프로그램 소개", MB_OK);
			break;
		case ID_MENU_EXIT:
			DestroyWindow(hWnd);
			break;
		}
		return 0;
	case WM_INITMENU:
		hPopup=GetSubMenu((HMENU)wParam,0);
		for (i=0;i<6;i++) {
			CheckMenuItem(hPopup,i,MF_BYPOSITION | MF_UNCHECKED);
		}
		CheckMenuItem(hPopup,ratio==10 ? 5:ratio-1,MF_BYPOSITION | MF_CHECKED);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd,0);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

