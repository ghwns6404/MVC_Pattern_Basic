#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("Mouse3");

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

HBITMAP hbit;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc, hMemDC;
	HBITMAP OldBitmap;
	static int x;
	static int y;
	static BOOL bNowDraw=FALSE;
	PAINTSTRUCT ps;
	switch (iMessage) {
	// 1024*768 크기의 메모리 비트맵을 만들고 초기화한다.
	case WM_CREATE:
		hdc=GetDC(hWnd);
		hbit=CreateCompatibleBitmap(hdc, 1024,768);
		hMemDC=CreateCompatibleDC(hdc);
		OldBitmap=(HBITMAP)SelectObject(hMemDC, hbit);
		SelectObject(hMemDC, GetStockObject(WHITE_PEN));
		Rectangle(hMemDC, 0,0,1024,768);
		SelectObject(hMemDC, OldBitmap);
		DeleteDC(hMemDC);
		ReleaseDC(hWnd, hdc);
		return 0;
	// 그리기를 시작한다.
	case WM_LBUTTONDOWN:
		x=LOWORD(lParam);
		y=HIWORD(lParam);
		bNowDraw=TRUE;
		return 0;
	// 화면과 메모리 비트맵으로 동시에 출력한다.
	case WM_MOUSEMOVE:
		if (bNowDraw==TRUE) {
			hdc=GetDC(hWnd);
			hMemDC=CreateCompatibleDC(hdc);
			OldBitmap=(HBITMAP)SelectObject(hMemDC, hbit);
			MoveToEx(hdc,x,y,NULL);
			MoveToEx(hMemDC,x,y,NULL);
			x=LOWORD(lParam);
			y=HIWORD(lParam);
			LineTo(hdc,x,y);
			LineTo(hMemDC,x,y);
			SelectObject(hMemDC, OldBitmap);
			DeleteDC(hMemDC);
			ReleaseDC(hWnd,hdc);
		}
		return 0;
	case WM_LBUTTONUP:
		bNowDraw=FALSE;
		return 0;
	// 메모리 비트맵에 저장된 그림을 화면으로 고속 전송한다.
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		hMemDC=CreateCompatibleDC(hdc);
		OldBitmap=(HBITMAP)SelectObject(hMemDC, hbit);
		BitBlt(hdc,0,0,1024,768,hMemDC,0,0,SRCCOPY);
		SelectObject(hMemDC, OldBitmap);
		DeleteDC(hMemDC);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		DeleteObject(hbit);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
