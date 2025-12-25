#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("CreateMenu");

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

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HMENU hMenu,hPopup;
	TCHAR *Mes="실행중에 메뉴를 만드는 예제입니다";
	switch (iMessage) {
	case WM_CREATE:
		hMenu=CreateMenu();
		hPopup=CreatePopupMenu();
		AppendMenu(hMenu,MF_STRING | MF_POPUP, (UINT)hPopup, "Menu");
		AppendMenu(hPopup,MF_STRING,40001,"Item1");
		AppendMenu(hPopup,MF_STRING,40002,"Item2");
		AppendMenu(hPopup,MF_STRING,40003,"Item3");
		SetMenu(hWnd,hMenu);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 40001:
			MessageBox(hWnd,"Item1을 선택했습니다.","메뉴",MB_OK);
			break;
		case 40002:
			MessageBox(hWnd,"Item2를 선택했습니다.","메뉴",MB_OK);
			break;
		case 40003:
			MessageBox(hWnd,"Item3을 선택했습니다.","메뉴",MB_OK);
			break;
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,100,100,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

