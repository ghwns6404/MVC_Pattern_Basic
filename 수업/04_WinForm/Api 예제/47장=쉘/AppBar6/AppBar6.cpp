#include <windows.h>
#include <shlwapi.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPSTR lpszClass=TEXT("AppBar6");

UINT gEdge;						// 현재 도킹된 면
BOOL gAlwaysOnTop=FALSE;		// 항상 위
BOOL gAutoHide=FALSE;			// 자동 숨김
BOOL gThick=50;					// 앱바의 두께
UINT gEdgeMoving;				// 이동중의 현재 면
BOOL gbMove=FALSE;				// 이동중

#define WM_MYAPPCALLBACK WM_USER+100

// 함수 원형들
BOOL MyABRegister(HWND hWnd,UINT uCallBack);
BOOL MyABUnRegister(HWND hWnd);
BOOL MyABSetPos(HWND hWnd,UINT uEdge,UINT nPixel,BOOL bSetPos);
void MyABSetAlwaysOnTop(HWND hWnd,BOOL bTop);
void MyABSetAutoHide(HWND hWnd,BOOL bHide);
void MyABHideReveal(HWND hWnd,BOOL bHide);
void OnMouseMove(int x, int y);

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
	return Message.wParam;
}

// 앱바를 등록한다.
BOOL MyABRegister(HWND hWnd,UINT uCallBack)
{
	APPBARDATA abd;

	abd.cbSize = sizeof(APPBARDATA);
	abd.hWnd = hWnd;
    abd.uCallbackMessage = uCallBack;
	if (!SHAppBarMessage(ABM_NEW, &abd))
		return FALSE;
	return TRUE;
}

// 앱바를 해제한다.
BOOL MyABUnRegister(HWND hWnd)
{
	APPBARDATA abd;

	abd.cbSize = sizeof(APPBARDATA);
	abd.hWnd = hWnd;
	if (!SHAppBarMessage(ABM_REMOVE, &abd))
		return FALSE;
	return TRUE;
}

// 앱바의 위치를 지정한다.
// uEdge:고정할 위치. nPixel:앱바의 두께, bSetPos:위치 확정 여부
BOOL MyABSetPos(HWND hWnd,UINT uEdge,UINT nPixel,BOOL bSetPos)
{
	APPBARDATA abd;

	abd.cbSize = sizeof(APPBARDATA);     
	abd.hWnd = hWnd;

	if ((uEdge==ABE_LEFT) || (uEdge==ABE_RIGHT)) 
		gThick=nPixel=min(gThick, GetSystemMetrics(SM_CXSCREEN)/2);
	if ((uEdge==ABE_TOP) || (uEdge==ABE_BOTTOM)) 
		gThick=nPixel=min(gThick, GetSystemMetrics(SM_CYSCREEN)/2);

	// 사용 가능한 스크린 영역을 조사한다.
	abd.uEdge = uEdge;
	SetRect(&abd.rc, 0,0,GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN));
	SHAppBarMessage(ABM_QUERYPOS, &abd);

	// 원하는 영역을 계산하여 설정한다.
	switch (uEdge) {
	case ABE_LEFT:
		abd.rc.right = abd.rc.left + nPixel;
		break;
	case ABE_RIGHT:
		abd.rc.left = abd.rc.right - nPixel;
		break;
	case ABE_TOP:
		abd.rc.bottom = abd.rc.top + nPixel;
		break;
	case ABE_BOTTOM:
		abd.rc.top = abd.rc.bottom - nPixel;
		break;
	}

	// 엡바 영역의 위치를 확정한다.
	if (bSetPos) {
		gEdge = uEdge;
		SHAppBarMessage(ABM_SETPOS, &abd);
	}

	// 실제로 윈도우를 이동시킨다.
	MoveWindow(hWnd, abd.rc.left, abd.rc.top, 
		abd.rc.right - abd.rc.left, abd.rc.bottom - abd.rc.top, TRUE); 

	return TRUE;
}

