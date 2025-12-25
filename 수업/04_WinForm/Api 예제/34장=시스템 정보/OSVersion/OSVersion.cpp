#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("OSVersion");

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

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	OSVERSIONINFO osv;
	TCHAR str[255];
	TCHAR Plat[255];
	switch (iMessage) {
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		osv.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
		GetVersionEx(&osv);
		wsprintf(str,"운영체제 버전 = %d.%d, 빌더 넘버=%d",
			osv.dwMajorVersion, osv.dwMinorVersion, osv.dwBuildNumber);
		TextOut(hdc,50,50,str,lstrlen(str));
		switch (osv.dwPlatformId) {
		case VER_PLATFORM_WIN32s:
			lstrcpy(Plat,"윈도우즈 3.1의 Win32s");
			break;
		case VER_PLATFORM_WIN32_WINDOWS:
			lstrcpy(Plat,"윈도우즈 95/98");
			break;
		case VER_PLATFORM_WIN32_NT:
			lstrcpy(Plat,"윈도우즈 NT, 2000");
			break;
		}
		
		wsprintf(str,"플랫폼=%s, 서비스팩=%s",Plat,osv.szCSDVersion);
		TextOut(hdc,50,70,str,lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}


