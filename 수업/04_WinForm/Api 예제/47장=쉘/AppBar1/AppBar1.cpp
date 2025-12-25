#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("AppBar1");
#define WM_MYAPPCALLBACK WM_USER+100

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;

	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)GetStockObject(LTGRAY_BRUSH);
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=NULL;
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	// 타이틀 바가 없는 윈도우를 만든다. 크기는 아무래도 상관없다.
	hWnd=CreateWindowEx(WS_EX_TOOLWINDOW,lpszClass,"",WS_POPUP | WS_THICKFRAME,
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
	APPBARDATA abd;

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		// 앱바를 등록한다.
		abd.cbSize=sizeof(APPBARDATA);
		abd.hWnd=hWnd;
		abd.uCallbackMessage=WM_MYAPPCALLBACK;
		SHAppBarMessage(ABM_NEW,&abd);

		// 사용 가능한 스크린 영역을 조사한다.
		abd.uEdge=ABE_RIGHT;
		SetRect(&abd.rc,0,0,GetSystemMetrics(SM_CXSCREEN),
			GetSystemMetrics(SM_CYSCREEN));
		SHAppBarMessage(ABM_QUERYPOS,&abd);

		// 원하는 영역을 계산하여 설정한다. 
		abd.rc.left=abd.rc.right-50;
		
		// 앱바 영역의 위치를 옮긴다.
		SHAppBarMessage(ABM_SETPOS,&abd);

		// 실제로 윈도우를 이동시킨다.
		MoveWindow(hWnd,abd.rc.left,abd.rc.top,
			50,abd.rc.bottom-abd.rc.top,TRUE);

		return 0;
	// 마우스 왼쪽 버튼을 누르면 종료한다.
	case WM_LBUTTONDOWN:
		DestroyWindow(hWnd);
		return 0;
	case WM_DESTROY:
		// 앱바 등록을 해제한다.
		abd.cbSize=sizeof(APPBARDATA);
		abd.hWnd=hWnd;
		SHAppBarMessage(ABM_REMOVE,&abd);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

