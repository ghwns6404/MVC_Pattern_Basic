#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("DevMode");

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

BOOL MyGetDefaultPrinter(LPSTR DefPrinter)
{
	OSVERSIONINFO osv;
	DWORD dwNeed;
	TCHAR Buffer[255];

	osv.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	GetVersionEx(&osv);

	// 2000 이후
	if (osv.dwMajorVersion >= 5) {
		dwNeed=128;
		if (GetDefaultPrinter(DefPrinter,&dwNeed)==0)
			return FALSE;
	// 95/98, NT 4.0
	} else {
		if (GetProfileString("windows","device",",,,",Buffer,255) <= 0)
			return FALSE;
		strtok(Buffer,",");
		lstrcpy(DefPrinter,Buffer);
	}

	return TRUE;
}

void DisplayDevMode2()
{
	TCHAR szPrinter[128];
	DWORD dwPrinter=128;
	HANDLE hPrinter;
	DEVMODE *dm;
	LONG ret;
	HDC hdc;
	int y=2;
	TCHAR Mes[256];

	hdc=GetDC(hWndMain);

	if (MyGetDefaultPrinter(szPrinter) == FALSE) {
		return;
	}
	OpenPrinter(szPrinter,&hPrinter,NULL);

	ret=DocumentProperties(hWndMain, hPrinter, szPrinter, NULL, NULL, 0);
	dm=(DEVMODE *)malloc(ret);
	DocumentProperties(hWndMain, hPrinter, szPrinter, dm, NULL, DM_OUT_BUFFER);

	// 초기화 정보 구함
	wsprintf(Mes,"기본 프린터 이름=%s",dm->dmDeviceName);
	TextOut(hdc,10,y++*20,Mes,lstrlen(Mes));
	wsprintf(Mes,"드라이버 버전=%d", dm->dmDriverVersion);
	TextOut(hdc,10,y++*20,Mes,lstrlen(Mes));
	wsprintf(Mes,"공용 데이터의 크기=%d, 드라이버 고유 데이터의 크기=%d", 
		dm->dmSize, dm->dmDriverExtra);
	TextOut(hdc,10,y++*20,Mes,lstrlen(Mes));
	wsprintf(Mes,"용지의 종류=%d, 가로=%d, 세로=%d, 용지의 방향=%s", 
		dm->dmPaperSize, dm->dmPaperWidth, dm->dmPaperLength, 
		dm->dmOrientation == DMORIENT_PORTRAIT  ? "세로":"가로");
	TextOut(hdc,10,y++*20,Mes,lstrlen(Mes));
	wsprintf(Mes,"X해상도=%d, Y해상도=%d", dm->dmPrintQuality, dm->dmYResolution);
	TextOut(hdc,10,y++*20,Mes,lstrlen(Mes));

	free(dm);
	ClosePrinter(hPrinter);
	ReleaseDC(hWndMain,hdc);
}

void DisplayDevMode1()
{
	PRINTDLG pd;
	HDC hPrtdc;
	DEVMODE *dm;
	TCHAR szPrinter[128];
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

	// DEVMODE의 번지를 구한다.
	dm=(DEVMODE *)GlobalLock(pd.hDevMode);
	lstrcpy(szPrinter,(LPCTSTR)dm->dmDeviceName);

	// 초기화 정보 구함
	wsprintf(Mes,"선택한 프린터 이름=%s",dm->dmDeviceName);
	TextOut(hdc,10,y++*20,Mes,lstrlen(Mes));
	wsprintf(Mes,"드라이버 버전=%d", dm->dmDriverVersion);
	TextOut(hdc,10,y++*20,Mes,lstrlen(Mes));
	wsprintf(Mes,"공용 데이터의 크기=%d, 드라이버 고유 데이터의 크기=%d", 
		dm->dmSize, dm->dmDriverExtra);
	TextOut(hdc,10,y++*20,Mes,lstrlen(Mes));
	wsprintf(Mes,"용지의 종류=%d, 가로=%d, 세로=%d, 용지의 방향=%s", 
		dm->dmPaperSize, dm->dmPaperWidth, dm->dmPaperLength, 
		dm->dmOrientation == DMORIENT_PORTRAIT  ? "세로":"가로");
	TextOut(hdc,10,y++*20,Mes,lstrlen(Mes));
	wsprintf(Mes,"X해상도=%d, Y해상도=%d", dm->dmPrintQuality, dm->dmYResolution);
	TextOut(hdc,10,y++*20,Mes,lstrlen(Mes));

	GlobalUnlock(pd.hDevMode);
	ReleaseDC(hWndMain,hdc);
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes="왼쪽 버튼=프린터 선택, 오른쪽 버튼=기본 프린터의 정보";

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		return 0;
	case WM_LBUTTONDOWN:
		InvalidateRect(hWnd,NULL,TRUE);
		UpdateWindow(hWnd);
		DisplayDevMode1();
		return 0;
	case WM_RBUTTONDOWN:
		InvalidateRect(hWnd,NULL,TRUE);
		UpdateWindow(hWnd);
		DisplayDevMode2();
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
