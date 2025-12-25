#include <windows.h>
#include <commctrl.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("IPAddress");

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
	INITCOMMONCONTROLSEX icex;
	static HWND hIP;
	DWORD Address;
	TCHAR Result[256];
	TCHAR *Mes="왼쪽 버튼:입력한 IP 조사, 오른쪽 버튼:IP 설정";

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		icex.dwSize=sizeof(icex);
		icex.dwICC=ICC_INTERNET_CLASSES;
		InitCommonControlsEx(&icex);
		hIP=CreateWindow(WC_IPADDRESS, NULL,WS_BORDER | WS_CHILD | WS_VISIBLE,
			50,50,200,25,hWnd,NULL,g_hInst,NULL);
		return 0;
	case WM_LBUTTONDOWN:
		SendMessage(hIP,IPM_GETADDRESS,0,(LPARAM)&Address);
		wsprintf(Result,"입력한 주소는 %d.%d.%d.%d입니다.",
			FIRST_IPADDRESS(Address),SECOND_IPADDRESS(Address),
			THIRD_IPADDRESS(Address),FOURTH_IPADDRESS(Address));
		MessageBox(hWnd,Result,"주소",MB_OK);
		return 0;
	case WM_RBUTTONDOWN:
		SendMessage(hIP,IPM_SETADDRESS,0,0x11223344);
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
