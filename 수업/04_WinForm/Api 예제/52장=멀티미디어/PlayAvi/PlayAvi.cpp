#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("PlayAvi");

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

#include <vfw.h>
HWND hWndAVI=0;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	TCHAR str[128];
	switch (iMessage) {
	case WM_LBUTTONDOWN:
		if (hWndAVI) {
			MCIWndClose(hWndAVI);
			MCIWndDestroy(hWndAVI);
			hWndAVI=0;
		}
		hWndAVI=MCIWndCreate(hWnd, g_hInst,MCIWNDF_NOTIFYMODE | MCIWNDF_NOTIFYPOS,
			"ballet.avi");
		if (hWndAVI) {
			MCIWndPlay(hWndAVI);
		}
		return 0;
	case MCIWNDM_NOTIFYMODE:
		switch (lParam) {
		case MCI_MODE_PLAY:
			MessageBox(hWnd, "Play","MCI",MB_OK);
			break;
		case MCI_MODE_STOP:
			MessageBox(hWnd, "Stop","MCI",MB_OK);
			break;
		}
		return 0;
	case MCIWNDM_NOTIFYPOS:
		hdc=GetDC(hWnd);
		wsprintf(str, "현재 위치:%ld",lParam);
		TextOut(hdc,0,300,str,lstrlen(str));
		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

