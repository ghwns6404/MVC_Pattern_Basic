#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("SetOtherText");

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
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HWND hOther,hChild;
	LVITEM LI;
	TCHAR *Mes="왼쪽:리스트 박스에 문자열 추가, 오른쪽:리스트 컨트롤에 항목 추가";

	switch (iMessage) {
	case WM_LBUTTONDOWN:
		hOther=FindWindow("MyListBox",NULL);
		SendMessage(hOther,WM_SETTEXT,0,(LPARAM)"남의 타이틀 바");
		if (hOther) {
			hChild=GetWindow(hOther,GW_CHILD);
			SendMessage(hChild,LB_ADDSTRING,0,(LPARAM)"장난꾸러기");
		}
		return 0;
	case WM_RBUTTONDOWN:
		hOther=FindWindow("ListCtrl",NULL);
		if (hOther) {
			hChild=GetWindow(hOther,GW_CHILD);
			LI.mask=LVIF_TEXT;
			LI.iSubItem=0;
			LI.iItem=0;
			LI.pszText="욕심꾸러기";
			SendMessage(hChild, LVM_INSERTITEM, 0, (LPARAM)&LI);
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,10,10,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
