#include <windows.h>
#include "resource.h"

BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);
HINSTANCE g_hInst;

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
         ,LPSTR lpszCmdParam,int nCmdShow)
{
    g_hInst=hInstance;
    DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), HWND_DESKTOP, MainDlgProc);
    return 0;
}

BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	static HBITMAP hBtnBit;
	HICON hIcon;
	switch (iMessage) {
	case WM_INITDIALOG:
		hBtnBit=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP1));
		hIcon=LoadIcon(g_hInst,MAKEINTRESOURCE(IDI_ICON1));
		SendDlgItemMessage(hDlg,IDC_BUTTON1,BM_SETIMAGE,
			(WPARAM)IMAGE_BITMAP,(LPARAM)hBtnBit);
		SendDlgItemMessage(hDlg,IDC_BUTTON2,BM_SETIMAGE,
			(WPARAM)IMAGE_ICON,(LPARAM)hIcon);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BUTTON1:
			MessageBox(hDlg,TEXT("비트맵 버튼을 클릭했습니다"),
				TEXT("알림"),MB_OK);
			return TRUE;
		case IDC_BUTTON2:
			MessageBox(hDlg,TEXT("아이콘 버튼을 클릭했습니다"),
				TEXT("알림"),MB_OK);
			return TRUE;
		case IDOK:
		case IDCANCEL:
			DeleteObject(hBtnBit);
			EndDialog(hDlg,0);
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}
