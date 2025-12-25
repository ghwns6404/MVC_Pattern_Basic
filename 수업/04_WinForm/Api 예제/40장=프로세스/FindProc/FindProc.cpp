#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("FindProc");

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
	TCHAR *Mes="왼쪽 클릭:CreateProcess로 실행, 오른쪽 클릭:WinExec로 실행";
	STARTUPINFO si={sizeof(STARTUPINFO),};
	PROCESS_INFORMATION pi;
	HWND hMemo;

	switch (iMessage) {
	case WM_LBUTTONDOWN:
		CreateProcess(NULL, "NotePad.exe",NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);

		//WaitForInputIdle(pi.hProcess, INFINITE);
		hMemo = FindWindow("NotePad", NULL);
		if (hMemo == NULL) {
			MessageBox(hWnd, "메모장 윈도우를 찾을 수 없습니다.", "알림", MB_OK);
		} else {
			MessageBox(hWnd, "메모장 윈도우를 찾았습니다.", "알림", MB_OK);
		}
		return 0;
	case WM_RBUTTONDOWN:
		WinExec("Notepad.exe",SW_SHOWNORMAL);

		hMemo = FindWindow("NotePad", NULL);
		if (hMemo == NULL) {
			MessageBox(hWnd, "메모장 윈도우를 찾을 수 없습니다.", "알림", MB_OK);
		} else {
			MessageBox(hWnd, "메모장 윈도우를 찾았습니다.", "알림", MB_OK);
		}
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

