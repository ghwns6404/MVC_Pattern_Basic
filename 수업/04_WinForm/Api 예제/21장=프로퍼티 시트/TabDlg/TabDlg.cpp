#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
BOOL CALLBACK Dlg1Proc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);
BOOL CALLBACK Dlg2Proc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("TabDlg");

#include <commctrl.h>
#include "resource.h"
HWND hTab;
HWND hTabDlg;

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

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	
	while (GetMessage(&Message,NULL,0,0)) {
		if (!IsWindow(hTabDlg) || !IsDialogMessage(hTabDlg,&Message)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}
	return (int)Message.wParam;
}

typedef struct {  
  WORD      dlgVer; 
  WORD      signature; 
  DWORD     helpID; 
  DWORD     exStyle; 
  DWORD     style; 
  WORD      cDlgItems; 
  short     x; 
  short     y; 
  short     cx; 
  short     cy; 
//  sz_Or_Ord menu; 
//  sz_Or_Ord windowClass; 
//  WCHAR     title[titleLen]; 
//  WORD     pointsize; 
//  WORD     weight; 
//  BYTE     italic;
//  BYTE     charset; 
//  WCHAR    typeface[stringLen];  
} DLGTEMPLATEEX; 

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	TCITEM tie;
	int Sel;
	HRSRC hRc;
	HGLOBAL hGlb;
	DLGTEMPLATEEX *dTem;
	RECT trt={0,0,0,0};

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		InitCommonControls();

		hTab=CreateWindow(WC_TABCONTROL,"",WS_CHILD | WS_VISIBLE 
			| WS_CLIPSIBLINGS,
			0,0,0,0,hWnd,(HMENU)0,g_hInst,NULL);

		tie.mask=TCIF_TEXT;
		tie.pszText="one";
		TabCtrl_InsertItem(hTab,0,&tie);
		tie.pszText="two";
		TabCtrl_InsertItem(hTab,1,&tie);

		// 첫번째 대화상자의 크기 구함
		hRc=FindResource(NULL,MAKEINTRESOURCE(IDD_DIALOG1),RT_DIALOG);
		hGlb=LoadResource(NULL,hRc);
		dTem=(DLGTEMPLATEEX *)LockResource(hGlb);
		trt.right=dTem->cx;
		trt.bottom=dTem->cy;

		// 두번째 대화상자의 크기 구하고 둘 중 큰 값을 취함
		hRc=FindResource(NULL,MAKEINTRESOURCE(IDD_DIALOG2),RT_DIALOG);
		hGlb=LoadResource(NULL,hRc);
		dTem=(DLGTEMPLATEEX *)LockResource(hGlb);
		trt.right=max(trt.right,dTem->cx);
		trt.bottom=max(trt.bottom,dTem->cy);

		// 대화상자 단위를 픽셀 단위로 바꾼다.
		hTabDlg=CreateDialog(g_hInst,MAKEINTRESOURCE(IDD_DIALOG1),hWnd,Dlg1Proc);
		MapDialogRect(hTabDlg,&trt);
		DestroyWindow(hTabDlg);

		// 표시 영역을 윈도우 크기로 바꾼다.
		TabCtrl_AdjustRect(hTab,TRUE,&trt);

		// 윈도우 좌표를 원점으로 옮긴다.
		OffsetRect(&trt,-trt.left,-trt.top);

		// 계산된 크기만큼 탭 컨트롤 크기 변경
		SetWindowPos(hTab,NULL,trt.left,trt.top,trt.right,trt.bottom,SWP_NOZORDER);

		hTabDlg=CreateDialog(g_hInst,MAKEINTRESOURCE(IDD_DIALOG1),hWnd,Dlg1Proc);
		return 0;
	case WM_NOTIFY:
		switch (((LPNMHDR)lParam)->code) {
		case TCN_SELCHANGE:
			if (hTabDlg) {
				DestroyWindow(hTabDlg);
			}
			Sel=TabCtrl_GetCurSel(hTab);
			if (Sel==0) {
				hTabDlg=CreateDialog(g_hInst,MAKEINTRESOURCE(IDD_DIALOG1),hWnd,Dlg1Proc);
			} else {
				hTabDlg=CreateDialog(g_hInst,MAKEINTRESOURCE(IDD_DIALOG2),hWnd,Dlg2Proc);
			}
			break;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

// 대화상자는 반드시 차일드 스타일이어야 하며 경계선을 가지지 않는다.
BOOL CALLBACK Dlg1Proc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	RECT prt;
	switch (iMessage) {
	case WM_INITDIALOG:
		// 탭의 표시 영역 상단으로 이동하며 형제중에 제일 위로 올라간다.
		GetWindowRect(hTab,&prt);
		TabCtrl_AdjustRect(hTab,FALSE,&prt);
		ScreenToClient(hWndMain,(LPPOINT)&prt);
		SetWindowPos(hDlg,HWND_TOP,prt.left,prt.top,0,0,SWP_NOSIZE);
		SendDlgItemMessage(hDlg,IDC_COMBO1,CB_ADDSTRING,0,(LPARAM)"짜장면");
		SendDlgItemMessage(hDlg,IDC_COMBO1,CB_ADDSTRING,0,(LPARAM)"탕수육");
		SendDlgItemMessage(hDlg,IDC_COMBO1,CB_ADDSTRING,0,(LPARAM)"갈비탕");
		SendDlgItemMessage(hDlg,IDC_COMBO1,CB_ADDSTRING,0,(LPARAM)"육계장");
		SendDlgItemMessage(hDlg,IDC_COMBO1,CB_SETCURSEL,0,0);
		return TRUE;
	// EndDialog를 호출할 필요가 없다.
	case WM_COMMAND:
		switch (LOWORD(wParam))	{
		case IDC_BUTTON1:
			MessageBox(hDlg,"버튼을 눌렀습니다","알림",MB_OK);
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}

BOOL CALLBACK Dlg2Proc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	RECT prt;
	switch (iMessage) {
	case WM_INITDIALOG:
		SetWindowPos(hDlg,HWND_TOP,100,100,0,0,SWP_NOSIZE);
		GetWindowRect(hTab,&prt);
		TabCtrl_AdjustRect(hTab,FALSE,&prt);
		ScreenToClient(hWndMain,(LPPOINT)&prt);
		SetWindowPos(hDlg,HWND_TOP,prt.left,prt.top,0,0,SWP_NOSIZE);
		SetWindowText(GetDlgItem(hDlg,IDC_EDIT1),"에디트 컨트롤이다");
		return TRUE;
	}
	return FALSE;
}