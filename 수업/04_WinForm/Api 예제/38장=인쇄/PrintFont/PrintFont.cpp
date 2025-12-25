#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("PrintFont");

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

void Print()
{
	HDC hPrtdc;
	int xpage, ypage;
	DOCINFO doc;
	int Result;
	PRINTDLG pd;
	HFONT MyFont, OldFont;
	int dpiX, dpiY;

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
	xpage = GetDeviceCaps(hPrtdc, HORZRES);
	ypage = GetDeviceCaps(hPrtdc, VERTRES);
	Rectangle(hPrtdc,0,0,xpage,ypage);

	// 프린터의 해상도를 구한다.
	dpiX = GetDeviceCaps(hPrtdc, LOGPIXELSX);
	dpiY = GetDeviceCaps(hPrtdc, LOGPIXELSY);
	SetTextAlign(hPrtdc,TA_CENTER);

	// 1인치=1/72포인트. 그러므로 n포인트 크기=n*1/72*dpi가 된다.
	MyFont=CreateFont(20*dpiY/72,0,0,0,FW_NORMAL, FALSE, FALSE, FALSE,
		HANGEUL_CHARSET, 3,2,1,VARIABLE_PITCH | FF_ROMAN, "궁서");
	OldFont=(HFONT)SelectObject(hPrtdc, MyFont);
	TextOut(hPrtdc,xpage/2,ypage/2,"Test String 대한민국",20);
	SelectObject(hPrtdc, OldFont);
	DeleteObject(MyFont);

	// 출력을 종료한다.
	Result=EndPage(hPrtdc);
	if (Result <= 0) goto end;

	// 인쇄 작업을 끝낸다.
	Result=EndDoc(hPrtdc);
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
	TCHAR *Mes="마우스 왼쪽 버튼을 누르면 인쇄를 시작합니다";

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		return 0;
	case WM_LBUTTONDOWN:
		Print();
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd,&ps);
		TextOut(hdc,100,100,Mes,lstrlen(Mes));
		EndPaint(hWnd,&ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
