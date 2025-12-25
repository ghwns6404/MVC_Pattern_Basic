#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("MkCursor");

HCURSOR CustCursor;
BYTE AndMask[]={0xFF, 0XFF, 0XFF, 0XFF, 0X80, 0X07, 0XE0, 0X01,
				0x80, 0X07, 0XE0, 0X01, 0X9F, 0XFF, 0XFF, 0XF9,
				0X9F, 0XFF, 0XFF, 0XF9, 0X9F, 0XFF, 0XFF, 0XF9,
				0X9F, 0XFF, 0XFF, 0XF9, 0X9F, 0XFF, 0XFF, 0XF9,
				0X9F, 0XFF, 0XFF, 0XF9, 0X9F, 0XFF, 0XFF, 0XF9,
				0X9F, 0XFF, 0XFF, 0XF9, 0X9F, 0XFF, 0XFF, 0XF9,
				0X9F, 0XFF, 0XFF, 0XF9, 0XFF, 0XF8, 0X1F, 0XFF,
				0XFF, 0XF8, 0X1F, 0XFF, 0XFF, 0XF8, 0X1F, 0XFF,
				0XFF, 0XF8, 0X1F, 0XFF, 0XFF, 0XF8, 0X1F, 0XFF,
				0XFF, 0XF8, 0X1F, 0XFF, 0X9F, 0XFF, 0XFF, 0XF9,
				0X9F, 0XFF, 0XFF, 0XF9, 0X9F, 0XFF, 0XFF, 0XF9,
				0X9F, 0XFF, 0XFF, 0XF9, 0X9F, 0XFF, 0XFF, 0XF9,
				0X9F, 0XFF, 0XFF, 0XF9, 0X9F, 0XFF, 0XFF, 0XF9,
				0X9F, 0XFF, 0XFF, 0XF9, 0X9F, 0XFF, 0XFF, 0XF9,
				0X9F, 0XFF, 0XFF, 0XF9, 0X80, 0X07, 0XE0, 0X01,
				0X80, 0X07, 0XE0, 0X01, 0xFF, 0XFF, 0XFF, 0XFF
};
BYTE XorMask[]={0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
				0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
				0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
				0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
				0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
				0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
				0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
				0X00, 0X03, 0XC0, 0X00, 0X00, 0X03, 0XC0, 0X00,
				0X00, 0X03, 0XC0, 0X00, 0X00, 0X03, 0XC0, 0X00,
				0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
				0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
				0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
				0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
				0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
				0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,
				0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00
};

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;

	CustCursor=CreateCursor(hInstance, 16,16,32,32,AndMask,XorMask);
	
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
	WndClass.hCursor=CustCursor;
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
	DestroyCursor(CustCursor);
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes="실행중에 만들어진 커서입니다.";

	switch (iMessage) {
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,10,10,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
