#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("PrintMulti");

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
	TCHAR str[128];
	int i;
	SIZE sz;
	int nPage;
	int nFirstPage,nFinalPage;
	int LinePerPage;

	// 프린터에 관한 정보를 구하고 DC를 만든다.
	memset(&pd,0,sizeof(PRINTDLG));
	pd.lStructSize=sizeof(PRINTDLG);
	pd.Flags=PD_RETURNDC;
	pd.hwndOwner=hWndMain;
	pd.nFromPage=1;
	pd.nToPage=5;
	pd.nMinPage=1;
	pd.nMaxPage=5;	// 최대 5페이지까지
	pd.nCopies=1;
	PrintDlg(&pd);
	hPrtdc=pd.hDC;
	if (hPrtdc == NULL) return;

	doc.cbSize=sizeof(DOCINFO);
	doc.lpszDocName="Test Document";
	doc.lpszOutput=NULL;
	doc.lpszDatatype=NULL;
	doc.fwType=0;
	Result=StartDoc(hPrtdc, &doc);
	if (Result <= 0) goto end;

	xpage = GetDeviceCaps(hPrtdc, HORZRES);
	ypage = GetDeviceCaps(hPrtdc, VERTRES);
	dpiX = GetDeviceCaps(hPrtdc, LOGPIXELSX);
	dpiY = GetDeviceCaps(hPrtdc, LOGPIXELSY);

	// 사용자의 옵션 선택에 따라 마지막 페이지를 계산한다.
	if (pd.Flags & PD_PAGENUMS) {
		nFirstPage=pd.nFromPage;
		nFinalPage=pd.nToPage;
	} else {
		nFirstPage=pd.nMinPage;
		nFinalPage=pd.nMaxPage;
	}
	for (nPage=nFirstPage;nPage<=nFinalPage;nPage++) {
		Result=StartPage(hPrtdc);
		if (Result <= 0) goto end;
		Rectangle(hPrtdc,0,0,xpage,ypage);
		MyFont=CreateFont(10*dpiY/72,0,0,0,FW_NORMAL, FALSE, FALSE, FALSE,
			HANGEUL_CHARSET, 3,2,1,VARIABLE_PITCH | FF_ROMAN, "궁서");
		OldFont=(HFONT)SelectObject(hPrtdc, MyFont);
		GetTextExtentPoint32(hPrtdc,"한",2,&sz);
/* 다섯 페이지 인쇄
		for (i=0;i<=30;i++) {
			wsprintf(str,"%d번째 페이지의 %d번째 줄입니다. ABCDEFGHIJKLMNOPQRSTUVWXYZ "
				"1234567 대한민국",nPage,i);
			TextOut(hPrtdc,10,int(sz.cy*1.5)*i+10,str,lstrlen(str));
		}
//*/
//* 줄 수 계산 및 꼬리말 인쇄
		// 한 페이지에 들어가는 줄 수 계산
		LinePerPage=(int)((ypage-dpiY)/(sz.cy*1.5));
		for (i=0;i<=LinePerPage;i++) {
			wsprintf(str,"%d번째 페이지의 %d번째 줄입니다. ABCDEFGHIJKLMNOPQRSTUVWXYZ "
				"1234567 대한민국",nPage,i);
			TextOut(hPrtdc,10,int(sz.cy*1.5)*i+10,str,lstrlen(str));
		}

		// 꼬리말 출력
		MoveToEx(hPrtdc,0,ypage-dpiY/2,NULL);
		LineTo(hPrtdc,dpiY*5,ypage-dpiY/2);
		wsprintf(str,"인쇄 테스트 - %d 페이지",nPage);
		TextOut(hPrtdc,0,int(ypage-dpiY/2*0.9),str,lstrlen(str));
//*/
		SelectObject(hPrtdc, OldFont);
		DeleteObject(MyFont);
		Result=EndPage(hPrtdc);
		if (Result <= 0) goto end;
	}
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

