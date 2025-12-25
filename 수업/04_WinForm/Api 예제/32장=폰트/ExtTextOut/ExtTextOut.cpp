#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ExtTextOut");

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
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *str="ExtTextOut Test";
	RECT rt;
	int lpDx[]={5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};

	switch (iMessage) {
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);

		// TextOut과 동일
		ExtTextOut(hdc,10,10,0,NULL,str,lstrlen(str),NULL);

		// 개별 자간 지정
		ExtTextOut(hdc,10,30,0,NULL,str,lstrlen(str),lpDx);

		// 클리핑 영역 지정
		SetRect(&rt,15,50,90,60);
		ExtTextOut(hdc,10,50,ETO_CLIPPED,&rt,str,lstrlen(str),NULL);

		// 불투명 영역 지정
		SetBkColor(hdc,RGB(255,0,0));
		SetBkMode(hdc,TRANSPARENT);
		SetRect(&rt,15,70,90,80);
		ExtTextOut(hdc,10,70,ETO_OPAQUE,&rt,str,lstrlen(str),NULL);

		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
