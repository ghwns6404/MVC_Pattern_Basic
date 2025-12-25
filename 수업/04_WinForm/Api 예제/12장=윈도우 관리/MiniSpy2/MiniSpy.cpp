#include <windows.h>
#include "SHReg.h"
#include "resource.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("MiniSpy");

#define CAPWIDTH 15
#define WIDTH 300
#define HEIGHT 100
#define KEY TEXT("Software\\MiyoungSoft\\MiniSpy\\")

TCHAR *szAbout=TEXT("각종 윈도우의 속성을 살펴보는 MiniSpy입니다\r\n")
	TEXT("마우스 커서를 원하는 윈도우위에 올려 두시면 커서 아래에 ")
	TEXT("있는 윈도우의 속성들을 조사해서 보여줍니다.");
int bTop;
HWND hWndOld;
TCHAR str[1024];
HFONT font;
BOOL bCloseDown;

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

	hWnd=CreateWindowEx(WS_EX_TOPMOST,lpszClass,lpszClass,WS_POPUP,
		CW_USEDEFAULT,CW_USEDEFAULT,WIDTH,HEIGHT,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	hWndMain=hWnd;
	
	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

// 윈도우 속성을 문자열 형태로 조립한다. WS_는 생략하였다.
#define TEST(n) (dwStyle & (1 << n))
void GetStyleString(DWORD dwStyle, TCHAR *sStyle)
{
	if (TEST(31)) lstrcpy(sStyle,TEXT("POPUP"));
	else if (TEST(30)) lstrcpy(sStyle,TEXT("CHILD"));
	else lstrcpy(sStyle,TEXT("OVERLAPPED"));

	if (TEST(29)) lstrcat(sStyle,TEXT(" | MINIMIZE"));
	if (TEST(28)) lstrcat(sStyle,TEXT(" | VISIBLE"));
	if (TEST(27)) lstrcat(sStyle,TEXT(" | DISABLED"));
	if (TEST(26)) lstrcat(sStyle,TEXT(" | CLIPSIBLINGS"));
	if (TEST(25)) lstrcat(sStyle,TEXT(" | CLIPCHILDREN"));
	if (TEST(24)) lstrcat(sStyle,TEXT(" | MAXIMIZE"));
	if (TEST(23)) lstrcat(sStyle,TEXT(" | BORDER"));
	if (TEST(22)) lstrcat(sStyle,TEXT(" | DLGFRAME"));
	if (TEST(21)) lstrcat(sStyle,TEXT(" | VSCROLL"));
	if (TEST(20)) lstrcat(sStyle,TEXT(" | HSCROLL"));
	if (TEST(19)) lstrcat(sStyle,TEXT(" | SYSMENU"));
	if (TEST(18)) lstrcat(sStyle,TEXT(" | THICKFRAME"));
	if (TEST(17)) lstrcat(sStyle,TEXT(" | MINIMIZEBOX"));
	if (TEST(16)) lstrcat(sStyle,TEXT(" | MAXIMIZEBOX"));
}

BOOL CALLBACK AboutDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage) {
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			EndDialog(hDlg,IDOK);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	POINT pt;
	HWND hWndPoint, hWndParent;
	TCHAR sCaption[256], sClass[256];
	RECT crt;
	UINT nHit;
	HFONT oldfont;
	HRGN hWndRgn;
	HMENU hMenu, hPopup;
	DWORD dwStyle;
	TCHAR sStyle[256];

	switch (iMessage) {
	case WM_CREATE:
		hWndRgn=CreateRoundRectRgn(0,0,WIDTH,HEIGHT,20,20);
		SetWindowRgn(hWnd,hWndRgn,FALSE);
		SetTimer(hWnd,1,500,NULL);
		font=CreateFont(12,0,0,0,0,0,0,0,HANGEUL_CHARSET,3,2,1,
			VARIABLE_PITCH | FF_MODERN,TEXT("굴림"));
		// 최후 실행 위치로 이동한다.
		crt.left=SHRegReadInt(SHCU,KEY TEXT("Pos"),TEXT("X"),0);
		crt.top=SHRegReadInt(SHCU,KEY TEXT("Pos"),TEXT("Y"),0);
		bTop=SHRegReadInt(SHCU,KEY TEXT("Pos"),TEXT("Top"),1);
		SetWindowPos(hWnd,(bTop ? HWND_TOPMOST:HWND_NOTOPMOST),
			crt.left,crt.top,0,0,SWP_NOSIZE);
		return 0;
	case WM_TIMER:
		GetCursorPos(&pt);
		hWndPoint=WindowFromPoint(pt);
		if (hWndPoint == hWndOld)
			return 0;
		hWndOld=hWndPoint;
		if (hWndPoint == NULL) {
			lstrcpy(str,TEXT("윈도우 없음"));
		} else if (hWndPoint == hWnd) {
			lstrcpy(str,szAbout);
		} else {
			GetWindowText(hWndPoint,sCaption,256);
			GetClassName(hWndPoint,sClass,256);
			hWndParent=GetParent(hWndPoint);
			GetWindowRect(hWndPoint,&crt);
			dwStyle=GetWindowLong(hWndPoint,GWL_STYLE);
			GetStyleString(dwStyle,sStyle);
			wsprintf(str,
				TEXT("핸들 : %d(0x%x)\r\n")
				TEXT("클래스 : %s\r\n")
				TEXT("캡션 : %s\r\n")
				TEXT("부모 : %d(0x%x)\r\n")
				TEXT("영역 : (%d,%d) - (%d,%d), %d*%d\r\n")
				TEXT("스타일 : %X(%s)"),
				hWndPoint,hWndPoint,sClass,sCaption,hWndParent,hWndParent,
				crt.left, crt.top, crt.right, crt.bottom, crt.right-crt.left, 
				crt.bottom-crt.top,dwStyle,sStyle);
		}
		InvalidateRect(hWnd,NULL,TRUE);	
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);

		RoundRect(hdc,0,0,WIDTH-1,HEIGHT-1,20,20);
		// 왼쪽에 타이틀 바를 그린다.
		GetClientRect(hWnd,&crt);
		crt.right=CAPWIDTH;
		FillRect(hdc,&crt,GetSysColorBrush(COLOR_ACTIVECAPTION));

		// 종료 버튼을 그린다.
		Rectangle(hdc,1,5,13,17);
		MoveToEx(hdc,4,8,NULL);LineTo(hdc,10,14);
		MoveToEx(hdc,10,8,NULL);LineTo(hdc,4,14);

		// 정보를 출력한다.
		GetClientRect(hWnd,&crt);
		crt.left+=CAPWIDTH+2;
		crt.top+=2;
		SetBkMode(hdc,TRANSPARENT);
		oldfont=(HFONT)SelectObject(hdc,font);
		DrawText(hdc,str,-1,&crt,DT_WORDBREAK);
		SelectObject(hdc,oldfont);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		pt.x=LOWORD(lParam);
		pt.y=HIWORD(lParam);
		if ((pt.x > 1) && (pt.x < 13) && (pt.y > 5) && (pt.y < 17)) {
			bCloseDown=TRUE;
		} else {
			bCloseDown=FALSE;
		}
		return 0;
	case WM_LBUTTONUP:
		pt.x=LOWORD(lParam);
		pt.y=HIWORD(lParam);
		if ((bCloseDown==TRUE) && (pt.x > 1) && (pt.x < 13) 
			&& (pt.y > 5) && (pt.y < 17)) {
			DestroyWindow(hWnd);
		}
		return 0;
	case WM_NCHITTEST:
		nHit=DefWindowProc(hWnd,iMessage,wParam,lParam);
		pt.x=LOWORD(lParam);
		pt.y=HIWORD(lParam);
		ScreenToClient(hWnd,&pt);
		if ((nHit==HTCLIENT) && (pt.x <= CAPWIDTH) && (pt.y > 15))
			nHit=HTCAPTION;
		return nHit;
	case WM_CONTEXTMENU:
		hMenu=LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU1));
		hPopup=GetSubMenu(hMenu, 0);

		if (bTop==TRUE)
			CheckMenuItem(hPopup, IDM_TOP, MF_BYCOMMAND | MF_CHECKED);

		TrackPopupMenu(hPopup, TPM_LEFTALIGN, LOWORD(lParam), HIWORD(lParam), 
			0, hWnd, NULL);
		DestroyMenu(hMenu);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_TOP:
			bTop=!bTop;
			SetWindowPos(hWnd,(bTop ? HWND_TOPMOST:HWND_NOTOPMOST)
				,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
			break;
		case IDM_ABOUT:
			DialogBox(g_hInst,MAKEINTRESOURCE(IDD_DIALOG1),hWnd,AboutDlgProc);
			break;
		}
		return 0;
	case WM_DESTROY:
		DeleteObject(font);
		// 마지막 위치를 저장해둔다.
		GetWindowRect(hWnd,&crt);
		SHRegWriteInt(SHCU,KEY TEXT("Pos"),TEXT("X"),crt.left);
		SHRegWriteInt(SHCU,KEY TEXT("Pos"),TEXT("Y"),crt.top);
		SHRegWriteInt(SHCU,KEY TEXT("Pos"),TEXT("Top"),bTop);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
