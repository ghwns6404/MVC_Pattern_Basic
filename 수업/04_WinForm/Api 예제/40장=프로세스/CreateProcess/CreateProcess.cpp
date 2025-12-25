#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("CreateProcess");

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

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes="마우스 왼쪽 버튼을 누르면 메모장을 실행합니다";
	STARTUPINFO si={sizeof(STARTUPINFO),};
	PROCESS_INFORMATION pi;

	switch (iMessage) {
	case WM_LBUTTONDOWN:
/*프로세스를 생성하는 가장 간단한 방법
		WinExec("Notepad.exe",SW_SHOWNORMAL);
//*/

/* LoadModule 함수를 사용하는 방법
		{
			typedef struct tagLOADPARMS32 { 
				LPSTR lpEnvAddress;  // address of environment strings 
				LPSTR lpCmdLine;     // address of command line 
				LPSTR lpCmdShow;     // how to show new program 
				DWORD dwReserved;    // must be zero 
			} LOADPARMS32; 
			
			LOADPARMS32 lp;
			TCHAR CmdLine[2]={0,};
			WORD CmdShow[2]={2,SW_SHOWNORMAL};

			lp.lpEnvAddress = NULL;
			lp.lpCmdLine = CmdLine;
			lp.lpCmdShow = (LPSTR)&CmdShow;
			lp.dwReserved = 0;
			LoadModule("Notepad.exe",&lp);
		}
//*/

/* CreateProcess 함수를 사용하는 방법(두 번째 인수만 사용)
		CreateProcess(NULL, "NotePad.exe",NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);
//*/

/* CreateProcess 함수를 사용하는 방법(첫 번째 인수 사용)
		TCHAR Path[MAX_PATH];
		GetWindowsDirectory(Path, MAX_PATH);
		lstrcat(Path, "\\NotePad.exe");
		CreateProcess(Path, NULL,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);
//*/

/* CreateProcess 함수를 사용하는 방법(두 번째 인수로 명령행 인수 전달)
		CreateProcess(NULL, "NotePad.exe c:\\autoexec.bat",NULL,NULL,
			FALSE,0,NULL,NULL,&si,&pi);
//*/

//* CreateProcess 함수를 사용하는 방법(두 인수 모두 사용)
		TCHAR Path[MAX_PATH];
		GetWindowsDirectory(Path, MAX_PATH);
		lstrcat(Path, "\\NotePad.exe");
		CreateProcess(Path, " c:\\autoexec.bat",NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);
//*/

/* STARTUPINFO 구조체 사용
		si.dwFlags = STARTF_USEPOSITION | STARTF_USESIZE;
		si.dwX = 100;
		si.dwY = 50;
		si.dwXSize = 640;
		si.dwYSize = 480;

		CreateProcess(NULL, "First.exe",NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);
//*/
		// 핸들은 닫아야 한다.
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
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
