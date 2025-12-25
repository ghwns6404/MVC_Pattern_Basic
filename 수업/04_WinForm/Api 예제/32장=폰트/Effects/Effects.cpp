#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("Effects");

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
	int i,j;
	TCHAR str[128];
	switch (iMessage) {
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		font=CreateFont(50,0,0,0,FW_BOLD,0,0,0,HANGUL_CHARSET,3,2,1,
			VARIABLE_PITCH | FF_ROMAN,"궁서");
		oldfont=(HFONT)SelectObject(hdc,font);

		SetBkMode(hdc,TRANSPARENT);
		lstrcpy(str,"외곽선 문자");
		for (i=-1;i<=1;i++) {
			for (j=-1;j<=1;j++) {
				TextOut(hdc,100+i,50+j,str,lstrlen(str));
			}
		}
		SetTextColor(hdc,RGB(255,255,255));
		TextOut(hdc,100,50,str,lstrlen(str));

		lstrcpy(str,"그림자 문자");
		SetTextColor(hdc,RGB(0,0,0));
		TextOut(hdc,100+2,150+2,str,lstrlen(str));
		SetTextColor(hdc,RGB(255,255,255));
		TextOut(hdc,100,150,str,lstrlen(str));

		lstrcpy(str,"또 다른 그림자 문자");
		SetTextColor(hdc,RGB(192,192,192));
		TextOut(hdc,100+5,250+5,str,lstrlen(str));
		SetTextColor(hdc,RGB(0,0,0));
		TextOut(hdc,100,250,str,lstrlen(str));

		SelectObject(hdc,oldfont);
		DeleteObject(font);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
};
