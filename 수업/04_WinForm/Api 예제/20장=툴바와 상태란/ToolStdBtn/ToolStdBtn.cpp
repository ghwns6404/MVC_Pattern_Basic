#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ToolStdBtn");

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
	
	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

#include <commctrl.h>
#define ID_TOOLBAR 100
HWND hToolBar;
BOOL bBig;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes="마우스 왼쪽 버튼을 누르면 버튼의 크기가 바뀝니다.";
	TBBUTTON ToolBtn[20]={
		{STD_FILENEW,10,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{STD_FILEOPEN,11,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{STD_FILESAVE,12,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{4,0,0,TBSTYLE_SEP,0,0,0,0},
		{STD_CUT,13,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{STD_COPY,14,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{STD_PASTE,15,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{STD_DELETE,16,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{4,0,0,TBSTYLE_SEP,0,0,0,0},
		{STD_UNDO,17,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{STD_REDOW,18,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{4,0,0,TBSTYLE_SEP,0,0,0,0},
		{STD_PRINT,19,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{STD_PRINTPRE,20,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{4,0,0,TBSTYLE_SEP,0,0,0,0},
		{STD_FIND,21,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{STD_REPLACE,22,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{STD_PROPERTIES,23,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{4,0,0,TBSTYLE_SEP,0,0,0,0},
		{STD_HELP,24,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0}
	};

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		InitCommonControls();
		bBig=FALSE;
		hToolBar=CreateToolbarEx(hWndMain, WS_CHILD | WS_VISIBLE | WS_BORDER,
			ID_TOOLBAR, 15, HINST_COMMCTRL, IDB_STD_SMALL_COLOR, ToolBtn, 20,
			16,16,16,16,sizeof(TBBUTTON));
		return 0;
	case WM_LBUTTONDOWN:
		TBREPLACEBITMAP rb;
		int bx,by;
		bBig=!bBig;
		rb.hInstOld=rb.hInstNew=HINST_COMMCTRL;
		if (bBig) {
			rb.nIDOld=IDB_STD_SMALL_COLOR;
			rb.nIDNew=IDB_STD_LARGE_COLOR;
			bx=24;by=23;
		} else {
			rb.nIDOld=IDB_STD_LARGE_COLOR;
			rb.nIDNew=IDB_STD_SMALL_COLOR;
			bx=16;by=15;
		}
		rb.nButtons=15;

		SendMessage(hToolBar,TB_REPLACEBITMAP,0,(LPARAM)&rb);
		SendMessage(hToolBar,TB_SETBITMAPSIZE,0,(LPARAM)MAKELONG(bx,by));
		SendMessage(hToolBar,TB_AUTOSIZE,0,0);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,10,50,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
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
