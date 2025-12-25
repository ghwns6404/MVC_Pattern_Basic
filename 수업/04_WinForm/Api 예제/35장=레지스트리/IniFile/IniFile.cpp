#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("IniFile");

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

/* 별도의 파일에 저장하기
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	RECT rt;
	TCHAR buf[10];
	switch (iMessage) {
	case WM_CREATE:
		rt.left=GetPrivateProfileInt("Position", "Left", 0, "IniFile.ini");
		rt.top=GetPrivateProfileInt("Position", "Top", 0, "IniFile.ini");
		rt.right=GetPrivateProfileInt("Position", "Right", 100, "IniFile.ini");
		rt.bottom=GetPrivateProfileInt("Position", "Bottom", 100, "IniFile.ini");
		MoveWindow(hWnd, rt.left, rt.top, rt.right-rt.left, 
			rt.bottom-rt.top, TRUE);
		return 0;
	case WM_DESTROY:
		GetWindowRect(hWnd, &rt);
		itoa(rt.left, buf, 10);
		WritePrivateProfileString("Position", "Left", buf, "IniFile.ini");
		itoa(rt.top, buf, 10);
		WritePrivateProfileString("Position", "Top", buf, "IniFile.ini");
		itoa(rt.right, buf, 10);
		WritePrivateProfileString("Position", "Right", buf, "IniFile.ini");
		itoa(rt.bottom, buf, 10);
		WritePrivateProfileString("Position", "Bottom", buf, "IniFile.ini");
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
//*/
//* Win.Ini 파일에 저장하기
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	RECT rt;
	TCHAR buf[10];
	switch (iMessage) {
	case WM_CREATE:
		rt.left=GetProfileInt("Position", "Left", 0);
		rt.top=GetProfileInt("Position", "Top", 0);
		rt.right=GetProfileInt("Position", "Right", 100);
		rt.bottom=GetProfileInt("Position", "Bottom", 100);
		MoveWindow(hWnd, rt.left, rt.top, rt.right-rt.left, 
			rt.bottom-rt.top, TRUE);
		return 0;
	case WM_DESTROY:
		GetWindowRect(hWnd, &rt);
		itoa(rt.left, buf, 10);
		WriteProfileString("Position", "Left", buf);
		itoa(rt.top, buf, 10);
		WriteProfileString("Position", "Top", buf);
		itoa(rt.right, buf, 10);
		WriteProfileString("Position", "Right", buf);
		itoa(rt.bottom, buf, 10);
		WriteProfileString("Position", "Bottom", buf);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
//*/
