#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("Mouse4");

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

// 그려지는 선의 정보를 담는 구조체 배열
struct {
	int x,y;
	BOOL Move;
} Line[10000];
int index=0;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int i;
	static BOOL bNowDraw=FALSE;
	switch (iMessage) {
	// 시작점의 위치를 배열에 기록해 둔다.
	case WM_LBUTTONDOWN:
		bNowDraw=TRUE;
		Line[index].x=(int)(short)LOWORD(lParam);
		Line[index].y=(int)(short)HIWORD(lParam);
		Line[index].Move=TRUE;
		SetCapture(hWnd);
		index++;
		return 0;
	// 다음 마우스 이동점까지 선을 그리고 배열상에 기록한다.
	case WM_MOUSEMOVE:
		if (bNowDraw==TRUE) {
			hdc=GetDC(hWnd);
			MoveToEx(hdc,Line[index-1].x,Line[index-1].y,NULL);
			Line[index].x=(int)(short)LOWORD(lParam);
			Line[index].y=(int)(short)HIWORD(lParam);
			Line[index].Move=FALSE;
			LineTo(hdc,Line[index].x,Line[index].y);
			index++;
			ReleaseDC(hWnd,hdc);
		}
		return 0;
	// 그리기를 종료한다.
	case WM_LBUTTONUP:
		bNowDraw=FALSE;
		ReleaseCapture();
		return 0;
	// 화면을 지우고 다시 그리며 배열의 인덱스를 리셋하여 다시 기록하도록 한다.
	case WM_RBUTTONDOWN:
		index=0;
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	// 배열의 정보를 읽어 화면을 복구한다.
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		for (i=0;i<index;i++) {
			if (Line[i].Move == TRUE) {
				MoveToEx(hdc,Line[i].x, Line[i].y, NULL);
			} else {
				LineTo(hdc,Line[i].x, Line[i].y);
			}
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
