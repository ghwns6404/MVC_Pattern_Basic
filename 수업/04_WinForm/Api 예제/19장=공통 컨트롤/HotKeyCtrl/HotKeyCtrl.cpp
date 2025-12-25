#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("HotKeyCtrl");

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

#include <commctrl.h>
HWND hHotKey;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	LRESULT Key;
	TCHAR sMod[32];
	TCHAR vk;
	TCHAR str[128];

	switch (iMessage) {
	case WM_CREATE:
		InitCommonControls();
		hHotKey=CreateWindow(HOTKEY_CLASS,NULL,WS_CHILD | WS_VISIBLE | WS_BORDER,
			10,10,200,30,hWnd,NULL,g_hInst,NULL);
		return 0;
	case WM_LBUTTONDOWN:
		sMod[0]=0;
		Key=SendMessage(hHotKey,HKM_GETHOTKEY,0,0);
		if (HIBYTE(LOWORD(Key)) & HOTKEYF_CONTROL) 
			strcat(sMod, "Ctrl+");
		if (HIBYTE(LOWORD(Key)) & HOTKEYF_SHIFT) 
			strcat(sMod, "Shift+");
		if (HIBYTE(LOWORD(Key)) & HOTKEYF_ALT) 
			strcat(sMod, "Alt+");
		if (HIBYTE(LOWORD(Key)) & HOTKEYF_EXT) 
			strcat(sMod, "Ext+");
		vk=(TCHAR)(LOBYTE(LOWORD(Key)));
		if (vk==0) {
			wsprintf(str,"핫키를 지정하지 않았습니다");
		} else {
			wsprintf(str,"%s%c키를 핫키로 지정했습니다",sMod,vk);
		}
		MessageBox(hWnd,str,"알림",MB_OK);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

