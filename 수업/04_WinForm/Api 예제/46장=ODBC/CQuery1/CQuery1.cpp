#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("CQuery1");

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

#include "CQuery.h"
CQuery qry;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR Name[21];
	int arTab[2]={200,400};
	TCHAR str[255];
	int y;

	switch (iMessage) {
	case WM_CREATE:
		qry.Connect(1,"cigarette.mdb");
		//qry.Connect(2,"FileMdb.dsn");
		//qry.Connect(3,"SQLCigar","sa","");
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		y=0;
		qry.Exec("select name,price,korean from tblCigar");
		while (qry.Fetch()!=SQL_NO_DATA) {
			qry.GetStr("name",Name);
			wsprintf(str,"이름:%s\t가격:%d\t국산:%s",Name,qry.GetInt("price"),
				(qry.GetInt("korean")==1 ? "예":"아니오"));
			TabbedTextOut(hdc,10,y*20,str,lstrlen(str),2,arTab,0);
			y++;
		}
		qry.Clear();
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
