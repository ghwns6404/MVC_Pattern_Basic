#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("CenterMessage");

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

HHOOK hCbtHook;
void MoveToParentCenter(HWND hWnd)
{
	RECT wrt,crt;
	HWND hParent;

	hParent=GetParent(hWnd);
	if (IsIconic(hParent)) {
		ShowWindow(hParent,SW_RESTORE);
	}

	GetWindowRect(hParent,&wrt);
	GetWindowRect(hWnd,&crt);
	SetWindowPos(hWnd,HWND_NOTOPMOST,wrt.left+(wrt.right-wrt.left)/2-(crt.right-crt.left)/2,
		wrt.top+(wrt.bottom-wrt.top)/2-(crt.bottom-crt.top)/2,0,0,SWP_NOSIZE);
}

LRESULT CALLBACK NewWndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	WNDPROC OldProc;
	OldProc=(WNDPROC)GetProp(hWnd,"OldProp");

	switch (iMessage) {
	case WM_CREATE:
		MoveToParentCenter(hWnd);
		break;
	case WM_NCDESTROY:
		SetWindowLong(hWnd,GWL_WNDPROC,(DWORD)OldProc);
		RemoveProp(hWnd,"OldProp");
		break;
	}
	return CallWindowProc(OldProc,hWnd,iMessage,wParam,lParam);
}

LRESULT CALLBACK CbtHookProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	CBT_CREATEWND *pCbt;
	HWND hWnd;
	TCHAR szClassName[32];
	WNDPROC OldProc;

	if (nCode == HCBT_CREATEWND) {
		hWnd=(HWND)wParam;
		pCbt=(CBT_CREATEWND *)lParam;

		if (HIWORD(pCbt->lpcs->lpszClass)) {
			lstrcpy(szClassName,pCbt->lpcs->lpszClass);
		} else {
			GlobalGetAtomName((ATOM)pCbt->lpcs->lpszClass,szClassName,32);
		}

		if (lstrcmpi(szClassName,"#32770")==0 && ((pCbt->lpcs->style & WS_CHILD)==0)) {
			OldProc=(WNDPROC)GetWindowLong(hWnd,GWL_WNDPROC);
			SetProp(hWnd,"OldProp",OldProc);
			SetWindowLong(hWnd,GWL_WNDPROC,(DWORD)NewWndProc);
		}
	}
	return CallNextHookEx(hCbtHook,nCode,wParam,lParam);
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes="마우스 왼쪽 버튼을 누르면 메시지 박스가 나타납니다.";

	switch (iMessage) {
	case WM_CREATE:
		hCbtHook=SetWindowsHookEx(WH_CBT,CbtHookProc,NULL,GetCurrentThreadId());
		return 0;
	case WM_LBUTTONDOWN:
		MessageBox(hWnd,"이 메시지 박스는 부모 윈도우의 중앙에 나타납니다","알림",MB_OK);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,10,10,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		UnhookWindowsHookEx(hCbtHook);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
