#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("KeyHook");

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

TCHAR *Mes="키보드 훅 테스트 프로그램입니다.";
TCHAR Mes2[128];
TCHAR Mes3[128];
int Count=0;
HHOOK hKeyHook;
LRESULT CALLBACK KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode>=0) {
		InvalidateRect(hWndMain,NULL,TRUE);
		if (wParam == VK_F2) {
			wsprintf(Mes2,"F2는 입력이 금지된 키입니다.");
			return 1;
		} else {
			wsprintf(Mes2,"nCode=%d, wParam=%u, lParam=%08x, Count=%d",
				nCode, wParam, lParam,Count++);
		}
	}
	return CallNextHookEx(hKeyHook,nCode,wParam,lParam);
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		hKeyHook=SetWindowsHookEx(WH_KEYBOARD,KeyHookProc,NULL,GetCurrentThreadId());
		return 0;
	case WM_KEYDOWN:
		wsprintf(Mes3,"실제 받은 키 : %u",wParam);
		InvalidateRect(hWnd,NULL,TRUE);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,100,10,Mes,lstrlen(Mes));
		TextOut(hdc,100,30,Mes2,lstrlen(Mes2));
		TextOut(hdc,100,50,Mes3,lstrlen(Mes3));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		UnhookWindowsHookEx(hKeyHook);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
