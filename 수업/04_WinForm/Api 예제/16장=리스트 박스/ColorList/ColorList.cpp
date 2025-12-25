#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ColorList");

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

#define ID_LISTBOX 100
HWND hList;
COLORREF ColorTable[15]={RGB(0,0,0), RGB(255,255,255), RGB(255,0,0), RGB(0,255,0),
	RGB(0,0,255), RGB(255,255,0), RGB(255,0,255), RGB(0,255,255), RGB(128,128,128),
	RGB(128,0,0), RGB(0,128,0), RGB(0,0,128), RGB(128,128,0), RGB(128,0,128),
	RGB(0,128,128)
};
COLORREF SelColor=RGB(255,255,255);
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	LPMEASUREITEMSTRUCT lpmis;
	LPDRAWITEMSTRUCT lpdis;
	HBRUSH bkBrush, ColorBrush, OldBrush;
	COLORREF Color;
	int i, index;
	PAINTSTRUCT ps;
	HDC hdc;
	switch (iMessage) {
	case WM_CREATE:
		// 오너 드로우 리스트 박스를 만든다.
		hList=CreateWindow(TEXT("listbox"),NULL,WS_CHILD | WS_VISIBLE | WS_BORDER |
			WS_VSCROLL | LBS_NOTIFY | LBS_OWNERDRAWFIXED,
			10,10,150,200,hWnd,(HMENU)ID_LISTBOX,g_hInst,NULL);

		// 항목 데이터에 색상값을 넣는다.
		for (i=0;i<15;i++) {
			SendMessage(hList,LB_ADDSTRING,0,(LPARAM)ColorTable[i]);
		}
		return 0;
	case WM_MEASUREITEM:
		// 각 항목의 높이는 24픽셀이다.
		lpmis=(LPMEASUREITEMSTRUCT)lParam;
		lpmis->itemHeight=24;
		return TRUE;
	case WM_DRAWITEM:
		lpdis=(LPDRAWITEMSTRUCT)lParam;

		// 선택 상황에 따라 파란색 배경을 출력한다.
		if (lpdis->itemState & ODS_SELECTED) {
			bkBrush=GetSysColorBrush(COLOR_HIGHLIGHT);
		} else {
			bkBrush=GetSysColorBrush(COLOR_WINDOW);
		}
		FillRect(lpdis->hDC, &lpdis->rcItem, bkBrush);

		// 색상 바를 출력한다.
		Color=(COLORREF)lpdis->itemData;
		ColorBrush=CreateSolidBrush(Color);
		OldBrush=(HBRUSH)SelectObject(lpdis->hDC, ColorBrush);
		Rectangle(lpdis->hDC,lpdis->rcItem.left+5,lpdis->rcItem.top+2,
			lpdis->rcItem.right-5, lpdis->rcItem.bottom-2);
		SelectObject(lpdis->hDC, OldBrush);
		DeleteObject(ColorBrush);
		return TRUE;
	case WM_COMMAND:
		// 선택된 항목의 항목 데이터로부터 색상값을 구하고 다시 그린다.
		switch (LOWORD(wParam)) {
		case ID_LISTBOX:
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				index=SendMessage(hList, LB_GETCURSEL, 0, 0);
				SelColor=(COLORREF)SendMessage(hList, LB_GETITEMDATA, index, 0);
				InvalidateRect(hWnd, NULL, TRUE);
			}
		}
		return 0;
	case WM_PAINT:
		// 선택된 색상으로 원을 그린다.
		hdc=BeginPaint(hWnd, &ps);
		ColorBrush=CreateSolidBrush(SelColor);
		OldBrush=(HBRUSH)SelectObject(hdc, ColorBrush);
		Ellipse(hdc,200,10,400,200);
		SelectObject(hdc, OldBrush);
		DeleteObject(ColorBrush);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}