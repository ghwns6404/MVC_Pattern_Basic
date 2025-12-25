#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("EnumPrinters");

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

void EnumPrintObject()
{
	PRINTER_INFO_1 *pi1;
	PRINTER_INFO_2 *pi2;
	PRINTER_INFO_4 *pi4;
	DWORD cbNeed, cbReturn;
	UINT i;
	HDC hdc;
	int y=2;
	TCHAR Mes[256];

	hdc=GetDC(hWndMain);

	// 로컬 프린터 열거 - 레지스트리에서 얻음
	lstrcpy(Mes,"로컬 프린터 열거 - 레지스트리");
	TextOut(hdc,10,y++*20,Mes,lstrlen(Mes));
	EnumPrinters(PRINTER_ENUM_LOCAL, NULL, 4, NULL, 0, &cbNeed, &cbReturn);
	pi4=(PRINTER_INFO_4 *)malloc(cbNeed);
	EnumPrinters(PRINTER_ENUM_LOCAL, NULL, 4, (PBYTE)pi4, cbNeed, &cbNeed, &cbReturn);
	for (i=0;i<cbReturn;i++) {
		wsprintf(Mes,"프린터 이름: %s, 종류:%s ", pi4[i].pPrinterName, 
			pi4[i].Attributes==PRINTER_ATTRIBUTE_LOCAL ? "로컬":"네트워크");
		TextOut(hdc,10,y++*20,Mes,lstrlen(Mes));
	}
	free(pi4);y++;

	// 로컬 프린터 열거 - 실제 프린터에서 구함
	lstrcpy(Mes,"로컬 프린터 열거 - 실제 프린터에서 구함");
	TextOut(hdc,10,y++*20,Mes,lstrlen(Mes));
	EnumPrinters(PRINTER_ENUM_LOCAL, NULL, 2, NULL, 0, &cbNeed, &cbReturn);
	pi2=(PRINTER_INFO_2 *)malloc(cbNeed);
	EnumPrinters(PRINTER_ENUM_LOCAL, NULL, 2, (PBYTE)pi2, cbNeed, &cbNeed, &cbReturn);
	for (i=0;i<cbReturn;i++) {
		wsprintf(Mes,"프린터 이름: %s, 포트:%s, 용지:%d ", pi2[i].pPrinterName, 
			pi2[i].pPortName, pi2[i].pDevMode->dmPaperSize);
		TextOut(hdc,10,y++*20,Mes,lstrlen(Mes));
	}
	free(pi2);y++;

	// 인쇄 프로바이더 열거
	lstrcpy(Mes,"인쇄 프로바이더 열거");
	TextOut(hdc,10,y++*20,Mes,lstrlen(Mes));
	EnumPrinters(PRINTER_ENUM_NAME, NULL, 1, NULL, 0, &cbNeed, &cbReturn);
	pi1=(PRINTER_INFO_1 *)malloc(cbNeed);
	EnumPrinters(PRINTER_ENUM_NAME, NULL, 1, (PBYTE)pi1, cbNeed, &cbNeed, &cbReturn);
	for (i=0;i<cbReturn;i++) {
		wsprintf(Mes,"인쇄 프로바이더: %s, 설명:%s ", pi1[i].pName, pi1[i].pComment);
		TextOut(hdc,10,y++*20,Mes,lstrlen(Mes));
	}
	free(pi1);y++;

	// 도메인내의 프린터 열거
	lstrcpy(Mes,"도메인내의 프린터 열거");
	TextOut(hdc,10,y++*20,Mes,lstrlen(Mes));
	EnumPrinters(PRINTER_ENUM_REMOTE, NULL, 1, NULL, 0, &cbNeed, &cbReturn);
	pi1=(PRINTER_INFO_1 *)malloc(cbNeed);
	EnumPrinters(PRINTER_ENUM_REMOTE, NULL, 1, (PBYTE)pi1, cbNeed, &cbNeed, &cbReturn);
	for (i=0;i<cbReturn;i++) {
		wsprintf(Mes,"프린터 이름: %s ", pi1[i].pName);
		TextOut(hdc,10,y++*20,Mes,lstrlen(Mes));
	}
	free(pi1);y++;

	ReleaseDC(hWndMain,hdc);
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes="마우스 왼쪽 버튼을 누르면 설치된 프린터 목록을 조사합니다.";

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		return 0;
	case WM_LBUTTONDOWN:
		EnumPrintObject();
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
