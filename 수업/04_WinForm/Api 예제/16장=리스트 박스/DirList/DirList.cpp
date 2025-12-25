#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("DirList");

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

#define ID_LISTBOX 100
HWND hList;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	int i;
	TCHAR File[MAX_PATH];
	TCHAR Mes[256];
	BOOL dir;
	switch (iMessage) {
	case WM_CREATE:
		hList=CreateWindow("listbox",NULL,WS_CHILD | WS_VISIBLE | WS_BORDER |
			WS_VSCROLL | LBS_NOTIFY,
			10,10,300,200,hWnd,(HMENU)ID_LISTBOX,g_hInst,NULL);
		SendMessage(hList, LB_DIR, (WPARAM)(DDL_ARCHIVE | 
			DDL_DIRECTORY | DDL_DRIVES | DDL_READWRITE),(LPARAM)"*.*");
/*		
		lstrcpy(File,"*.*");
		DlgDirList(hWnd, File, ID_LISTBOX, 0, (DDL_ARCHIVE | 
			DDL_DIRECTORY | DDL_DRIVES | DDL_READWRITE));
//*/
		return 0;
	case WM_LBUTTONDOWN:
		dir=DlgDirSelectEx(hWnd,File,MAX_PATH,ID_LISTBOX);
		if (lstrlen(File)!=0) {
			if (dir) {
				wsprintf(Mes, "%s 디렉토리를 선택했습니다", File);
			} else {
				wsprintf(Mes, "%s 파일을 선택했습니다", File);
			}
			MessageBox(hWnd,Mes,"알림",MB_OK);
		}
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_LISTBOX:
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				i=SendMessage(hList, LB_GETCURSEL,0,0);
				SendMessage(hList, LB_GETTEXT, i, (LPARAM)File);
				SetWindowText(hWnd, File);
				break;
			}
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
