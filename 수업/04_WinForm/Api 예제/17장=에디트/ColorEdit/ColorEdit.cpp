#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ColorEdit");

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

#define ID_EDIT 100
HWND hEdit;
HBRUSH hBrushEdit;
HFONT hFontEdit;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage) {
	case WM_CREATE:
		hEdit=CreateWindow(TEXT("edit"),NULL,WS_CHILD | WS_VISIBLE | WS_BORDER |
			ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
			10,10,400,400,hWnd,(HMENU)ID_EDIT,g_hInst,NULL);
		hBrushEdit=CreateSolidBrush(RGB(255,0,0));
		hFontEdit=CreateFont(30,0,0,0,0,0,0,0,HANGEUL_CHARSET,
			3,2,1,VARIABLE_PITCH | FF_ROMAN,TEXT("±Ã¼­"));
		SendMessage(hEdit, WM_SETFONT, (WPARAM)hFontEdit, MAKELPARAM(FALSE,0));
		return 0;
	case WM_CTLCOLOREDIT:
		if ((HWND)lParam == hEdit) {
			SetTextColor((HDC)wParam, RGB(255,255,0));
			SetBkColor((HDC)wParam, RGB(255,0,0));
			return (LRESULT)hBrushEdit;
		}
		break;
	case WM_DESTROY:
		DeleteObject(hBrushEdit);
		DeleteObject(hFontEdit);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
