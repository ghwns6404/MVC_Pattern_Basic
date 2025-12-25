#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ToolEdit");

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;
	
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)(COLOR_BTNFACE+1);
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=NULL;
	WndClass.style=0;
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
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

#include <commctrl.h>
#define ID_TOOLBAR 100
HWND hToolBar;
HWND hEdit;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	TBBUTTON ToolBtn[7]={
		{STD_FILENEW,10,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{STD_FILEOPEN,11,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{STD_FILESAVE,12,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{200,0,0,TBSTYLE_SEP,0,0,0,0},
		{STD_CUT,13,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{STD_COPY,14,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{STD_PASTE,15,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0}
	};
	TCHAR szBuf[256];
	RECT srt;

	switch (iMessage) {
	case WM_CREATE:
		InitCommonControls();
		hToolBar=CreateToolbarEx(hWnd, WS_CHILD | WS_VISIBLE | WS_BORDER,
			ID_TOOLBAR, 15, HINST_COMMCTRL, IDB_STD_SMALL_COLOR, ToolBtn, 7,
			16,16,16,16,sizeof(TBBUTTON));
		SendMessage(hToolBar,TB_GETITEMRECT,3,(LPARAM)&srt);
		hEdit=CreateWindow("edit",NULL,WS_CHILD | WS_VISIBLE | WS_BORDER
			|ES_AUTOHSCROLL,
			srt.left+2,srt.top+1,(srt.right-srt.left-4),(srt.bottom-srt.top-2)
			,hWnd,(HMENU)20,g_hInst,NULL);
		SetParent(hEdit,hToolBar);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 20:
			switch (HIWORD(wParam)) {
			case EN_CHANGE:
				GetWindowText(hEdit,szBuf,256);
				SetWindowText(hWnd,szBuf);
				return 0;
			}
			break;
		}
		return 0;
	case WM_SIZE:
		SendMessage(hToolBar,TB_AUTOSIZE,0,0);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
