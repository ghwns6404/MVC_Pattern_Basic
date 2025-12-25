#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("SHRegTest");

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

#include "ShReg.h"
#define KEY "Software\\MiyoungSoft\\SHRegLibTest\\"
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	RECT rt;
	static HWND hEdit;
	TCHAR str[256];
	switch (iMessage) {
	case WM_CREATE:
		rt.left=SHRegReadInt(SHCU,KEY"Position","Left",0);
		rt.top=SHRegReadInt(SHCU,KEY"Position","Top",0);
		rt.right=SHRegReadInt(SHCU,KEY"Position","Right",300);
		rt.bottom=SHRegReadInt(SHCU,KEY"Position","Bottom",200);
		MoveWindow(hWnd, rt.left, rt.top, rt.right-rt.left, 
			rt.bottom-rt.top, TRUE);
		hEdit=CreateWindow("edit",NULL,WS_CHILD | WS_VISIBLE | WS_BORDER | 
			ES_AUTOHSCROLL,10,10,200,25,hWnd,(HMENU)100,g_hInst,NULL);
		SHRegReadString(SHCU,KEY"Edit","Str","¹®ÀÚ¿­",str,256);
		SetWindowText(hEdit,str);
		return 0;
	case WM_DESTROY:
		GetWindowRect(hWnd, &rt);
		SHRegWriteInt(SHCU,KEY"Position","Left",rt.left);
		SHRegWriteInt(SHCU,KEY"Position","Top",rt.top);
		SHRegWriteInt(SHCU,KEY"Position","Right",rt.right);
		SHRegWriteInt(SHCU,KEY"Position","Bottom",rt.bottom);
		GetWindowText(hEdit,str,256);
		SHRegWriteString(SHCU,KEY"Edit","Str",str);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
