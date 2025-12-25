#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("MkFont");

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
	HFONT font, oldfont;
	TCHAR *str="폰트 Test 1234";
	switch (iMessage) {
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
//* 궁서 50픽셀 크기의 폰트
		font=CreateFont(50,0,0,0,0,0,0,0,HANGEUL_CHARSET,3,2,1,
			VARIABLE_PITCH | FF_ROMAN,"궁서");
//*/
/* 기울어진 폰트
		font=CreateFont(50,0,200,0,0,0,0,0,HANGUL_CHARSET,3,2,1,
			VARIABLE_PITCH | FF_ROMAN,"궁서");
//*/
/* 개별 문자가 기울어진 폰트
		SetGraphicsMode(hdc,GM_ADVANCED);
		font=CreateFont(50,0,900,900,0,0,0,0,HANGUL_CHARSET,3,2,1,
			VARIABLE_PITCH | FF_ROMAN,"궁서");
//*/
		oldfont=(HFONT)SelectObject(hdc,font);
		TextOut(hdc, 100,200,str,lstrlen(str));
		SelectObject(hdc,oldfont);
		DeleteObject(font);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
