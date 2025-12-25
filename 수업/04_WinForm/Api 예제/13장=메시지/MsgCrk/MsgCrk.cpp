#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("MsgCrk");

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

#define TYPE 2

#if TYPE == 0
// 메시지 크랙커를 사용하는 경우
#include <windowsx.h>
// 메시지 처리 함수의 원형 정의
BOOL MsgCrk_OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void MsgCrk_OnDestroy(HWND hwnd);
void MsgCrk_OnPaint(HWND hwnd);
void MsgCrk_OnKey(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags);
void MsgCrk_OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
void MsgCrk_OnTimer(HWND hwnd, UINT id);
void MsgCrk_OnSize(HWND hwnd, UINT state, int cx, int cy);

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage) {
		HANDLE_MSG(hWnd, WM_CREATE, MsgCrk_OnCreate);
		HANDLE_MSG(hWnd, WM_DESTROY, MsgCrk_OnDestroy);
		HANDLE_MSG(hWnd, WM_PAINT, MsgCrk_OnPaint);
		HANDLE_MSG(hWnd, WM_KEYDOWN, MsgCrk_OnKey);
		HANDLE_MSG(hWnd, WM_LBUTTONDOWN, MsgCrk_OnLButtonDown);
		HANDLE_MSG(hWnd, WM_TIMER, MsgCrk_OnTimer);
		HANDLE_MSG(hWnd, WM_SIZE, MsgCrk_OnSize);
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

BOOL MsgCrk_OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	SetTimer(hWnd,0,1000,NULL);
	return TRUE;
}

void MsgCrk_OnDestroy(HWND hWnd)
{
	KillTimer(hWnd,0);
	PostQuitMessage(0);
}

void MsgCrk_OnPaint(HWND hWnd)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes=TEXT("메시지 크랙커를 사용하는 예제입니다");
	hdc=BeginPaint(hWnd, &ps);
	TextOut(hdc,10,10,Mes,lstrlen(Mes));
	EndPaint(hWnd, &ps);
}

void MsgCrk_OnKey(HWND hWnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
	HDC hdc;
	RECT rt;
	TCHAR str[MAX_PATH];
	hdc=GetDC(hWnd);
	SetRect(&rt,10,80,500,100);
	FillRect(hdc,&rt,(HBRUSH)GetStockObject(WHITE_BRUSH));
	wsprintf(str,TEXT("%c키를 눌렀습니다"),vk);
	TextOut(hdc,10,80,str,lstrlen(str));
	ReleaseDC(hWnd,hdc);
}

void MsgCrk_OnLButtonDown(HWND hWnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	HDC hdc;
	RECT rt;
	TCHAR str[MAX_PATH];
	hdc=GetDC(hWnd);
	SetRect(&rt,10,100,500,120);
	FillRect(hdc,&rt,(HBRUSH)GetStockObject(WHITE_BRUSH));
	lstrcpy(str,TEXT("마우스 왼쪽 버튼을 눌렀습니다"));
	TextOut(hdc,10,100,str,lstrlen(str));
	ReleaseDC(hWnd,hdc);
}

void MsgCrk_OnTimer(HWND hWnd, UINT id)
{
	HDC hdc;
	RECT rt;
	TCHAR str[MAX_PATH];
	static int Count;
	hdc=GetDC(hWnd);
	SetRect(&rt,10,60,500,80);
	FillRect(hdc,&rt,(HBRUSH)GetStockObject(WHITE_BRUSH));
	wsprintf(str,TEXT("%d초 경과했습니다."),Count++);
	TextOut(hdc,10,60,str,lstrlen(str));
	ReleaseDC(hWnd,hdc);
}

void MsgCrk_OnSize(HWND hWnd, UINT state, int cx, int cy)
{
	HDC hdc;
	RECT rt;
	TCHAR str[MAX_PATH];
	hdc=GetDC(hWnd);
	SetRect(&rt,10,40,500,60);
	FillRect(hdc,&rt,(HBRUSH)GetStockObject(WHITE_BRUSH));
	wsprintf(str,TEXT("현재 작업 영역의 크기:%d*%d"),cx,cy);
	TextOut(hdc,10,40,str,lstrlen(str));
	ReleaseDC(hWnd,hdc);
}
#endif

