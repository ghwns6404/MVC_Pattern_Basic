#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("Mless");
int x;
int y;
TCHAR str[128];
HWND hMDlg;

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
		if (!IsWindow(hMDlg) || !IsDialogMessage(hMDlg,&Message)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}
	return (int)Message.wParam;
}

#include "resource.h"
BOOL CALLBACK MlessDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage) {
	case WM_INITDIALOG:
		SetDlgItemText(hDlg,IDC_STR,str);
		SetDlgItemInt(hDlg,IDC_X,x,FALSE);
		SetDlgItemInt(hDlg,IDC_Y,y,FALSE);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_CHANGE:
			GetDlgItemText(hDlg,IDC_STR,str,128);
			x=GetDlgItemInt(hDlg,IDC_X,NULL,FALSE);
			y=GetDlgItemInt(hDlg,IDC_Y,NULL,FALSE);
			InvalidateRect(hWndMain,NULL,TRUE);
			return TRUE;
		case IDCANCEL:
		case ID_CLOSE:
			DestroyWindow(hMDlg);
			hMDlg=NULL;
			return TRUE;
		}
		break;
	}
	return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		x=100;
		y=100;
		lstrcpy(str,TEXT("String"));
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,x,y,str,lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		if (!IsWindow(hMDlg)) {
			hMDlg=CreateDialog(g_hInst,MAKEINTRESOURCE(IDD_DIALOG1),
				hWnd,MlessDlgProc);
			ShowWindow(hMDlg,SW_SHOW);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

