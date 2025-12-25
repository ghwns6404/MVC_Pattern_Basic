#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("FontCombo");

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

#define ID_COMBOBOX 100
HWND hCombo;
LOGFONT logfont[500];			// 폰트 정보 배열
int num=0;						// 발견된 폰트 수

// 발견된 폰트의 정보를 배열에 복사한다.
int CALLBACK EnumFamCallBack(ENUMLOGFONT FAR *lpelf, NEWTEXTMETRIC FAR *lpntm, 
							 int FontType, LPARAM lParam)
{
	// 최대 500개까지의 폰트를 조사할 수 있다. 
	if (num < 500) {
		logfont[num] = lpelf->elfLogFont;
		num++;
		return TRUE;
	} else {
		return FALSE;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HFONT font, oldfont;
	TCHAR str[128];
	int i,idx;
	LPMEASUREITEMSTRUCT lpmis;
	LPDRAWITEMSTRUCT lpdis;
	HBRUSH bkBrush;
	LOGFONT tmpFont;

	switch (iMessage) {
	case WM_CREATE:
		// 오너 드로우 콤보 박스 생성
		hCombo=CreateWindow(TEXT("combobox"),NULL,WS_CHILD | WS_VISIBLE 
			| CBS_DROPDOWNLIST | WS_VSCROLL | CBS_OWNERDRAWFIXED,
			10,10,400,400,hWnd,(HMENU)ID_COMBOBOX,g_hInst,NULL);

		// 설치되어 있는 폰트의 목록을 조사한다.
		hdc=GetDC(hWnd);
		EnumFontFamilies(hdc, NULL, (FONTENUMPROC)EnumFamCallBack, (LPARAM)NULL);
		ReleaseDC(hWnd, hdc);

		// 발견된 폰트 개수만큼 콤보 박스에 항목을 삽입한다.
		SendMessage(hCombo, CB_RESETCONTENT,0,0);
		for (i=0;i<num;i++) {
			SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)&logfont[i]);
		}
		return 0;
	case WM_MEASUREITEM:
		// 각 항목의 높이는 30픽셀로 설정한다.
		lpmis=(LPMEASUREITEMSTRUCT)lParam;
		lpmis->itemHeight=30;
		return TRUE;
	case WM_DRAWITEM:
		lpdis=(LPDRAWITEMSTRUCT)lParam;

		// 선택된 항목은 파란 배경에 흰색으로 출력한다.
		if (lpdis->itemState & ODS_SELECTED) {
			bkBrush=GetSysColorBrush(COLOR_HIGHLIGHT);
			SetBkColor(lpdis->hDC, GetSysColor(COLOR_HIGHLIGHT));
			SetTextColor(lpdis->hDC, GetSysColor(COLOR_WINDOW));
		} else {
			bkBrush=GetSysColorBrush(COLOR_WINDOW);
			SetBkColor(lpdis->hDC, GetSysColor(COLOR_WINDOW));
			SetTextColor(lpdis->hDC, GetSysColor(COLOR_WINDOWTEXT));
		}
		FillRect(lpdis->hDC, &lpdis->rcItem, bkBrush);

		// 25 픽셀 높이로 글꼴 이름을 출력한다.
		tmpFont=logfont[lpdis->itemID];
		tmpFont.lfHeight=25;
		tmpFont.lfWidth=0;
		font=CreateFontIndirect(&tmpFont);
		oldfont=(HFONT)SelectObject(lpdis->hDC,font);

		TextOut(lpdis->hDC,lpdis->rcItem.left+5, lpdis->rcItem.top+2,
			logfont[lpdis->itemID].lfFaceName, lstrlen(logfont[lpdis->itemID].lfFaceName));
		SelectObject(lpdis->hDC,oldfont);
		DeleteObject(font);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_COMBOBOX:
			switch (HIWORD(wParam)) {
			case CBN_SELCHANGE:
				InvalidateRect(hWnd,NULL,TRUE);
				return 0;
			}
			break;
		}
		break;
	// 80픽셀 크기로 큼직하게 샘플 문자열을 출력한다.
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		idx=SendMessage(hCombo,CB_GETCURSEL,0,0);
		if (idx != -1) {
			tmpFont=logfont[idx];
			tmpFont.lfHeight=80;
			tmpFont.lfWidth=0;
			font=CreateFontIndirect(&tmpFont);
			oldfont=(HFONT)SelectObject(hdc,font);
			wsprintf(str, TEXT("선택 Font:%s"), tmpFont.lfFaceName);
			TextOut(hdc, 10,100,str,lstrlen(str));
			SelectObject(hdc,oldfont);
			DeleteObject(font);
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
