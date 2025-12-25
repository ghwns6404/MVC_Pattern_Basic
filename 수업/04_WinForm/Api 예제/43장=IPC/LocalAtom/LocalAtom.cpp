#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("LocalAtom");

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
	static ATOM AtStr, AtX, AtY;
	TCHAR str[128];
	TCHAR temp[128];
	int x, y;

	switch (iMessage) {
	case WM_CREATE:
		AtStr=AddAtom("Atom Test String");
		AtX=AddAtom(MAKEINTATOM(123));
		AtY=AddAtom(MAKEINTATOM(50));
		break;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		GetAtomName(AtStr, str, 128);
		GetAtomName(AtX, temp, 128);		// str="#123" 으로 읽혀짐
		x=atoi(temp+1);						// 앞의 #은 제외한다.
		GetAtomName(AtY, temp, 128);
		y=atoi(temp+1);
		TextOut(hdc, x,y, str, lstrlen(str));
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		DeleteAtom(AtStr);
		DeleteAtom(AtX);
		DeleteAtom(AtY);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

