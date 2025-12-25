#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("HookNotePad");

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
	hWndMain=hWnd;
	
	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

#include "../HookNotePadDll/HookNotePadDll.h"
TCHAR *Mes="메모장에서 BABO를 입력하면 chaunjae로 변경합니다.";
TCHAR *szSrc="BABO";
TCHAR *szDest="CHUNJAE";
int idx;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HWND hFGWnd;
	TCHAR szClass[32];
	int i;

	switch (iMessage) {
	case WM_CREATE:
		InstallHook(hWnd);
		return 0;
	case WM_USER+1:
		hFGWnd=GetForegroundWindow();
		GetClassName(hFGWnd,szClass,32);
		if (lstrcmpi(szClass,"NotePad")==0 && (lParam & 0x80000000)==0) {
			if (wParam == (WPARAM)szSrc[idx]) {
				idx++;
			} else {
				idx=0;
			}
			if (szSrc[idx]==0) {
				for (i=0;i<lstrlen(szSrc);i++) {
					keybd_event(VK_BACK,0,0,0);
					keybd_event(VK_BACK,0,KEYEVENTF_KEYUP,0);
				}
				for (i=0;i<lstrlen(szDest);i++) {
					keybd_event(szDest[i],0,0,0);
					keybd_event(szDest[i],0,KEYEVENTF_KEYUP,0);
				}
			}
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,10,10,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		UninstallHook();
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
