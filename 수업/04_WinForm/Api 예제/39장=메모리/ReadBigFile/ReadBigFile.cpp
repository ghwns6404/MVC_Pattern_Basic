#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ReadBigFile");

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

HANDLE hFile;
HANDLE hFMap;
TCHAR *base;
int line;
#define LINE 600000
#define PAGE 50
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR buf[256];
	DWORD dwWritten;
	int i;

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		hFile=CreateFile("c:\\BigFile.dat",GENERIC_WRITE,0,NULL,
			CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL);
		if (hFile != INVALID_HANDLE_VALUE) {
			for (i=0;i<LINE;i++) {
				wsprintf(buf,"Read Very BigFile Using Memory Mapped File"
					" - %06d Lines\r\n",i);
				WriteFile(hFile,buf,lstrlen(buf),&dwWritten,NULL);
			}
			MessageBox(hWnd,"c:\\BigFile.dat를 생성했습니다","알림",MB_OK);
			CloseHandle(hFile);
		}
		hFile=CreateFile("c:\\BigFile.dat",GENERIC_READ, 0, 
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		hFMap=CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
		base=(TCHAR *)MapViewOfFile(hFMap, FILE_MAP_READ,0,0,0);
		return 0;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_DOWN:
			line = min(line+1,LINE-1);
			break;
		case VK_UP:
			line = max(line-1,0);
			break;
		case VK_NEXT:
			line = min(line+PAGE,LINE-1);
			break;
		case VK_PRIOR:
			line = max(line-PAGE,0);
			break;
		}
		InvalidateRect(hWnd,NULL,FALSE);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		for (i=0;i<PAGE;i++) {
			if (line+i < LINE) {
				TextOut(hdc,10,i*16,base+(line+i)*59,57);
			}
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		UnmapViewOfFile(base);
		CloseHandle(hFMap);
		CloseHandle(hFile);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
