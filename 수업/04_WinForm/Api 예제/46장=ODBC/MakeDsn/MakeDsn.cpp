#include <windows.h>
#include "SHReg.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("MakeDsn");

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

// SQL 서버용의 DSN을 작성한다.
void MakeSQLDsn(HKEY key, TCHAR *Dsn, TCHAR *Server, TCHAR *DB)
{
	TCHAR szDriver[MAX_PATH];
	TCHAR szRegPath[MAX_PATH];

	// 드라이버의 경로와 DSN기록을 위한 레지스트리 경로를 조사한다.
	GetSystemDirectory(szDriver,MAX_PATH);
	lstrcat(szDriver,"\\sqlsvr32.dll");
	wsprintf(szRegPath,"Software\\Odbc\\Odbc.ini\\%s",Dsn);

	// DSN 이름을 등록한다.
	SHRegWriteString(key, "Software\\Odbc\\Odbc.ini\\Odbc Data Sources",
		Dsn,"SQL Server");

	// DSN 정보를 레지스트리에 기록한다.
	SHRegWriteString(key, szRegPath,"DataBase",DB);
	SHRegWriteString(key, szRegPath,"Description",DB);
	SHRegWriteString(key, szRegPath,"Driver",szDriver);
	SHRegWriteString(key, szRegPath,"Server",Server);
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes="왼쪽 마우스 버튼을 누르면 SQLCigar DSN을 생성합니다.";
	switch (iMessage) {
	case WM_LBUTTONDOWN:
		MakeSQLDsn(HKEY_CURRENT_USER, "SQLCigar","(local)","Cigarette");
		MessageBox(hWnd,"DSN을 생성했습니다","알림",MB_OK);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,50,50,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
