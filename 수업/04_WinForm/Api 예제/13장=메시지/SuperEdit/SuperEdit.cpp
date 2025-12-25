#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("SuperEditExam");

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

WNDPROC OldEditProc;
#define ID_EDIT1 100
HWND hEdit1;
LRESULT CALLBACK SuperProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage) {
	case WM_KEYDOWN:
		MessageBeep(0);
	}
	return CallWindowProc(OldEditProc,hWnd,iMessage,wParam,lParam);
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	WNDCLASS WndClass;
	TCHAR *Mes=TEXT("에디트에서 키보드를 누를때마다 소리를 낸다");
	switch (iMessage) {
	case WM_CREATE:
		GetClassInfo(NULL,TEXT("edit"),&WndClass);
		WndClass.hInstance=g_hInst;
		WndClass.lpszClassName=TEXT("SuperEdit");
		WndClass.hCursor=LoadCursor(NULL,IDC_WAIT);
		OldEditProc=WndClass.lpfnWndProc;
		WndClass.lpfnWndProc=SuperProc;
		RegisterClass(&WndClass);
		hEdit1=CreateWindow(TEXT("SuperEdit"),NULL,WS_CHILD | WS_VISIBLE | 
			WS_BORDER | ES_AUTOHSCROLL,
			10,10,200,25,hWnd,(HMENU)ID_EDIT1,g_hInst,NULL);
		SetFocus(hEdit1);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,10,100,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

