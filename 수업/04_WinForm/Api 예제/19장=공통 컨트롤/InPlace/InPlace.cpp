#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("InPlace");

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
		CW_USEDEFAULT,CW_USEDEFAULT,450,300,
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
struct tag_Text {
	int x,y;
	SIZE sz;
	TCHAR *Text;
};

tag_Text arText[]={
	{200,30,{0,0},"툴팁"},
	{200,50,{0,0},"Windows API ToolTip Control"},
	{200,70,{0,0},"아주 정말 무척 매우 많이 긴 텍스트입니다."},
	{200,90,{0,0},"이 문장의 일부가 가려지면 제자리 툴팁이 위에 출력되어 "
	"가려진 텍스트를 보여 줍니다."}
};

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	INITCOMMONCONTROLSEX iccex;
	static HWND hTip;
	TOOLINFO ti;
	int i;
	POINT pt;
	RECT crt, trt;

	switch (iMessage) {
	case WM_CREATE:
		iccex.dwICC=ICC_WIN95_CLASSES;
		iccex.dwSize=sizeof(INITCOMMONCONTROLSEX);
		InitCommonControlsEx(&iccex);

		// 툴팁 컨트롤 생성
		hTip=CreateWindowEx(WS_EX_TOPMOST,TOOLTIPS_CLASS,NULL,0,
			CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
			hWnd,NULL,g_hInst,NULL);

		// 시스템 글꼴로 변경
		SendMessage(hTip,WM_SETFONT,(WPARAM)(HFONT)GetStockObject(SYSTEM_FONT),0);

		ti.cbSize=sizeof(TOOLINFO);
		ti.uFlags=TTF_TRACK | TTF_ABSOLUTE;
		ti.hwnd=hWnd;
		ti.uId=0;
		ti.lpszText="dummy";
		SetRect(&ti.rect,0,0,0,0);
		SendMessage(hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti);

		// 텍스트의 크기 계산
		hdc=GetDC(hWnd);
		for (i=0;i<sizeof(arText)/sizeof(arText[0]);i++) {
			GetTextExtentPoint32(hdc,arText[i].Text,
				lstrlen(arText[i].Text),&arText[i].sz);
		}
		ReleaseDC(hWnd,hdc);
		return 0;
	case WM_MOUSEMOVE:
		// 텍스트 영역 안인지 조사
		pt.x=LOWORD(lParam);
		pt.y=HIWORD(lParam);
		for (i=0;i<sizeof(arText)/sizeof(arText[0]);i++) {
			if (pt.x > arText[i].x && pt.x < arText[i].x+arText[i].sz.cx &&
				pt.y > arText[i].y && pt.y < arText[i].y+arText[i].sz.cy)
				break;
		}

		ti.cbSize=sizeof(TOOLINFO);
		ti.uFlags=0;
		ti.hwnd=hWnd;
		ti.uId=0;

		// 영역 밖이면 툴팁 숨김
		if (i==sizeof(arText)/sizeof(arText[0])) {
			SendMessage(hTip, TTM_TRACKACTIVATE,(WPARAM)FALSE,(LPARAM)&ti);
			return 0;
		}

		// 영역 안이더라도 잘리지 않았으면 상관없다.
		GetClientRect(hWnd,&crt);
		if (crt.right >= arText[i].x+arText[i].sz.cx &&
			crt.bottom >= arText[i].y+arText[i].sz.cy) {
			return 0;
		}

		// 텍스트 변경
		ti.lpszText=arText[i].Text;
		SendMessage(hTip,TTM_UPDATETIPTEXT,0,(LPARAM)&ti);

		// 위치 조정
		pt.x=arText[i].x;
		pt.y=arText[i].y;
		ClientToScreen(hWnd,&pt);
		SetRect(&trt,pt.x,pt.y,pt.x+arText[i].sz.cx,pt.y+arText[i].sz.cy);
		SendMessage(hTip,TTM_ADJUSTRECT,(WPARAM)TRUE,(LPARAM)&trt);

		// 툴팁 보임
		SendMessage(hTip,TTM_TRACKPOSITION,0,(LPARAM)MAKELPARAM(trt.left, trt.top));
		SendMessage(hTip, TTM_TRACKACTIVATE,(WPARAM)TRUE,(LPARAM)&ti);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		for (i=0;i<sizeof(arText)/sizeof(arText[0]);i++) {
			TextOut(hdc,arText[i].x,arText[i].y,arText[i].Text,lstrlen(arText[i].Text));
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

