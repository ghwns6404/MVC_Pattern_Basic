#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("DefPrinter");

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
	hWndMain=hWnd;
	
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

BOOL MySetDefaultPrinter(LPSTR DefPrinter)
{
	OSVERSIONINFO osv;
	DWORD dwNeed=0, dwReturn=0;
	PRINTER_INFO_2 *pi2;
	HANDLE hPrinter=NULL;
	TCHAR Buffer[255];

	osv.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	GetVersionEx(&osv);

	// 2000 이후
	if (osv.dwMajorVersion >= 5) {
		if (SetDefaultPrinter(DefPrinter) == 0)
			return FALSE;
	// 95/98, NT 4.0
	} else {
		if (OpenPrinter(DefPrinter,&hPrinter,NULL) == 0 || hPrinter == NULL)
			return FALSE;
		GetPrinter(hPrinter,2,0,0,&dwNeed);
		pi2=(PRINTER_INFO_2 *)malloc(dwNeed);
		GetPrinter(hPrinter,2,(LPBYTE)pi2,dwNeed,&dwNeed);

		if (osv.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) {
			pi2->Attributes |= PRINTER_ATTRIBUTE_DEFAULT;
			if (SetPrinter(hPrinter,2,(LPBYTE)pi2,0) == 0) {
				free(pi2);
				return FALSE;
			}
		} else {
			wsprintf(Buffer,"%s,%s,%s",DefPrinter,pi2->pDriverName,pi2->pPortName);
			if (WriteProfileString("windows","device",Buffer) == 0)
				return FALSE;
		}
		SendNotifyMessage(HWND_BROADCAST,WM_SETTINGCHANGE,0,(LPARAM)
			(LPCTSTR)"windows");
	}

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR DefPrinter[128];
	TCHAR *Mes="왼쪽 - 기본 프린터 조사, 오른쪽 - 기본 프린터 변경";

	switch (iMessage) {
	// 기본 프린터 구하기
	case WM_LBUTTONDOWN:
		if (MyGetDefaultPrinter(DefPrinter) == TRUE) {
			MessageBox(hWnd,DefPrinter,"기본 프린터",MB_OK);
		} else {
			MessageBox(hWnd,"기본 프린터를 찾을 수 없습니다","기본 프린터",MB_OK);
		}
		return 0;
	// 기본 프린터 설정하기
	case WM_RBUTTONDOWN:
		if (MySetDefaultPrinter("HP LaserJet 6P") == TRUE) {
			MessageBox(hWnd,"기본 프린터를 변경했습니다","알림",MB_OK);
		} else {
			MessageBox(hWnd,"기본 프린터 변경에 실패했습니다.","알림",MB_OK);
		}
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


/*
	// 95/95에서 기본 프린터 구하기 - 실제로 이 방법을 쓸 필요는 없다. 다만 95에서 이 방법이
	// 동작했었다는 의미만 남아 있을 뿐이다.
	if (osv.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) {
		PRINTER_INFO_2 *pi2;
		dwNeed=dwReturn=0;

		EnumPrinters(PRINTER_ENUM_DEFAULT,NULL,2,NULL,0,&dwNeed,&dwReturn);
		if (dwNeed==0)
			return FALSE;
		pi2=(PRINTER_INFO_2 *)malloc(dwNeed);
		if (EnumPrinters(PRINTER_ENUM_DEFAULT,NULL,2,(PBYTE)pi2,0,&dwNeed,&dwReturn) == 0) {
			free(pi2);
			return FALSE;
		}
		lstrcpy(DefPrinter,pi2->pPrinterName);
		free(pi2);
	}

	// 95/98에서 기본 프린터 지정하기
	if (osv.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) {
		PRINTER_INFO_2 *pi2;
		dwNeed=dwReturn=0;
		HANDLE hPrinter=NULL;

		if (OpenPrinter(DefPrinter,&hPrinter,NULL) == 0 || hPrinter == NULL)
			return FALSE;
		GetPrinter(hPrinter,2,0,0,&dwNeed);
		pi2=(PRINTER_INFO_2 *)malloc(dwNeed);
		GetPrinter(hPrinter,2,(LPBYTE)pi2,dwNeed,&dwNeed);

		pi2->Attributes |= PRINTER_ATTRIBUTE_DEFAULT;
		if (SetPrinter(hPrinter,2,(LPBYTE)pi2,0) == 0) {
			free(pi2);
			return FALSE;
		}

		SendMessageTimeout(HWND_BROADCAST,WM_SETTINGCHANGE,0,(LPARAM)(LPCTSTR)"windows",SMTO_NORMAL,1000,NULL);
		free(pi2);
	}
*/