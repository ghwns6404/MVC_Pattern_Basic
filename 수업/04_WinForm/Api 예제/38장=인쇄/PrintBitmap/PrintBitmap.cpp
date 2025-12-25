#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("PrintBitmap");

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

void DrawBitmap(HDC hdc,int x,int y,HBITMAP hBit)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx,by;
	BITMAP bit;

	MemDC=CreateCompatibleDC(hdc);
	OldBitmap=(HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit,sizeof(BITMAP),&bit);
	bx=bit.bmWidth;
	by=bit.bmHeight;

	BitBlt(hdc,x,y,bx,by,MemDC,0,0,SRCCOPY);

	SelectObject(MemDC,OldBitmap);
	DeleteDC(MemDC);
}

#include "resource.h"
void Print()
{
	HDC hPrtdc;
	int xpage, ypage;
	DOCINFO doc;
	int Result;
	PRINTDLG pd;
	int dpiX, dpiY;
	HDC MemDC;
	HBITMAP hbit,OldBitmap;
	BITMAP bit;
	int bx,by;

	// 프린터에 관한 정보를 구하고 DC를 만든다.
	memset(&pd,0,sizeof(PRINTDLG));
	pd.lStructSize=sizeof(PRINTDLG);
	pd.Flags=PD_RETURNDC;
	pd.hwndOwner=hWndMain;
	pd.nFromPage=1;
	pd.nToPage=1;
	pd.nMinPage=1;
	pd.nMaxPage=1;
	pd.nCopies=1;
	PrintDlg(&pd);
	hPrtdc=pd.hDC;
	if (hPrtdc == NULL) return;

	// 인쇄 작업을 시작한다.
	doc.cbSize=sizeof(DOCINFO);
	doc.lpszDocName="Test Document";
	doc.lpszOutput=NULL;
	doc.lpszDatatype=NULL;
	doc.fwType=0;
	Result=StartDoc(hPrtdc, &doc);
	if (Result <= 0) goto end;
	Result=StartPage(hPrtdc);
	if (Result <= 0) goto end;

	// 출력을 보낸다.
	xpage = GetDeviceCaps(hPrtdc, HORZRES);
	ypage = GetDeviceCaps(hPrtdc, VERTRES);

	// 프린터의 해상도를 구한다.
	dpiX = GetDeviceCaps(hPrtdc, LOGPIXELSX);
	dpiY = GetDeviceCaps(hPrtdc, LOGPIXELSY);
	Rectangle(hPrtdc,0,0,xpage,ypage);

	// 비트맵을 출력한다.
	Result=GetDeviceCaps(hPrtdc, RASTERCAPS) & RC_BITBLT;
	if (!Result) goto end;
	hbit=(HBITMAP)LoadImage(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1),
		IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION);
	GetObject(hbit,sizeof(BITMAP),&bit);
	bx=bit.bmWidth;
	by=bit.bmHeight;
	MemDC=CreateCompatibleDC(hPrtdc);
	OldBitmap=(HBITMAP)SelectObject(MemDC, hbit);
	StretchBlt(hPrtdc,dpiX,dpiY,4*dpiX,4*by/bx*dpiY,MemDC,0,0,bx,by,SRCCOPY);
	SelectObject(MemDC, OldBitmap);
	DeleteObject(hbit);
	DeleteDC(MemDC);

	// 출력을 종료한다.
	Result=EndPage(hPrtdc);
	if (Result <= 0) goto end;
	// 인쇄 작업을 끝낸다.
	Result=EndDoc(hPrtdc);
	if (Result <= 0) goto end;
end:
	if (pd.hDevMode) GlobalFree(pd.hDevMode);
	if (pd.hDevNames) GlobalFree(pd.hDevNames);
	DeleteDC(hPrtdc);
	return;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static HBITMAP hbit;
	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		hbit=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		return 0;
	case WM_LBUTTONDOWN:
		Print();
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd,&ps);
		DrawBitmap(hdc,0,0,hbit);
		EndPaint(hWnd,&ps);
		return 0;
	case WM_DESTROY:
		DeleteObject(hbit);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

