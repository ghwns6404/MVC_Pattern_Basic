#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("KeyBeep");

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
		CW_USEDEFAULT,CW_USEDEFAULT,600,100,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	hWndMain=hWnd;
	
	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

#include <mmsystem.h>
#include "resource.h"
#include "../KeyBeepDll/KeyBeepDll.h"
TCHAR *Mes="시스템의 키 입력을 감시하며 키가 눌러질 때마다 소리를 냅니다.";
TCHAR Mes2[128];

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static count;

	switch (iMessage) {
	case WM_CREATE:
		InstallHook(hWnd);
		return 0;
	case WM_USER+1:
		wsprintf(Mes2,"입력된 키:%d, lParam : %x ",wParam,lParam);
		InvalidateRect(hWnd,NULL,TRUE);
		if ((lParam & 0x80000000)==0) {
			if (wParam >= 'A' && wParam <= 'Z') {
				PlaySound(MAKEINTRESOURCE(IDR_CHARACTER), g_hInst, SND_RESOURCE | SND_ASYNC);
			}
			if (wParam == ' ') {
				PlaySound(MAKEINTRESOURCE(IDR_SPACE), g_hInst, SND_RESOURCE | SND_ASYNC);
			}
			if (wParam >= '0' && wParam <= '9') {
				PlaySound(MAKEINTRESOURCE(IDR_NUMBER), g_hInst, SND_RESOURCE | SND_ASYNC);
			}
			if (wParam >= VK_F1 && wParam <= VK_F24) {
				PlaySound(MAKEINTRESOURCE(IDR_FUNCTION), g_hInst, SND_RESOURCE | SND_ASYNC);
			}
			if (wParam >= VK_PRIOR && wParam <= VK_HELP) {
				PlaySound(MAKEINTRESOURCE(IDR_EDIT), g_hInst, SND_RESOURCE | SND_ASYNC);
			}
			if (wParam == VK_BACK || wParam == VK_TAB || wParam == VK_RETURN) {
				PlaySound(MAKEINTRESOURCE(IDR_BACKTAB), g_hInst, SND_RESOURCE | SND_ASYNC);
			}
			if ((wParam >= 186 && wParam <= 191) || (wParam >= 219 && wParam <= 222)) {
				PlaySound(MAKEINTRESOURCE(IDR_PUNC), g_hInst, SND_RESOURCE | SND_ASYNC);
			}
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,10,10,Mes,lstrlen(Mes));
		TextOut(hdc,10,30,Mes2,lstrlen(Mes2));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		UninstallHook();
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
