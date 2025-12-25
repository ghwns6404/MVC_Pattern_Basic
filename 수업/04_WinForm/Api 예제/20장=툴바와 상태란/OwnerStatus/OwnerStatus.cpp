#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("OwnerStatus");

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;
	
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)(COLOR_BTNFACE+1);
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

#include <commctrl.h>
HWND hState;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int SBPart[6],i;
	LPDRAWITEMSTRUCT lpdis;
	RECT rt;

	switch (iMessage) {
	case WM_CREATE:
		InitCommonControls();
		hState=CreateStatusWindow(WS_CHILD | WS_VISIBLE, "", hWnd,0);
		for (i=0;i<5;i++) {
			SBPart[i]=100+i*100;
		}
		SBPart[5]=-1;
		SendMessage(hState, SB_SETPARTS, 6, (LPARAM)SBPart);
		SendMessage(hState, SB_SETMINHEIGHT,30,0);
		SendMessage(hState, SB_SETTEXT,0 | SBT_OWNERDRAW,0);
		SendMessage(hState, SB_SETTEXT,1 | SBT_OWNERDRAW,0);
		SendMessage(hState, SB_SETTEXT,2,(LPARAM)"왼쪽");
		SendMessage(hState, SB_SETTEXT,3,(LPARAM)"\t가운데");
		SendMessage(hState, SB_SETTEXT,4,(LPARAM)"\t\t오른쪽");
		SendMessage(hState, SB_SETTEXT,5,(LPARAM)"좌\t중앙\t우    ");
		SendMessage(hState, SB_SETTEXT,0xff/*SB_SIMPLEID*/,
			(LPARAM)"상태란 전체를 다 차지하는 도움말입니다");
		return 0;
	case WM_DRAWITEM:
		lpdis=(LPDRAWITEMSTRUCT)lParam;
		if (lpdis->hwndItem == hState) {
			if (lpdis->itemID == 0) {
				SelectObject(lpdis->hDC,GetStockObject(LTGRAY_BRUSH));
				Rectangle(lpdis->hDC,lpdis->rcItem.left+10, lpdis->rcItem.top+2,
					lpdis->rcItem.right-10, lpdis->rcItem.bottom-2);
			} else {
				SelectObject(lpdis->hDC,GetStockObject(DKGRAY_BRUSH));
				Ellipse(lpdis->hDC,lpdis->rcItem.left+10, lpdis->rcItem.top+2,
					lpdis->rcItem.right-10, lpdis->rcItem.bottom-2);
			}
		}
		return 0;
	case WM_LBUTTONDOWN:
		if (SendMessage(hState, SB_ISSIMPLE,0,0)) {
			SendMessage(hState,SB_SIMPLE,(WPARAM)FALSE,0);
		} else {
			SendMessage(hState,SB_SIMPLE,(WPARAM)TRUE,0);
		}
		return 0;
	case WM_SIZE:
		SendMessage(hState, WM_SIZE, wParam, lParam);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		SetRect(&rt,10,10,500,30);
		DrawStatusText(hdc,&rt,"마우스 왼쪽 버튼을 누르면 심플 모드를 토글합니다.",0);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

