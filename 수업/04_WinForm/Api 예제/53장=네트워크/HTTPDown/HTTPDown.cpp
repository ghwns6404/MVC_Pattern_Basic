#include <windows.h>
#include <wininet.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("HTTPDown");

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

#define ID_EDIT 100
HWND hEdit;
void HTTPDown()
{
	HINTERNET hInternet, hHttp;
	HINTERNET hReq;
	DWORD Size;
	TCHAR buf[65000];
	TCHAR buf2[65000]={0,};
	DWORD dwRead;

	hInternet=InternetOpen("HTTPTEST", INTERNET_OPEN_TYPE_PRECONFIG,
		NULL, NULL, 0);
	if (hInternet == NULL) {
		return;
	}

	hHttp=InternetConnect(hInternet,"www.microsoft.com",0,"","",INTERNET_SERVICE_HTTP,0,0);
	if (hHttp==NULL)
		return;

	hReq=HttpOpenRequest(hHttp,"GET","index.htm",NULL,NULL,NULL,0,0);
	HttpSendRequest(hReq,NULL,0,NULL,0);

	do {
		InternetQueryDataAvailable(hReq,&Size,0,0);
		InternetReadFile(hReq,buf,Size,&dwRead);
		buf[dwRead]=0;
		lstrcat(buf2,buf);
	} while (dwRead != 0);

	SetWindowText(hEdit,buf2);

	InternetCloseHandle(hHttp);
	InternetCloseHandle(hInternet);
	hHttp=NULL;
	hInternet=NULL;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage) {
	case WM_CREATE:
		hEdit=CreateWindow("edit",NULL,WS_CHILD | WS_VISIBLE | WS_BORDER | 
			WS_VSCROLL | ES_MULTILINE,
			10,10,200,25,hWnd,(HMENU)ID_EDIT,g_hInst,NULL);
		HTTPDown();
		return 0;
	case WM_SIZE:
		MoveWindow(hEdit,0,0,LOWORD(lParam), HIWORD(lParam), TRUE);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

