#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("DTP");

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
HWND hDtp;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	INITCOMMONCONTROLSEX icex;
	SYSTEMTIME tm;
	TCHAR str[255];

	switch (iMessage) {
	case WM_CREATE:
		icex.dwSize=sizeof(icex);
		icex.dwICC=ICC_DATE_CLASSES;
		InitCommonControlsEx(&icex);
		hDtp=CreateWindow(DATETIMEPICK_CLASS, "DTP",WS_BORDER | WS_CHILD | WS_VISIBLE |
			DTS_SHORTDATEFORMAT,
			50,50,200,25,hWnd,NULL,g_hInst,NULL);
		return 0;
	case WM_LBUTTONDOWN:
		DateTime_GetSystemtime(hDtp,&tm);
		wsprintf(str,"%d년 %d월 %d일을 선택하셨습니다",tm.wYear, tm.wMonth, tm.wDay);
		MessageBox(hWnd, str, "알림",MB_OK);
		return 0;
	case WM_RBUTTONDOWN:
		memset(&tm,0,sizeof(SYSTEMTIME));
		tm.wYear=1970;
		tm.wMonth=6;
		tm.wDay=29;
		DateTime_SetSystemtime(hDtp,GDT_VALID,&tm);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

