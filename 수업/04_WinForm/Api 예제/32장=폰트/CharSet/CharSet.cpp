#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("CharSet");

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
	static TCHAR str[257];
	int i,j;
	switch (iMessage) {
	case WM_CREATE:
		for (i=0;i<256;i++) {
			str[i]=i;
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		font=CreateFont(16,0,0,0,0,0,0,0,ANSI_CHARSET,3,2,1,
			VARIABLE_PITCH | FF_ROMAN,"Times New Roman");
//		font=CreateFont(16,0,0,0,0,0,0,0,OEM_CHARSET,3,2,1,
//			VARIABLE_PITCH | FF_MODERN,"Modern");
//		font=CreateFont(16,0,0,0,0,0,0,0,SYMBOL_CHARSET,3,2,1,
//			VARIABLE_PITCH | FF_DONTCARE,"Wingdings");
//		font=CreateFont(16,0,0,0,0,0,0,0,SYMBOL_CHARSET,1,2,1,
//			VARIABLE_PITCH | FF_ROMAN,"Symbol");
		oldfont=(HFONT)SelectObject(hdc,font);
		for (i=0;i<16;i++)
			for (j=0;j<16;j++)
				TextOut(hdc, j*20,i*20,str+(i*16+j),1);
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