void MyABSetAlwaysOnTop(HWND hWnd,BOOL bTop)
{
	if (gAlwaysOnTop)
		SetWindowPos(hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	else
		SetWindowPos(hWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

void MyABSetAutoHide(HWND hWnd,BOOL bHide)
{
	APPBARDATA abd;
	
	abd.cbSize = sizeof(APPBARDATA);     
	abd.hWnd = hWnd;
	if (bHide == TRUE) {
		abd.uEdge = gEdge;
		if (SHAppBarMessage(ABM_GETAUTOHIDEBAR,&abd) != NULL) {
			MessageBox(hWnd,"이미 자동 숨김 윈도우가 있습니다.","알림",MB_OK);
			return;
		}
		abd.lParam=TRUE;
		if (SHAppBarMessage(ABM_SETAUTOHIDEBAR,&abd) == FALSE) {
			MessageBox(hWnd,"자동 숨김 실패","알림",MB_OK);
			return;
		}
		gAutoHide=TRUE;
		MyABUnRegister(hWnd);
		SetTimer(hWnd,0,1000,NULL);
	} else {
		KillTimer(hWnd,0);
		gAutoHide=FALSE;
		abd.uEdge = gEdge;
		abd.lParam=FALSE;
		SHAppBarMessage(ABM_SETAUTOHIDEBAR,&abd);
		MyABRegister(hWnd,WM_MYAPPCALLBACK);
		MyABSetPos(hWnd,gEdge,gThick,TRUE);
	}
}

void MyABHideReveal(HWND hWnd,BOOL bHide)
{
	int cx,cy;
	RECT rt;

	cx=GetSystemMetrics(SM_CXSCREEN);
	cy=GetSystemMetrics(SM_CYSCREEN);
	GetWindowRect(hWnd,&rt);

	if (bHide) {
		switch (gEdge) {
		case ABE_LEFT:
			SetWindowPos(hWnd,NULL,-gThick+2,rt.top,0,0,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOSIZE);
			break;
		case ABE_TOP:
			SetWindowPos(hWnd,NULL,rt.left,-gThick+2,0,0,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOSIZE);
			break;
		case ABE_RIGHT:
			SetWindowPos(hWnd,NULL,cx-1,rt.top,0,0,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOSIZE);
			break;
		case ABE_BOTTOM:
			SetWindowPos(hWnd,NULL,rt.left,cy-1,0,0,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOSIZE);
			break;
		}
	} else {
		switch (gEdge) {
		case ABE_LEFT:
			SetWindowPos(hWnd,NULL,0,rt.top,0,0,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOSIZE);
			break;
		case ABE_TOP:
			SetWindowPos(hWnd,NULL,rt.left,0,0,0,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOSIZE);
			break;
		case ABE_RIGHT:
			SetWindowPos(hWnd,NULL,cx-gThick,rt.top,0,0,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOSIZE);
			break;
		case ABE_BOTTOM:
			SetWindowPos(hWnd,NULL,rt.left,cy-gThick,0,0,SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOSIZE);
			break;
		}
	}
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HMENU hMenu, hPopup;
	APPBARDATA abd;

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		MyABRegister(hWnd,WM_MYAPPCALLBACK);
		MyABSetPos(hWnd,ABE_RIGHT,gThick,TRUE);
		return 0;
	case WM_NCMOUSEMOVE:
		if (gAutoHide)
			MyABHideReveal(hWnd,FALSE);
		return 0;
	case WM_TIMER:
		RECT rt;
		POINT pt;
		if (GetActiveWindow()==hWnd)
			return 0;
		GetWindowRect(hWnd,&rt);
		GetCursorPos(&pt);
		if (PtInRect(&rt,pt)==FALSE)
			MyABHideReveal(hWnd,TRUE);
		return 0;
	case WM_LBUTTONDOWN:
		if (gAutoHide==FALSE) {
			gEdgeMoving=gEdge;
			gbMove=TRUE;
			SetCapture(hWnd);
		}
		return 0;
	case WM_LBUTTONUP:
		if (gAutoHide==FALSE) {
			gbMove=FALSE;
			ReleaseCapture();
			MyABSetPos(hWndMain,gEdgeMoving,gThick,TRUE);
		}
		return 0;
	case WM_MOUSEMOVE:
		if (gbMove)
			OnMouseMove(LOWORD(lParam),HIWORD(lParam));
		return 0;
	case WM_CONTEXTMENU:
		hMenu = LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_POPUP));
		hPopup = GetSubMenu(hMenu, 0);
		if (gAlwaysOnTop) CheckMenuItem(hPopup,IDM_ALWAYSONTOP,MF_BYCOMMAND|MF_CHECKED);
		if (gAutoHide) CheckMenuItem(hPopup,IDM_AUTOHIDE,MF_BYCOMMAND|MF_CHECKED);
		CheckMenuItem(hPopup,IDM_LEFT+gEdge,MF_BYCOMMAND|MF_CHECKED);
		TrackPopupMenu(hPopup, TPM_LEFTALIGN, LOWORD(lParam), HIWORD(lParam), 0, hWnd, NULL);
		DestroyMenu(hMenu);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDM_LEFT:
			if (gAutoHide==FALSE)
				MyABSetPos(hWndMain,ABE_LEFT,gThick,TRUE);
			break;
		case IDM_TOP:
			if (gAutoHide==FALSE)
				MyABSetPos(hWndMain,ABE_TOP,gThick,TRUE);
			break;
		case IDM_RIGHT:
			if (gAutoHide==FALSE)
				MyABSetPos(hWndMain,ABE_RIGHT,gThick,TRUE);
			break;
		case IDM_BOTTOM:
			if (gAutoHide==FALSE)
				MyABSetPos(hWndMain,ABE_BOTTOM,gThick,TRUE);
			break;
		case IDM_ALWAYSONTOP:
			gAlwaysOnTop =! gAlwaysOnTop;
			MyABSetAlwaysOnTop(hWndMain,gAlwaysOnTop);
			break;
		case IDM_AUTOHIDE:
			if (gAutoHide==FALSE)
				MyABSetAutoHide(hWndMain,TRUE);
			else
				MyABSetAutoHide(hWndMain,FALSE);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		}
		return 0;
	case WM_ACTIVATE:
		abd.cbSize = sizeof(APPBARDATA);     
		abd.hWnd = hWnd;
		SHAppBarMessage(ABM_ACTIVATE,&abd);
		return 0;
	case WM_WINDOWPOSCHANGED:
		abd.cbSize = sizeof(APPBARDATA);     
		abd.hWnd = hWnd;
		SHAppBarMessage(ABM_WINDOWPOSCHANGED,&abd);
		return 0;
	case WM_SETTINGCHANGE:
		if (wParam == SPI_SETWORKAREA)
			MyABSetPos(hWnd,gEdge,gThick,TRUE);
		return 0;
	case WM_MYAPPCALLBACK:
		switch (wParam) {
		case ABN_POSCHANGED:
			MyABSetPos(hWnd,gEdge,gThick,TRUE);
			break;
		case ABN_FULLSCREENAPP:
			if ((lParam==TRUE) && (gAlwaysOnTop==TRUE))
				SetWindowPos(hWndMain,HWND_BOTTOM,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
			if ((lParam==FALSE) && (gAlwaysOnTop==TRUE))
				MyABSetAlwaysOnTop(hWnd,gAlwaysOnTop);
			break;
		// 한번 자동 숨김 상태가 되어 버리면 이 메시지는 다시 받을 수 없다.
		case ABN_STATECHANGE:
			DWORD TState;
			abd.cbSize = sizeof(APPBARDATA);     
			abd.hWnd = hWnd;
			TState=SHAppBarMessage(ABM_GETSTATE,&abd);
			gAlwaysOnTop=((TState & ABS_ALWAYSONTOP)!=0);
			MyABSetAlwaysOnTop(hWnd,gAlwaysOnTop);
			MyABSetAutoHide(hWnd,((TState & ABS_AUTOHIDE)!=0));
			break;
		case ABN_WINDOWARRANGE:
			// 아무일도 할 필요가 없다.
			break;
		}
		return 0;
	// 화면 안쪽으로만 크기 조정이 가능하다.
	case WM_NCHITTEST:
		LRESULT lRes;
		lRes=DefWindowProc(hWnd,iMessage,wParam,lParam);
		if ((gEdge==ABE_TOP) && ((lRes==HTBOTTOM) || (lRes==HTBOTTOMLEFT) || (lRes==HTBOTTOMRIGHT))) return HTBOTTOM;
		if ((gEdge==ABE_BOTTOM) && ((lRes==HTTOP) || (lRes==HTTOPLEFT) || (lRes==HTTOPRIGHT))) return HTTOP;
		if ((gEdge==ABE_LEFT) && ((lRes==HTRIGHT) || (lRes==HTTOPRIGHT) || (lRes==HTBOTTOMRIGHT))) return HTRIGHT;
		if ((gEdge==ABE_RIGHT) && ((lRes==HTLEFT) || (lRes==HTTOPLEFT) || (lRes==HTBOTTOMLEFT))) return HTLEFT;
		return HTCLIENT;
	case WM_EXITSIZEMOVE:
		RECT crt;
		GetWindowRect(hWnd,&crt);
		if ((gEdge==ABE_LEFT) || (gEdge==ABE_RIGHT)) gThick=crt.right-crt.left;
		if ((gEdge==ABE_TOP) || (gEdge==ABE_BOTTOM)) gThick=crt.bottom-crt.top;
		MyABSetPos(hWnd,gEdge,gThick,TRUE);
		return 0;
	case WM_GETMINMAXINFO:
		MINMAXINFO *pM;
		if (gbMove == FALSE) {
			pM=(MINMAXINFO *)lParam;
			if ((gEdge==ABE_LEFT) || (gEdge==ABE_RIGHT))
				pM->ptMaxTrackSize.x=GetSystemMetrics(SM_CXSCREEN)/2;
			if ((gEdge==ABE_TOP) || (gEdge==ABE_BOTTOM))
				pM->ptMaxTrackSize.y=GetSystemMetrics(SM_CYSCREEN)/2;
		}
		return 0;
	case WM_DESTROY:
		MyABUnRegister(hWnd);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

void OnMouseMove(int x, int y)
{
	POINT pt;
	UINT Edge;
	int cx,cy;
	double a;

	cx=GetSystemMetrics(SM_CXSCREEN);
	cy=GetSystemMetrics(SM_CYSCREEN);
	a=(double)cy/cx;
	GetCursorPos(&pt);

	if (pt.y < a*pt.x) {
		if (pt.y < cy-a*pt.x)
			Edge=ABE_TOP;
		else
			Edge=ABE_RIGHT;
	} else {
		if (pt.y < cy-a*pt.x)
			Edge=ABE_LEFT;
		else
			Edge=ABE_BOTTOM;
	}

	if (Edge != gEdgeMoving) {
		MyABSetPos(hWndMain,Edge,gThick,FALSE);
		gEdgeMoving=Edge;
	}
}
