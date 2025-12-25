#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("Font");

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

//* CreateFont 함수를 사용하는 방법
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HFONT hFont, OldFont;
	TCHAR *str=TEXT("폰트 Test 1234");
	switch (iMessage) {
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		hFont=CreateFont(50,0,0,0,0,0,0,0,HANGEUL_CHARSET,0,0,0,
			VARIABLE_PITCH | FF_ROMAN,TEXT("궁서"));
		OldFont=(HFONT)SelectObject(hdc,hFont);
		TextOut(hdc, 100,100,str,lstrlen(str));
		SelectObject(hdc,OldFont);
		DeleteObject(hFont);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
//*/

/* LOGFONT 구조체를 사용하는 방법
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HFONT hFont, OldFont;
	TCHAR *str=TEXT("폰트 Test 1234");
	LOGFONT lf;
	switch (iMessage) {
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		lf.lfHeight=50;
		lf.lfWidth=0;
		lf.lfEscapement=0;
		lf.lfOrientation=0;
		lf.lfWeight=0;
		lf.lfItalic=0;
		lf.lfUnderline=0;
		lf.lfStrikeOut=0;
		lf.lfCharSet=HANGEUL_CHARSET;
		lf.lfOutPrecision=0;
		lf.lfClipPrecision=0;
		lf.lfQuality=0;
		lf.lfPitchAndFamily=VARIABLE_PITCH | FF_ROMAN;
		lstrcpy(lf.lfFaceName,TEXT("궁서"));
		hFont=CreateFontIndirect(&lf);
		OldFont=(HFONT)SelectObject(hdc,hFont);
		TextOut(hdc, 100,100,str,lstrlen(str));
		SelectObject(hdc,OldFont);
		DeleteObject(hFont);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
//*/