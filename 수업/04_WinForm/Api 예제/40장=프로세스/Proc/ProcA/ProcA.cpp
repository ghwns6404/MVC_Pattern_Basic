#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ProcA");

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
		50,50,640,200,
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
	STARTUPINFO si={sizeof(STARTUPINFO),};
	PROCESS_INFORMATION pi;
	HWND hProcB;
	TCHAR *Mes="마우스 왼쪽 버튼을 누르면 프로세스를 생성한 후 ProcB로 ID를 전달합니다";
	static TCHAR Status[256]="";

	switch (iMessage) {
	case WM_LBUTTONDOWN:
		// 프로세스 ID를 전달할 ProcB를 먼저 찾는다. 없으면 예제 테스트가 불가능하다.
		hProcB=FindWindow(NULL, "ProcB");
		if (hProcB == NULL) {
			MessageBox(hWnd, "이 예제를 테스트해 보려면 ProcB를 먼저 실행하십시오",
				"알림",MB_OK);
			return 0;
		}
		if (CreateProcess(NULL, "NotePad.exe",NULL,NULL,FALSE,0,NULL,NULL,&si,&pi)) {
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}

		// ProcB에게 사용자 정의 메시지를 보내 프로세스 ID를 전달한다.
		SendMessage(hProcB, WM_USER+1,(WPARAM)pi.dwProcessId, 0);
		wsprintf(Status,"프로세스 핸들 %u와 ID %u를 생성했습니다.",
			pi.hProcess, pi.dwProcessId);
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,50,50,Mes,lstrlen(Mes));
		TextOut(hdc,50,90,Status,lstrlen(Status));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

