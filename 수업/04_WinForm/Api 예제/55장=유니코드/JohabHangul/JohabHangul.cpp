#include <windows.h>
#include "JohabUtil.h"
#include "resource.h"

BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);
HINSTANCE g_hInst;
HWND hDlgMain;

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	,LPSTR lpszCmdParam,int nCmdShow)
{
	g_hInst=hInstance;
	DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), HWND_DESKTOP, MainDlgProc);
	return 0;
}

BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	TCHAR Text[1024],Text2[1024];
	TCHAR szCount[128];

	switch (iMessage) {
	case WM_INITDIALOG:
		hDlgMain = hDlg;
		SetDlgItemText(hDlg,IDC_STCOUNT,"문장을 입력하세요");
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			GetDlgItemText(hDlg,IDC_EDSRC,Text,1024);
			BreakHangul(Text,Text2);
			SetDlgItemText(hDlg,IDC_EDDEST,Text2);
			wsprintf(szCount,"총 글자 수 : %d",GetHanStringCount(Text));
			SetDlgItemText(hDlg,IDC_STCOUNT,szCount);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg,IDCANCEL);
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}
