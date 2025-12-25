#include <windows.h>
#include <wininet.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("AsyncFTP");

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

TCHAR g_Server[50]="ftp.microsoft.com";
TCHAR g_User[20]="anonymous";
TCHAR g_Pass[20]="aaa@bbb.co.kr";
TCHAR g_File[MAX_PATH]="ls-lr.zip";
HINTERNET hInternet, hFtp;
DWORD Bytes;
BOOL bDownLoading=FALSE;

// 콜백함수
void CALLBACK MyCallBack(HINTERNET h, DWORD con, DWORD stat, LPVOID info, DWORD len)
{
	TCHAR szTmp[255];

	if (con==1) {
		switch (stat) {
		case INTERNET_STATUS_HANDLE_CREATED:
			bDownLoading=TRUE;
			SetWindowText(hWndMain,"다운로드 시작");
			break;
		case INTERNET_STATUS_REQUEST_COMPLETE:
			bDownLoading=FALSE;
			SetWindowText(hWndMain,"다운로드 완료");
			break;
		case INTERNET_STATUS_RESPONSE_RECEIVED:
			Bytes=Bytes+((INTERNET_ASYNC_RESULT *)info)->dwResult;
			wsprintf(szTmp, "%d바이트 받았습니다",Bytes);
			SetWindowText(hWndMain,szTmp);
			break;
		}
	}
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR szCap[255];
	TCHAR *Mes="마우스 왼쪽 버튼을 누르면 비동기 다운로드를 시작합니다";

	switch (iMessage) {
	case WM_CREATE:
		// 비동기 방식으로 연결한다.
		hInternet=InternetOpen("FTPTEST", INTERNET_OPEN_TYPE_PRECONFIG,
			NULL, NULL, INTERNET_FLAG_ASYNC);
		if (hInternet == NULL) {
			MessageBox(hWndMain, "인터넷 연결 실패","알림",MB_OK);
			return -1;
		}

		// 콜백함수 지정
		InternetSetStatusCallback(hInternet, MyCallBack);

		hFtp=InternetConnect(hInternet,g_Server,INTERNET_DEFAULT_FTP_PORT,
			g_User,g_Pass,INTERNET_SERVICE_FTP,0,0);

		if (hFtp==NULL) {
			MessageBox(hWndMain, "FTP 연결 실패","알림",MB_OK);
			InternetCloseHandle(hInternet);
			return -1;
		}
		wsprintf(szCap,"%s 서버에 연결됨",g_Server);
		SetWindowText(hWnd,szCap);
		return 0;
	// 비동기 방식으로 파일을 다운로드받는다.
	case WM_LBUTTONDOWN:
		if (bDownLoading == TRUE) {
			MessageBox(hWndMain,"다운로드 중입니다","알림",MB_OK);
			return 0;
		}
		Bytes=0;
		lstrcpy(szCap,"c:\\");
		lstrcat(szCap,g_File);
		if (FtpGetFile(hFtp,g_File,szCap,FALSE,FILE_ATTRIBUTE_NORMAL,
			FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD,1)==FALSE) {
			MessageBox(hWndMain,"파일이 없습니다.","알림",MB_OK);
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,50,50,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	// 연결을 끊는다.
	case WM_DESTROY:
		InternetCloseHandle(hFtp);
		InternetCloseHandle(hInternet);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

