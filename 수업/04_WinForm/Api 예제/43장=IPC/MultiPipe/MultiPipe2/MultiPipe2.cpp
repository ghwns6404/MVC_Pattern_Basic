#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("MultiPipe2");

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
		CW_USEDEFAULT,CW_USEDEFAULT,500,100,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);

	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}


TCHAR Mes[255]="파이프 서버를 실행시켜 주십시오";
#define random(i) (rand() % i)

// 서버로 요청을 보내 결과를 받아온다.
DWORD WINAPI MainThread(LPVOID temp)
{
	HANDLE hPipe;
	TCHAR szInput[255], szOutput[255];
	DWORD dwRead, dwWritten;
	TCHAR arInput[5][10]={"one","two","three","four","five"};
	BOOL bSuc;

	// 서버가 파이프를 생성할 때까지 무한 대기한다.
	for (;;) {
		if (WaitNamedPipe("\\\\.\\pipe\\MultiPipe",NMPWAIT_WAIT_FOREVER)==TRUE) {
			hPipe=CreateFile("\\\\.\\pipe\\MultiPipe",GENERIC_READ | GENERIC_WRITE,
				0,NULL,OPEN_EXISTING,0,NULL);
			if (hPipe!=INVALID_HANDLE_VALUE)
				break;
		}
	}

	for (;;) {
		// one~five중 하나를 난수로 선택한다.
		lstrcpy(szInput,arInput[random(5)]);

		// 요청을 보낸 후 응답을 받는다.
		bSuc=WriteFile(hPipe,szInput,lstrlen(szInput)+1,&dwWritten,NULL);
		if ((bSuc==FALSE) || (dwWritten==0))
			break;
		ReadFile(hPipe,szOutput,255,&dwRead,NULL);

		// 응답을 화면으로 출력한다.
		wsprintf(Mes,"%s=%s",szInput,szOutput);
		InvalidateRect(hWndMain,NULL,TRUE);
		UpdateWindow(hWndMain);

		Sleep(500);
	}

	// 서버가 먼저 종료된 경우
	lstrcpy(Mes,"서버가 종료되었습니다.");
	InvalidateRect(hWndMain,NULL,TRUE);

	// 파이프를 해제하고 결과를 메인 윈도우에 출력한다.
	CloseHandle(hPipe);
	
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	DWORD ThreadID;

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		srand(GetTickCount());
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
