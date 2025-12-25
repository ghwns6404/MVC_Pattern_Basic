#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("MultiHost");

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

#define IDS_LANGID                      1
#define IDS_CAPTION                     2
#define IDS_MENU1                       3
#define IDS_LBTN                        4
#define IDS_MES                         5
#define IDR_MENU1                       101
#define IDD_DIALOG1                     102
#define ID_FILE_MENU1                   40001
#define ID_FILE_ABOUT                   40002
#define ID_FILE_EXIT                    40003

HMODULE hRes;

BOOL CALLBACK AboutDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage) {
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			EndDialog(hDlg,IDOK);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR Dir[MAX_PATH];
	HMENU hMenu;
	TCHAR str[128];

	switch (iMessage) {
	case WM_CREATE:
		GetCurrentDirectory(MAX_PATH,Dir);
		lstrcat(Dir,"\\MultiLang.dll");
		hRes=LoadLibrary(Dir);
		if (hRes==NULL) {
			MessageBox(hWnd,"Resource DLL not found","Critical Error",MB_OK);
			return -1;
		}
		hMenu=LoadMenu(hRes,MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(hWnd,hMenu);
		LoadString(hRes,IDS_CAPTION,str,128);
		SetWindowText(hWnd,str);
		return 0;
	case WM_LBUTTONDOWN:
		LoadString(hRes,IDS_LBTN,str,128);
		MessageBox(hWnd,str,"Message",MB_OK);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_FILE_MENU1:
			LoadString(hRes,IDS_MENU1,str,128);
			MessageBox(hWnd,str,"Message",MB_OK);
			break;
		case ID_FILE_ABOUT:
			DialogBox(hRes,MAKEINTRESOURCE(IDD_DIALOG1),hWnd,AboutDlgProc);
			return 0;
		case ID_FILE_EXIT:
			DestroyWindow(hWnd);
			break;
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		LoadString(hRes,IDS_MES,str,128);
		TextOut(hdc,10,100,str,lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

