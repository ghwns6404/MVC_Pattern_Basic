#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("LandPort1");

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

void Print(int Orient)
{
	PRINTDLG pd;
	HDC hPrtdc;
	DEVMODE *dm;
	int xpage, ypage;
	int dpiX, dpiY;
	HPEN hPen, OldPen;
	HFONT MyFont, OldFont;
	DOCINFO doc;
	int Result;
	TCHAR *str="프린터 테스트";

	memset(&pd,0,sizeof(PRINTDLG));
	pd.lStructSize=sizeof(PRINTDLG);
	pd.Flags=PD_RETURNDC;
	pd.hwndOwner=hWndMain;
	pd.nFromPage=1;
	pd.nToPage=1;
	pd.nMinPage=1;
	pd.nMaxPage=1;
	pd.nCopies=1;

	pd.hDevMode=GlobalAlloc(GHND,sizeof(DEVMODE));
	dm=(DEVMODE *)GlobalLock(pd.hDevMode);
	dm->dmSize=sizeof(DEVMODE);
	dm->dmFields=DM_ORIENTATION;
	dm->dmOrientation=Orient;
	GlobalUnlock(pd.hDevMode);

	PrintDlg(&pd);
	hPrtdc=pd.hDC;
	if (hPrtdc == NULL) 
		return;

/*
	// 대화상자를 닫은 후에 강제로 용지 방향을 변경한다.
	dm=(DEVMODE *)GlobalLock(pd.hDevMode);
	dm->dmFields=DM_ORIENTATION;
	dm->dmOrientation=Orient;
	ResetDC(hPrtdc,dm);
	GlobalUnlock(pd.hDevMode);
//*/

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

	// 프린터의 해상도를 구한다.
	dpiX = GetDeviceCaps(hPrtdc, LOGPIXELSX);
	dpiY = GetDeviceCaps(hPrtdc, LOGPIXELSY);
	xpage = GetDeviceCaps(hPrtdc, HORZRES);
	ypage = GetDeviceCaps(hPrtdc, VERTRES);

	// 사각형을 그린다.
	hPen=CreatePen(PS_SOLID,dpiY/10,RGB(0,0,255));
	OldPen=(HPEN)SelectObject(hPrtdc,hPen);
	Rectangle(hPrtdc,200,200,xpage-200,ypage-200);
	DeleteObject(SelectObject(hPrtdc,OldPen));

	// 1인치=1/72포인트. 그러므로 n포인트 크기=n*1/72*dpi가 된다.
	MyFont=CreateFont(50*dpiY/72,0,0,0,FW_NORMAL, FALSE, FALSE, FALSE,
		HANGEUL_CHARSET, 3,2,1,VARIABLE_PITCH | FF_ROMAN, "궁서");
	OldFont=(HFONT)SelectObject(hPrtdc, MyFont);
	SetTextAlign(hPrtdc,TA_CENTER);
	TextOut(hPrtdc,xpage/2,ypage/2,str,lstrlen(str));
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
	SetCursor(LoadCursor(NULL,IDC_ARROW));
	return;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes="왼쪽 = 세로 찍기, 오른쪽 = 가로찍기";

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		return 0;
	case WM_LBUTTONDOWN:
		Print(DMORIENT_PORTRAIT);
		return 0;
	case WM_RBUTTONDOWN:
		Print(DMORIENT_LANDSCAPE);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,10,10,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
