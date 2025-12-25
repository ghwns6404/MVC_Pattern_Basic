#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ProcB");

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
		50,250,640,200,
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
	static DWORD ProcID = 0;
	static HANDLE hProc = 0;
	TCHAR str[256];
	TCHAR *Mes="마우스 왼쪽 버튼을 누르면 ProcA로부터 받은 프로세스를 종료합니다";
	static TCHAR Status[256]="";
	DWORD ExitCode;

	switch (iMessage) {
	// ProcA로부터 프로세스의 ID를 전달받아 프로세스 핸들을 먼저 구해 놓는다.
	case WM_USER+1:
		ProcID = wParam;
		hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcID);
		wsprintf(Status,"프로세스 ID로 %u를 받았으며 이 ID로부터 프로세스 핸들 "
			"%u를 열었습니다",ProcID, hProc);
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_LBUTTONDOWN:
		// 해당 프로세스가 실행중인 경우 프로세스를 강제 종료한다.
		GetExitCodeProcess(hProc, &ExitCode);
		if (ExitCode != STILL_ACTIVE) {
			MessageBox(hWnd, "프로세스 핸들이 무효합니다.","알림",MB_OK);
			return 0;
		} else {
			wsprintf(str, "%u 핸들을 가진 프로세스를 종료하겠습니다",hProc);
			MessageBox(hWnd, str, "알림", MB_OK);
			TerminateProcess(hProc, 0);
			ProcID = 0;
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,50,50,Mes,lstrlen(Mes));
		TextOut(hdc,50,90,Status,lstrlen(Status));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		if (hProc != 0)
			CloseHandle(hProc);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

