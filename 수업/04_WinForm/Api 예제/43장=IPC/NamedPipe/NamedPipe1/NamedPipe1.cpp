#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("NamedPipe1");

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
		100,100,600,150,
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
	HANDLE hPipe;
	BOOL bCon;
	DWORD dwWritten;
	TCHAR *LongStr="아주 긴 문자열ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	TCHAR *Mes="마우스 왼쪽 버튼을 누르면 차일드에게 문자열을 보냅니다";

	switch (iMessage) {
	case WM_LBUTTONDOWN:
		hPipe=CreateNamedPipe("\\\\.\\pipe\\MyPipe", PIPE_ACCESS_OUTBOUND, 
			PIPE_TYPE_BYTE, PIPE_UNLIMITED_INSTANCES,4096,0,0,NULL);
		SetWindowText(hWnd,"NPipeSvr-파이프 생성, 차일드 접속 대기");
		bCon=ConnectNamedPipe(hPipe,NULL);
		if ((bCon==FALSE) && (GetLastError() == ERROR_PIPE_CONNECTED))
			bCon=TRUE;
		if (bCon==TRUE) {
			SetWindowText(hWnd,"NamedPipe1-차일드에게 문자열 전송");
			WriteFile(hPipe,LongStr,lstrlen(LongStr),&dwWritten,NULL);
			FlushFileBuffers(hPipe);
			DisconnectNamedPipe(hPipe);
		}
		CloseHandle(hPipe);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,10,10,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}


