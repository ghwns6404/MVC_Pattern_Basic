#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("Meta");

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
	HENHMETAFILE hMFile;
	RECT rt;
	switch (iMessage) {
	case WM_LBUTTONDOWN:
		hdc=GetDC(hWnd);
		hdcMeta=CreateEnhMetaFile(hdc,"TestEnh.emf",NULL,
			"my program\0Test Enh Meta File\0");
		Ellipse(hdcMeta,10,10,100,100);
		Rectangle(hdcMeta,5,105,105,120);
		hMFile=CloseEnhMetaFile(hdcMeta);
		DeleteEnhMetaFile(hMFile);
		ReleaseDC(hWnd, hdc);
		MessageBox(hWnd, "Meta File Created","Meta",MB_OK);
		return 0;
	case WM_RBUTTONDOWN:
		hdc=GetDC(hWnd);
		hMFile=GetEnhMetaFile("TestEnh.emf");
		if (hMFile == NULL) {
			MessageBox(hWnd, "File Not Found", "Meta", MB_OK);
		} else {
			rt.left=LOWORD(lParam);
			rt.right=rt.left+100;
			rt.top=HIWORD(lParam);
			rt.bottom=rt.top+100;
			PlayEnhMetaFile(hdc,hMFile,&rt);
		}
		DeleteEnhMetaFile(hMFile);
		ReleaseDC(hWnd,hdc);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
