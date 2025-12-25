#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("DevCap");

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

void DevCap()
{
	PRINTDLG pd;
	HDC hPrtdc;
	DEVMODE *dm;
	TCHAR szPrinter[128];
	HANDLE hPrinter;
	PRINTER_INFO_2 *pi;
	DWORD cbNeeded;
	TCHAR Mes[256];
	HDC hdc;
	int y=2;

	hdc=GetDC(hWndMain);

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
	if (hPrtdc == NULL) 
		return;

	// 프린터 이름 조사
	dm=(DEVMODE *)GlobalLock(pd.hDevMode);
	lstrcpy(szPrinter,(LPCTSTR)dm->dmDeviceName);
	GlobalUnlock(pd.hDevMode);
	wsprintf(Mes,"프린터 이름: %s ", szPrinter);
	TextOut(hdc,10,y++*20,Mes,lstrlen(Mes));

	// 화면 해상도 조사
	int xres, yres,xsize,ysize,xdpi,ydpi,numcolor,rastercap;
	xres = GetDeviceCaps(hPrtdc, HORZRES);
	yres = GetDeviceCaps(hPrtdc, VERTRES);
	xsize = GetDeviceCaps(hPrtdc, HORZSIZE);
	ysize = GetDeviceCaps(hPrtdc, VERTSIZE);
	xdpi = GetDeviceCaps(hPrtdc, LOGPIXELSX);
	ydpi = GetDeviceCaps(hPrtdc, LOGPIXELSY);
	numcolor = GetDeviceCaps(hPrtdc, NUMCOLORS);
	rastercap = GetDeviceCaps(hPrtdc, RASTERCAPS) & RC_STRETCHDIB;

	wsprintf(Mes,"X크기(픽셀)=%d, Y크기(픽셀)=%d",xres,yres);
	TextOut(hdc,10,y++*20,Mes,lstrlen(Mes));
	wsprintf(Mes,"X크기(밀리미터)=%d, Y크기(밀리미터)=%d",xsize,ysize);
	TextOut(hdc,10,y++*20,Mes,lstrlen(Mes));
	wsprintf(Mes,"X해상도=%d, Y해상도=%d",xdpi,ydpi);
	TextOut(hdc,10,y++*20,Mes,lstrlen(Mes));
	wsprintf(Mes,"색상수=%d, 비트맵 스트레칭=%s",
		numcolor,rastercap ? "가능":"불가능");
	TextOut(hdc,10,y++*20,Mes,lstrlen(Mes));
		
	// 여러 가지 능력을 조사한다.
	DWORD Collate, Copies, Duplex, Orientation;
	Collate=DeviceCapabilities(szPrinter,szPrinter,DC_COLLATE,NULL,NULL);
	Copies=DeviceCapabilities(szPrinter,szPrinter,DC_COPIES,NULL,NULL);
	Duplex=DeviceCapabilities(szPrinter,szPrinter,DC_DUPLEX,NULL,NULL);
	Orientation=DeviceCapabilities(szPrinter,szPrinter,DC_ORIENTATION,NULL,NULL);

	wsprintf(Mes,"Collate(한 부씩 인쇄)=%s, Copies(같은 페이지 최대 출력 매수)=%d",
		Collate ? "지원":"지원 안함",Copies);
	TextOut(hdc,10,y++*20,Mes,lstrlen(Mes));
	wsprintf(Mes,"Duplex(양면 인쇄)=%s, 가로찍기=%d도",
		Duplex ? "지원":"지원 안함",Orientation);
	TextOut(hdc,10,y++*20,Mes,lstrlen(Mes));

	// GetPrinter로 조사
	OpenPrinter(szPrinter,&hPrinter,NULL);
	GetPrinter(hPrinter,2,0,0,&cbNeeded);
	pi=(PRINTER_INFO_2 *)malloc(cbNeeded);
	GetPrinter(hPrinter,2,(LPBYTE)pi,cbNeeded,&cbNeeded);

	wsprintf(Mes,"프린터 이름: %s, 인쇄 프로세서 이름 : %s ", 
		pi->pPrinterName, pi->pPrintProcessor);
	TextOut(hdc,10,y++*20,Mes,lstrlen(Mes));

	free(pi);
	ClosePrinter(hPrinter);

	if (pd.hDevMode) GlobalFree(pd.hDevMode); 	
	if (pd.hDevNames) GlobalFree(pd.hDevNames);
	DeleteDC(hPrtdc);
	ReleaseDC(hWndMain,hdc);
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes="마우스 왼쪽 버튼을 누르시오";

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,10,10,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		DevCap();
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
