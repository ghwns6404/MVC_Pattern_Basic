#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("PropSheet");

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

#include "resource.h"
#include <commctrl.h>
int Shape=0;					// 도형의 모양, 0=사각형, 1=타원
COLORREF Col=RGB(255,0,0);		// 도형의 색상

// 모양 선택 페이지의 대화상자 프로시저
BOOL CALLBACK ShapeDlgProc(HWND hDlg,WORD iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage) {
	case WM_INITDIALOG:
		return TRUE;
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code) {
		// 페이지가 활성화될 때 선택된 도형 라디오 박스를 체크한다.
		case PSN_SETACTIVE:
			SendMessage(GetDlgItem(hDlg, IDC_RADIO1), BM_SETCHECK, BST_UNCHECKED,0);
			SendMessage(GetDlgItem(hDlg, IDC_RADIO2), BM_SETCHECK, BST_UNCHECKED,0);
			switch (Shape) {
			case 0:
				SendMessage(GetDlgItem(hDlg, IDC_RADIO1), BM_SETCHECK, BST_CHECKED,0);
				break;
			case 1:
				SendMessage(GetDlgItem(hDlg, IDC_RADIO2), BM_SETCHECK, BST_CHECKED,0);
				break;
			}
			break;
		// 적용 버튼을 누르거나 다른 페이지로 이동할 때 메인 윈도우를 다시 그리도록 한다.
		case PSN_APPLY:
		case PSN_KILLACTIVE:
			InvalidateRect(hWndMain, NULL, TRUE);
			break;
/*
		case PSN_WIZNEXT:
			SendMessage(GetParent(hDlg), PSM_SETWIZBUTTONS, 0, PSWIZB_BACK | PSWIZB_FINISH);
//*/
		default:
			return FALSE;
		}
		return TRUE;
	// 도형의 모양을 변경한다.
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_RADIO1:
			Shape=0;
			break;
		case IDC_RADIO2:
			Shape=1;
			break;
		}
		return TRUE;
	}
	return FALSE;
}

// 색상 선택 페이지의 대화상자 프로시저
BOOL CALLBACK ColorDlgProc(HWND hDlg,WORD iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage) {
	case WM_INITDIALOG:
		return TRUE;
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code) {
		// 페이지가 활성화될 때 선택된 색상의 라디오 박스를 체크한다.
		case PSN_SETACTIVE:
			SendMessage(GetDlgItem(hDlg, IDC_RADIO1), BM_SETCHECK, BST_UNCHECKED,0);
			SendMessage(GetDlgItem(hDlg, IDC_RADIO2), BM_SETCHECK, BST_UNCHECKED,0);
			SendMessage(GetDlgItem(hDlg, IDC_RADIO3), BM_SETCHECK, BST_UNCHECKED,0);
			switch (Col) {
			case RGB(255,0,0):
				SendMessage(GetDlgItem(hDlg, IDC_RADIO1), BM_SETCHECK, BST_CHECKED,0);
				break;
			case RGB(0,255,0):
				SendMessage(GetDlgItem(hDlg, IDC_RADIO2), BM_SETCHECK, BST_CHECKED,0);
				break;
			case RGB(0,0,255):
				SendMessage(GetDlgItem(hDlg, IDC_RADIO3), BM_SETCHECK, BST_CHECKED,0);
				break;
			}
			break;
		// 적용 버튼을 누르거나 다른 페이지로 이동할 때 메인 윈도우를 다시 그리도록 한다.
		case PSN_APPLY:
		case PSN_KILLACTIVE:
			InvalidateRect(hWndMain, NULL, TRUE);
			break;
/*
		case PSN_WIZBACK:
			SendMessage(GetParent(hDlg), PSM_SETWIZBUTTONS, 0, PSWIZB_BACK | PSWIZB_NEXT);
//*/
		default:
			return FALSE;
		}
		return TRUE;
	// 색상을 변경한다.
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_RADIO1:
			Col=RGB(255,0,0);
			break;
		case IDC_RADIO2:
			Col=RGB(0,255,0);
			break;
		case IDC_RADIO3:
			Col=RGB(0,0,255);
			break;
		}
		return TRUE;
	}
	return FALSE;
}

// 메인 윈도우의 메시지 처리 루틴
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	HBRUSH hBrush, hOldBrush;
	PROPSHEETPAGE psp[2];
	PROPSHEETHEADER psh;
	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		InitCommonControls();
		return 0;
	// 프로퍼티 시트를 호출한다.
	case WM_LBUTTONDOWN:
		// 모양 페이지
		psp[0].dwSize=sizeof(PROPSHEETPAGE);
		psp[0].dwFlags=PSP_DEFAULT;
		psp[0].hInstance=g_hInst;
		psp[0].pszTemplate=MAKEINTRESOURCE(IDD_SHAPEDLG);
		psp[0].pfnDlgProc=(DLGPROC)ShapeDlgProc;
		psp[0].lParam=0;

		// 색상 페이지
		psp[1].dwSize=sizeof(PROPSHEETPAGE);
		psp[1].dwFlags=PSP_DEFAULT;
		psp[1].hInstance=g_hInst;
		psp[1].pszTemplate=MAKEINTRESOURCE(IDD_COLORDLG);
		psp[1].pfnDlgProc=(DLGPROC)ColorDlgProc;
		psp[1].lParam=0;

		// 프로퍼티 시트
		psh.dwSize=sizeof(PROPSHEETHEADER);
		psh.dwFlags=PSH_PROPSHEETPAGE;
		//psh.dwFlags=PSH_PROPSHEETPAGE | PSH_WIZARD | PSH_NOAPPLYNOW;
		psh.hwndParent=hWndMain;
		psh.pszCaption="모양과 색상";
		psh.nPages=2;
		psh.nStartPage=0;
		psh.ppsp=(LPCPROPSHEETPAGE)psp;

		PropertySheet(&psh);
		return 0;
	// 선택된 도형을 선택된 모양으로 그린다.
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		hBrush=CreateSolidBrush(Col);
		hOldBrush=(HBRUSH)SelectObject(hdc, hBrush);
		if (Shape==0) {
			Rectangle(hdc,10,10,200,200);
		} else {
			Ellipse(hdc,10,10,200,200);
		}
		SelectObject(hdc, hOldBrush);
		DeleteObject(hBrush);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

