#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
LPSTR lpszClass="FindIdle";
HWND hWndMain;

void OnIdle();

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
		  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;
	
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);
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

	
/* 원론적인 방법 : CPU가 항상 100%이므로 바람직하지 않다.
	for (;;) {
		if (PeekMessage(&Message,NULL,0,0,PM_REMOVE)) {
			if (Message.message==WM_QUIT)
				break;
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else {
			OnIdle();
		}
	}
//*/

//* OnIdle을 두 번 이상 호출하지 않도록 한다 - 회수는 줄어들지만 CPU 점유율은 떨어지지 않는다.
	BOOL AllowIdle=TRUE;
	for (;;) {
		if (PeekMessage(&Message,NULL,0,0,PM_REMOVE)) {
			if (Message.message==WM_QUIT)
				break;
			AllowIdle=TRUE;
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else {
			if (AllowIdle) {
				OnIdle();
				AllowIdle=FALSE;
			}
		}
	}
//*/

/* 처리할 메시지가 없을 때 WaitMessage 함수를 호출하여 대기하도록 한다.
	BOOL AllowIdle=TRUE;
	for (;;) {
		if (PeekMessage(&Message,NULL,0,0,PM_REMOVE)) {
			if (Message.message==WM_QUIT)
				break;
			AllowIdle=TRUE;
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else {
			if (AllowIdle) {
				OnIdle();
				AllowIdle=FALSE;
			}
			WaitMessage();
		}
	}
//*/

/* GetMessage 함수로 메시지를 읽도록 수정한다.
	BOOL AllowIdle=TRUE;
	for (;;) {
		do {
			if (!GetMessage(&Message,NULL,0,0))
				goto endloop;
			AllowIdle=TRUE;
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		} while (PeekMessage(&Message,NULL,0,0,PM_NOREMOVE));

		if (AllowIdle) {
			OnIdle();
			AllowIdle=FALSE;
		}
	}
endloop:
//*/
		
		return Message.wParam;
}

void OnIdle()
{
	HDC hdc;
	hdc=GetDC(hWndMain);
	int x,y,r;
	HBRUSH hBrush,hOldBrush;

	x=rand() % 640;
	y=rand() % 480;
	r=rand() % 10+10;
	hBrush=CreateSolidBrush(RGB(rand()%256,rand()%256,rand()%256));
	hOldBrush=(HBRUSH)SelectObject(hdc,hBrush);
	Ellipse(hdc,x-r,y-r,x+r,y+r);
	DeleteObject(SelectObject(hdc,hOldBrush));

	ReleaseDC(hWndMain,hdc);
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	TCHAR str[128];

	switch (iMessage) {
	case WM_MOUSEMOVE:
		wsprintf(str,TEXT("마우스 위치=(%d,%d)"),LOWORD(lParam),HIWORD(lParam));
		SetWindowText(hWnd,str);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
