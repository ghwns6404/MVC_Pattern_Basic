#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("Meta16");

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
	HDC hdcMeta;
	HMETAFILE hMFile;
	switch (iMessage) {
	case WM_LBUTTONDOWN:
		hdcMeta=CreateMetaFile("Test.wmf");
		Ellipse(hdcMeta,10,10,100,100);
		Rectangle(hdcMeta,5,105,105,120);
		hMFile=CloseMetaFile(hdcMeta);
		DeleteMetaFile(hMFile);
		MessageBox(hWnd, "Meta File Created","Meta",MB_OK);
		return 0;
	case WM_RBUTTONDOWN:
		hdc=GetDC(hWnd);
		hMFile=GetMetaFile("Test.wmf");
		if (hMFile == NULL) {
			MessageBox(hWnd, "File Not Found", "Meta", MB_OK);
		} else {
			PlayMetaFile(hdc,hMFile);
		}
		DeleteMetaFile(hMFile);
		ReleaseDC(hWnd,hdc);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

