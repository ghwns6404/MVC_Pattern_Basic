#include <windows.h>
#include "mmsystem.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("SoundPlayer");

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

#define ID_LISTBOX 100
HWND hList;
TCHAR NowPath[MAX_PATH];
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR Wave[MAX_PATH];
	TCHAR ext[_MAX_EXT];
	BOOL dir;
	TCHAR *Mes="리스트 박스에서 선택한 사운드를 재생합니다.";
	TCHAR tPath[MAX_PATH*2],*p;

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		hList=CreateWindow("listbox",NULL,WS_CHILD | WS_VISIBLE | WS_BORDER |
			WS_VSCROLL | LBS_NOTIFY,
			0,0,0,0,hWnd,(HMENU)ID_LISTBOX,g_hInst,NULL);
		GetCurrentDirectory(MAX_PATH,NowPath);
		wsprintf(tPath,"%s\\*.*",NowPath);
		SendMessage(hList, LB_DIR, (WPARAM)(DDL_DIRECTORY | DDL_DRIVES),
			(LPARAM)tPath);
		return 0;
	case WM_SIZE:
		if (wParam != SIZE_MINIMIZED) {
			MoveWindow(hList,0,50,LOWORD(lParam),HIWORD(lParam)-50,TRUE);
		}
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_LISTBOX:
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				DlgDirSelectEx(hWnd,Wave,MAX_PATH,ID_LISTBOX);
				_splitpath(Wave,NULL,NULL,NULL,ext);
				if (lstrcmpi(ext,".wav")==0) {
					wsprintf(tPath,"%s\\%s",NowPath,Wave);
					PlaySound(tPath,NULL,SND_ASYNC);
				}
				break;
			case LBN_DBLCLK:
				dir=DlgDirSelectEx(hWnd,Wave,MAX_PATH,ID_LISTBOX);
				if (dir) {
					if (lstrcmp(Wave,"..\\")==0) {
						p=strrchr(NowPath,'\\');
						*p=0;
					} else if (Wave[1]==':') {
						wsprintf(NowPath,"%c:\\",Wave[0]);
					} else {
						wsprintf(tPath,"%s\\%s",NowPath,Wave);
						tPath[lstrlen(tPath)-1]=0;
						lstrcpy(NowPath,tPath);
					}
					wsprintf(tPath,"%s\\*.*",NowPath);
					SendMessage(hList,LB_RESETCONTENT,0,0);
					SendMessage(hList, LB_DIR, (WPARAM)(DDL_DIRECTORY | DDL_DRIVES),
						(LPARAM)tPath);
					InvalidateRect(hWndMain,NULL,TRUE);
				}
				break;
			}
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,10,10,Mes,lstrlen(Mes));
		wsprintf(tPath,"현재 디렉토리=%s",NowPath);
		TextOut(hdc,10,30,tPath,lstrlen(tPath));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
