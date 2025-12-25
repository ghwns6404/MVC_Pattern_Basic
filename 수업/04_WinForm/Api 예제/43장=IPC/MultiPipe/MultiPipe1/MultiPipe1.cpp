#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("MultiPipe1");

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
		100,100,500,100,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);

	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

// 차일드의 요청을 처리한다.
DWORD WINAPI PipeThread(LPVOID temp)
{
	HANDLE hPipe=(HANDLE)temp;
	TCHAR szInput[255], szOutput[255];
	DWORD dwRead, dwWritten;
	BOOL bSuc;

	// 요청을 받고 연산을 한 후 응답을 보낸다.
	for (;;) {
		bSuc=ReadFile(hPipe,szInput,255,&dwRead,NULL);
		if ((bSuc==FALSE) || (dwRead==0)) {
			break;
		}

		if (lstrcmp(szInput,"one")==0) lstrcpy(szOutput,"하나");
		if (lstrcmp(szInput,"two")==0) lstrcpy(szOutput,"둘");
		if (lstrcmp(szInput,"three")==0) lstrcpy(szOutput,"셋");
		if (lstrcmp(szInput,"four")==0) lstrcpy(szOutput,"넷");
		if (lstrcmp(szInput,"five")==0) lstrcpy(szOutput,"다섯");

		bSuc=WriteFile(hPipe,szOutput,lstrlen(szOutput)+1,&dwWritten,NULL);
		if ((bSuc==FALSE) || (dwWritten==0)) {
			break;
		}
	}

	// 파이프 해제
	FlushFileBuffers(hPipe);
	DisconnectNamedPipe(hPipe);
	CloseHandle(hPipe);
	return 0;
}

// 클라이언트 하나당 하나씩의 스레드를 만든다.
DWORD WINAPI MainThread(LPVOID temp)
{
	HANDLE hPipe;
	DWORD ThreadID;
	BOOL bCon;

	for (;;) {
		// 파이프 생성
		hPipe=CreateNamedPipe("\\\\.\\pipe\\MultiPipe",PIPE_ACCESS_DUPLEX,
			PIPE_TYPE_BYTE, 3,4096,0,1,NULL);

		// 차일드 대기. 접속되면 차일드 요청 처리 스레드 생성
		bCon=ConnectNamedPipe(hPipe,NULL);
		if ((bCon==FALSE) && (GetLastError() == ERROR_PIPE_CONNECTED))
			bCon=TRUE;
		if (bCon==TRUE) {
			CloseHandle(CreateThread(NULL,0,PipeThread,(LPVOID)hPipe,0,&ThreadID));
		} else {
			CloseHandle(hPipe);
		}
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	DWORD ThreadID;
	TCHAR *Mes="클라이언트로부터의 접속을 대기중이며 요청을 처리합니다";
	
	switch (iMessage) {
	case WM_CREATE:
		CloseHandle(CreateThread(NULL,0,MainThread,NULL,0,&ThreadID));
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,50,50,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
