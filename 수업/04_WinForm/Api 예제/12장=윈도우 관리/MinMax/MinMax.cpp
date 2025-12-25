#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("MinMax");

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

	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

//*
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	LPMINMAXINFO pmmi;
	switch (iMessage) {
	case WM_GETMINMAXINFO:
		pmmi=(LPMINMAXINFO)lParam;
		pmmi->ptMinTrackSize.x = 300;
		pmmi->ptMinTrackSize.y = 200;
		pmmi->ptMaxTrackSize.x = 600;
		pmmi->ptMaxTrackSize.y = 400;

		pmmi->ptMaxSize.x = 500;
		pmmi->ptMaxSize.y = 300;
		pmmi->ptMaxPosition.x = 100;
		pmmi->ptMaxPosition.y = 100;
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
//*/

/*
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	RECT wrt;
	int Width,Height;

	switch (iMessage) {
	case WM_SIZE:
		GetWindowRect(hWnd,&wrt);
		Width=wrt.right-wrt.left;
		Height=wrt.bottom-wrt.top;
		Width=min(max(Width,300),600);
		Height=min(max(Height,200),400);
		SetWindowPos(hWnd,NULL,0,0,Width,Height,SWP_NOMOVE);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
//*/