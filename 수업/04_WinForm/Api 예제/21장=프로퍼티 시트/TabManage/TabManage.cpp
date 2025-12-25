#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("TabManage");

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

#include <commctrl.h>
HWND hTab, hEdit;
#define EDIT_TEXT 0
#define BTN_ADD 1
#define BTN_GET 2
#define BTN_EDIT 3
#define BTN_DEL 4
#define BTN_DELALL 5
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	TCITEM tie;
	TCHAR Text[250];
	int count, idx;

	switch (iMessage) {
	case WM_CREATE:
		InitCommonControls();
		hTab=CreateWindow(WC_TABCONTROL,"",WS_CHILD | WS_VISIBLE 
			| WS_CLIPSIBLINGS,
			0,0,400,250,hWnd,(HMENU)0,g_hInst,NULL);
		hEdit=CreateWindow("edit",NULL,WS_CHILD | WS_VISIBLE | WS_BORDER,
			420,20,100,25,hWnd,(HMENU)EDIT_TEXT,g_hInst,NULL);
		CreateWindow("button","추가",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			420,50,100,25,hWnd,(HMENU)BTN_ADD,g_hInst,NULL);
		CreateWindow("button","조사",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			420,80,100,25,hWnd,(HMENU)BTN_GET,g_hInst,NULL);
		CreateWindow("button","변경",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			420,110,100,25,hWnd,(HMENU)BTN_EDIT,g_hInst,NULL);
		CreateWindow("button","삭제",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			420,140,100,25,hWnd,(HMENU)BTN_DEL,g_hInst,NULL);
		CreateWindow("button","모두 삭제",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			420,170,100,25,hWnd,(HMENU)BTN_DELALL,g_hInst,NULL);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case BTN_ADD:
			GetWindowText(hEdit,Text,250);
			if (lstrlen(Text) !=0 ) {
				tie.mask=TCIF_TEXT;
				tie.pszText=Text;
				count=TabCtrl_GetItemCount(hTab);
				TabCtrl_InsertItem(hTab,count,&tie);
			}
			break;
		case BTN_GET:
			tie.mask=TCIF_TEXT;
			tie.pszText=Text;
			tie.cchTextMax=250;
			idx=TabCtrl_GetCurSel(hTab);
			if (idx != -1) {
				TabCtrl_GetItem(hTab,idx,&tie);
				SetWindowText(hEdit,Text);
			}
			break;
		case BTN_EDIT:
			idx=TabCtrl_GetCurSel(hTab);
			GetWindowText(hEdit,Text,250);
			if (idx != -1 && lstrlen(Text) != 0) {
				tie.mask=TCIF_TEXT;
				tie.pszText=Text;
				TabCtrl_SetItem(hTab,idx,&tie);
			}
			break;
		case BTN_DEL:
			idx=TabCtrl_GetCurSel(hTab);
			if (idx != -1) {
				TabCtrl_DeleteItem(hTab,idx);
			}
			break;
		case BTN_DELALL:
			TabCtrl_DeleteAllItems(hTab);
			break;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
