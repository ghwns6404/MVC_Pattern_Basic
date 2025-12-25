#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("Registry");

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
	RECT rt;
	HKEY key;
	DWORD dwDisp;
	DWORD Size;
	switch (iMessage) {
	case WM_CREATE:
		RegCreateKeyEx(HKEY_CURRENT_USER, 
			"Software\\MiyoungSoft\\RegiTest\\Position",0,NULL,
			REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,NULL,&key,&dwDisp);
		Size=sizeof(LONG);
		if (RegQueryValueEx(key, "Left", 0, NULL,(LPBYTE)&rt.left, &Size)
			!=ERROR_SUCCESS)
			rt.left=0;
		Size=sizeof(LONG);
		if (RegQueryValueEx(key, "Top", 0, NULL,(LPBYTE)&rt.top, &Size)
			!=ERROR_SUCCESS)
			rt.top=0;
		Size=sizeof(LONG);
		if (RegQueryValueEx(key, "Right", 0, NULL,(LPBYTE)&rt.right, &Size)
			!=ERROR_SUCCESS)
			rt.right=100;
		Size=sizeof(LONG);
		if (RegQueryValueEx(key, "Bottom", 0, NULL,(LPBYTE)&rt.bottom, &Size)
			!=ERROR_SUCCESS)
			rt.bottom=100;;
		RegCloseKey(key);
		MoveWindow(hWnd, rt.left, rt.top, rt.right-rt.left, 
			rt.bottom-rt.top, TRUE);
		return 0;
	case WM_DESTROY:
		RegCreateKeyEx(HKEY_CURRENT_USER, 
			"Software\\MiyoungSoft\\RegiTest\\Position",0,NULL,
			REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,NULL,&key,&dwDisp);
		GetWindowRect(hWnd, &rt);
		RegSetValueEx(key, "Left",0,REG_DWORD,(LPBYTE)&rt.left,sizeof(LONG));
		RegSetValueEx(key, "Top",0,REG_DWORD,(LPBYTE)&rt.top,sizeof(LONG));
		RegSetValueEx(key, "Right",0,REG_DWORD,(LPBYTE)&rt.right,sizeof(LONG));
		RegSetValueEx(key, "Bottom",0,REG_DWORD,(LPBYTE)&rt.bottom,sizeof(LONG));
		RegCloseKey(key);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

