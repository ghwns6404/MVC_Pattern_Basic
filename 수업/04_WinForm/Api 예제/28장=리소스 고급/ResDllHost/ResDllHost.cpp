#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ResDllHost");

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

#define IDS_STRING1                     1
#define IDS_STRING2                     2
#define ID_FILE_MENU1                   40001
#define ID_FILE_MENU2                   40002
#define ID_FILE_EXIT                    40003

HMODULE hRes;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	TCHAR Dir[MAX_PATH];
	HMENU hMenu;
	TCHAR str[128];

	switch (iMessage) {
	case WM_CREATE:
		GetCurrentDirectory(MAX_PATH,Dir);
		lstrcat(Dir,"\\ResDll.dll");
		hRes=LoadLibrary(Dir);
		if (hRes==NULL) {
			MessageBox(hWnd,"리소스 DLL 초기화 실패","에러",MB_OK);
			return -1;
		}
		hMenu=LoadMenu(hRes,"IDR_MENU1");
		SetMenu(hWnd,hMenu);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_FILE_MENU1:
			LoadString(hRes,IDS_STRING1,str,128);
			MessageBox(hWnd,str,"알림",MB_OK);
			break;
		case ID_FILE_MENU2:
			LoadString(hRes,IDS_STRING2,str,128);
			MessageBox(hWnd,str,"알림",MB_OK);
			break;
		case ID_FILE_EXIT:
			DestroyWindow(hWnd);
			break;
		}
		return 0;
	case WM_DESTROY:
		if (hRes)
			FreeLibrary(hRes);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

