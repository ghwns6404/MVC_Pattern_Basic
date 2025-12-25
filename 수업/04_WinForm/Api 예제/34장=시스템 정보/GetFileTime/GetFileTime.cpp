#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("GetFileTime");

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
	PAINTSTRUCT ps;
	TCHAR Path[MAX_PATH];
	HANDLE hFile;
	FILETIME Cr,Ac,Wr;
	FILETIME lCr,lAc,lWr;
	SYSTEMTIME sCr,sAc,sWr;
	static TCHAR Mes[255]="마우스 왼쪽 버튼을 누르면 파일의 시간을 조사해 줍니다.";
	switch (iMessage) {
	case WM_LBUTTONDOWN:
		GetCurrentDirectory(MAX_PATH,Path);
		lstrcat(Path,"\\Test.txt");
		hFile=CreateFile(Path,GENERIC_READ,0,NULL,
			OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		if (hFile == INVALID_HANDLE_VALUE) return 0;
		GetFileTime(hFile,&Cr,&Ac,&Wr);
		FileTimeToLocalFileTime(&Cr,&lCr);
		FileTimeToLocalFileTime(&Ac,&lAc);
		FileTimeToLocalFileTime(&Wr,&lWr);
		FileTimeToSystemTime(&lCr,&sCr);
		FileTimeToSystemTime(&lAc,&sAc);
		FileTimeToSystemTime(&lWr,&sWr);
		wsprintf(Mes,"생성날짜=%4d-%02d-%02d일 %d시 %d분 %d초,"
			"읽은날짜=%4d-%02d-%02d일 %d시 %d분 %d초,"
			"수정날짜=%4d-%02d-%02d일 %d시 %d분 %d초",
			sCr.wYear, sCr.wMonth, sCr.wDay, sCr.wHour, sCr.wMinute, sCr.wSecond,
			sAc.wYear, sAc.wMonth, sAc.wDay, sAc.wHour, sAc.wMinute, sAc.wSecond,
			sWr.wYear, sWr.wMonth, sWr.wDay, sWr.wHour, sWr.wMinute, sWr.wSecond
			);
		InvalidateRect(hWnd,NULL,TRUE);
		CloseHandle(hFile);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,0,50,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

