#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("StatusProg");

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

#include <commctrl.h>
HWND hState;
HWND hProg;
int Pos;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int SBPart[3];
	RECT prt;
	TCHAR *Mes="마우스 왼쪽 버튼을 누르면 작업을 시작합니다";

	switch (iMessage) {
	case WM_CREATE:
		InitCommonControls();
		hState=CreateStatusWindow(WS_CHILD | WS_VISIBLE, "", hWnd,0);
		SBPart[0]=200;
		SBPart[1]=400;
		SBPart[2]=-1;
		SendMessage(hState, SB_SETPARTS, 3, (LPARAM)SBPart);
		SendMessage(hState, SB_SETTEXT,0,(LPARAM)"작업 진행 과정");
		SendMessage(hState, SB_SETTEXT,1,(LPARAM)"일반 상태란");
		return 0;
	case WM_LBUTTONDOWN:
		if (Pos) return 0;
		SendMessage(hState,SB_GETRECT,0,(LPARAM)&prt);
		hProg=CreateWindow(PROGRESS_CLASS,NULL,WS_CHILD | PBS_SMOOTH,
			prt.left,prt.top,prt.right-prt.left,prt.bottom-prt.top,
			hState,NULL,g_hInst,NULL);
		Pos=0;
		SendMessage(hProg, PBM_SETRANGE, 0, MAKELPARAM(0,100));
		SendMessage(hProg, PBM_SETPOS, Pos, 0);
		ShowWindow(hProg,SW_SHOW);
		SetTimer(hWnd,0,50,NULL);
		return 0;
	case WM_TIMER:
		if (Pos==100) {
			DestroyWindow(hProg);
			KillTimer(hWnd,0);
			Pos=0;
		} else {
			Pos++;
			SendMessage(hProg, PBM_SETPOS, Pos, 0);
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,10,10,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_SIZE:
		SendMessage(hState, WM_SIZE, wParam, lParam);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
