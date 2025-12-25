#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("FreeSpace");

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;

	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)(COLOR_BTNFACE+1);
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

#include <stdio.h>
#include <commctrl.h>
HWND hProg[24], hStatic[24];
#define MEGA (1048576)

void ReScan()
{
	int i;
	UINT Type;
	TCHAR Drive[MAX_PATH];
	TCHAR Caption[MAX_PATH+128];
	TCHAR Volume[MAX_PATH];
	ULARGE_INTEGER Avail, Total;
	double nTotal, nAvail;
	int Find=0;

	// 이미 만들어져 있는 프로그래스와 스태틱 윈도우를 파괴한다.
	for (i=0;i<24;i++) {
		if (hProg[i]!=NULL) {
			DestroyWindow(hProg[i]);
			DestroyWindow(hStatic[i]);
			hProg[i]=NULL;
			hStatic[i]=NULL;
		}
	}

	// 남은 용량을 다시 조사한다.
	for (i='C';i<='Z';i++) {
		wsprintf(Drive,"%c:\\",i);
		Type=GetDriveType(Drive);
		if (Type==DRIVE_FIXED) {
			hProg[Find]=CreateWindow(PROGRESS_CLASS,NULL,WS_CHILD | 
				WS_VISIBLE | WS_BORDER,
				10,Find*40+50,300,25,hWndMain,NULL,g_hInst,NULL);
			hStatic[Find]=CreateWindow("static","",WS_CHILD | WS_VISIBLE,
				320,Find*40+50,500,25,hWndMain,(HMENU)-1,g_hInst,NULL);

			GetDiskFreeSpaceEx(Drive, &Avail, &Total, NULL);
			nTotal=int(Total.QuadPart/MEGA)/1024.0;
			nAvail=int(Avail.QuadPart/MEGA)/1024.0;
			GetVolumeInformation(Drive,Volume,MAX_PATH,NULL,NULL,NULL,NULL,0);
			sprintf(Caption,"%C(%s): 총 %.2fG중 %.2fG사용, %.2fG사용가능",i,Volume,
				nTotal, (nTotal-nAvail),nAvail);
			SetWindowText(hStatic[Find],Caption);

			SendMessage(hProg[Find], PBM_SETRANGE, 0, MAKELPARAM(0,100));
			SendMessage(hProg[Find], PBM_SETPOS, int((nTotal-nAvail)*100/nTotal), 0);
			Find++;
		}
	}
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		InitCommonControls();
		CreateWindow("button","다시 조사",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			20,15,150,25,hWnd,(HMENU)100,g_hInst,NULL);
		ReScan();
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 100:
			ReScan();
			break;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

