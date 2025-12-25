#include <windows.h>
#include "ShReg.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("AutoRun");

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

#define RunReg "Software\\Microsoft\\Windows\\CurrentVersion\\Run"
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes="왼쪽 클릭:자동 실행, 오른쪽 클릭:자동 실행 해제";
	TCHAR Status[128];
	TCHAR Path[MAX_PATH];
	HKEY key;
	DWORD dwDisp;

	switch (iMessage) {
	case WM_LBUTTONDOWN:
		GetModuleFileName(NULL,Path,MAX_PATH);
		SHRegWriteString(SHCU,RunReg,"AutoRun",Path);
		InvalidateRect(hWnd,NULL,TRUE);
		return 0;
	case WM_RBUTTONDOWN:
		RegCreateKeyEx(SHCU,RunReg,0,NULL,REG_OPTION_NON_VOLATILE, 
			KEY_SET_VALUE,NULL,&key,&dwDisp);
		RegDeleteValue(key,"AutoRun");
		RegCloseKey(key);
		InvalidateRect(hWnd,NULL,TRUE);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,10,10,Mes,lstrlen(Mes));
		SHRegReadString(SHCU,RunReg,"AutoRun","None",Path,MAX_PATH);
		if (lstrcmp(Path,"None") != 0) {
			lstrcpy(Status,"자동 실행상태입니다.");
		} else {
			lstrcpy(Status,"자동 실행상태가 아닙니다.");
		}
		TextOut(hdc,10,30,Status,lstrlen(Status));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