#if TYPE == 1
// 개별 메시지 처리 함수를 사용하는 경우
LRESULT OnCreate(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnDestroy(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnPaint(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnKeyDown(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnLButtonDown(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnTimer(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnSize(HWND hWnd,WPARAM wParam,LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_CREATE:
		return OnCreate(hWnd,wParam,lParam);
	case WM_DESTROY:
		return OnDestroy(hWnd,wParam,lParam);
	case WM_PAINT:
		return OnPaint(hWnd,wParam,lParam);
	case WM_KEYDOWN:
		return OnKeyDown(hWnd,wParam,lParam);
	case WM_LBUTTONDOWN:
		return OnLButtonDown(hWnd,wParam,lParam);
	case WM_TIMER:
		return OnTimer(hWnd,wParam,lParam);
	case WM_SIZE:
		return OnSize(hWnd,wParam,lParam);
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

LRESULT OnCreate(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	SetTimer(hWnd,0,1000,NULL);
	return 0;
}

LRESULT OnDestroy(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	KillTimer(hWnd,0);
	PostQuitMessage(0);
	return 0;
}

LRESULT OnPaint(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes=TEXT("메시지 크랙커를 사용하는 예제입니다");
	hdc=BeginPaint(hWnd, &ps);
	TextOut(hdc,10,10,Mes,lstrlen(Mes));
	EndPaint(hWnd, &ps);
	return 0;
}

LRESULT OnKeyDown(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	RECT rt;
	TCHAR str[MAX_PATH];
	hdc=GetDC(hWnd);
	SetRect(&rt,10,80,500,100);
	FillRect(hdc,&rt,(HBRUSH)GetStockObject(WHITE_BRUSH));
	wsprintf(str,TEXT("%c키를 눌렀습니다"),wParam);
	TextOut(hdc,10,80,str,lstrlen(str));
	ReleaseDC(hWnd,hdc);
	return 0;
}

LRESULT OnLButtonDown(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	RECT rt;
	TCHAR str[MAX_PATH];
	hdc=GetDC(hWnd);
	SetRect(&rt,10,100,500,120);
	FillRect(hdc,&rt,(HBRUSH)GetStockObject(WHITE_BRUSH));
	lstrcpy(str,TEXT("마우스 왼쪽 버튼을 눌렀습니다"));
	TextOut(hdc,10,100,str,lstrlen(str));
	ReleaseDC(hWnd,hdc);
	return 0;
}

LRESULT OnTimer(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	RECT rt;
	TCHAR str[MAX_PATH];
	static int Count;
	hdc=GetDC(hWnd);
	SetRect(&rt,10,60,500,80);
	FillRect(hdc,&rt,(HBRUSH)GetStockObject(WHITE_BRUSH));
	wsprintf(str,TEXT("%d초 경과했습니다."),Count++);
	TextOut(hdc,10,60,str,lstrlen(str));
	ReleaseDC(hWnd,hdc);
	return 0;
}

LRESULT OnSize(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	RECT rt;
	TCHAR str[MAX_PATH];
	hdc=GetDC(hWnd);
	SetRect(&rt,10,40,500,60);
	FillRect(hdc,&rt,(HBRUSH)GetStockObject(WHITE_BRUSH));
	wsprintf(str,TEXT("현재 작업 영역의 크기:%d*%d"),LOWORD(lParam),HIWORD(lParam));
	TextOut(hdc,10,40,str,lstrlen(str));
	ReleaseDC(hWnd,hdc);
	return 0;
}
#endif

#if TYPE == 2
// 고전적인 메시지 처리
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rt;
	TCHAR *Mes=TEXT("메시지 크랙커를 사용하는 예제입니다");
	TCHAR str[MAX_PATH];
	static int Count;
	switch (iMessage) {
	case WM_CREATE:
		SetTimer(hWnd,0,1000,NULL);
		return 0;
	case WM_LBUTTONDOWN:
		hdc=GetDC(hWnd);
		SetRect(&rt,10,100,500,120);
		FillRect(hdc,&rt,(HBRUSH)GetStockObject(WHITE_BRUSH));
		lstrcpy(str,TEXT("마우스 왼쪽 버튼을 눌렀습니다"));
		TextOut(hdc,10,100,str,lstrlen(str));
		ReleaseDC(hWnd,hdc);
		return 0;
	case WM_KEYDOWN:
		hdc=GetDC(hWnd);
		SetRect(&rt,10,80,500,100);
		FillRect(hdc,&rt,(HBRUSH)GetStockObject(WHITE_BRUSH));
		wsprintf(str,TEXT("%c키를 눌렀습니다"),wParam);
		TextOut(hdc,10,80,str,lstrlen(str));
		ReleaseDC(hWnd,hdc);
		return 0;
	case WM_TIMER:
		hdc=GetDC(hWnd);
		SetRect(&rt,10,60,500,80);
		FillRect(hdc,&rt,(HBRUSH)GetStockObject(WHITE_BRUSH));
		wsprintf(str,TEXT("%d초 경과했습니다."),Count++);
		TextOut(hdc,10,60,str,lstrlen(str));
		ReleaseDC(hWnd,hdc);
		return 0;
	case WM_SIZE:
		hdc=GetDC(hWnd);
		SetRect(&rt,10,40,500,60);
		FillRect(hdc,&rt,(HBRUSH)GetStockObject(WHITE_BRUSH));
		wsprintf(str,TEXT("현재 작업 영역의 크기:%d*%d"),LOWORD(lParam),HIWORD(lParam));
		TextOut(hdc,10,40,str,lstrlen(str));
		ReleaseDC(hWnd,hdc);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,10,10,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd,0);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
#endif
