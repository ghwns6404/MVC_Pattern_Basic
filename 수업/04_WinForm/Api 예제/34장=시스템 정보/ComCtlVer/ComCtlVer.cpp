#include <windows.h>
#include <commctrl.h>
#include <shlwapi.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ComCtlVer");

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

DWORD MyDllGetVersion(LPCTSTR dll)
{
	HINSTANCE hInst;
	DLLVERSIONINFO dvi;
	DWORD Ver=0;
	DLLGETVERSIONPROC pFunc;
	HRESULT hr;

	hInst=LoadLibrary(dll);
	if (hInst == NULL)
		return Ver;

	pFunc=(DLLGETVERSIONPROC)GetProcAddress(hInst,"DllGetVersion");
	if (pFunc != NULL) {
		memset(&dvi,0,sizeof(DLLVERSIONINFO));
		dvi.cbSize=sizeof(DLLVERSIONINFO);

		hr=(*pFunc)(&dvi);
		if (SUCCEEDED(hr)) {
			Ver=MAKELONG(dvi.dwMinorVersion, dvi.dwMajorVersion);
		}
	}
	FreeLibrary(hInst);
	return Ver;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	DWORD Ver;
	TCHAR str[255];

	switch (iMessage) {
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		Ver=MyDllGetVersion("ComCtl32.dll");
		wsprintf(str,"공통 컨트롤 버전 = %d.%d",HIWORD(Ver), LOWORD(Ver));
		TextOut(hdc,50,50,str,lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

