#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("CRegiTest");

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

#include "CRegi.h"
CRegi R("Software\\MiyoungSoft\\CRegiTest");
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rt;
	static HWND hEdit;
	TCHAR str[256],temp[256];
	switch (iMessage) {
	case WM_CREATE:
		rt.left=R.ReadInt("Position","Left",0);
		rt.top=R.ReadInt("Position","Top",0);
		rt.right=R.ReadInt("Position","Right",300);
		rt.bottom=R.ReadInt("Position","Bottom",200);
		MoveWindow(hWnd, rt.left, rt.top, rt.right-rt.left, 
			rt.bottom-rt.top, TRUE);
		hEdit=CreateWindow("edit",NULL,WS_CHILD | WS_VISIBLE | WS_BORDER | 
			ES_AUTOHSCROLL,10,10,200,25,hWnd,(HMENU)100,g_hInst,NULL);
		R.ReadString("Edit","Str","문자열",str,256);
		SetWindowText(hEdit,str);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		R.ReadString(SHCR,".jpg",NULL,"",temp,256);
		wsprintf(str,"JPG 확장자의 기본값 = %s",temp);
		TextOut(hdc,10,50,str,lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		if (R.DeleteKey(SHCU,"Software\\MiyoungSoft\\CRegiTest")) {
			MessageBox(hWnd,"이 프로그램의 레지스트리 정보를 삭제했습니다.",
				"알림",MB_OK);
		}
		return 0;
	case WM_DESTROY:
		GetWindowRect(hWnd, &rt);
		R.WriteInt("Position","Left",rt.left);
		R.WriteInt("Position","Top",rt.top);
		R.WriteInt("Position","Right",rt.right);
		R.WriteInt("Position","Bottom",rt.bottom);
		GetWindowText(hEdit,str,256);
		R.WriteString("Edit","Str",str);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
