#include <windows.h>
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

int idx=0;
BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	TCHAR CurPath[MAX_PATH];
	TCHAR Path[MAX_PATH];
	int i;

	switch (iMessage) {
	case WM_INITDIALOG:
		hDlgMain = hDlg;
		GetCurrentDirectory(MAX_PATH,CurPath);
		for (i=1;i<=5;i++) {
			wsprintf(Path,"%s\\flower%d.bmp",CurPath,i);
			SendDlgItemMessage(hDlg,IDC_BMPLIST,LB_ADDSTRING,idx,(LPARAM)Path);
		}
		SetTimer(hDlg,1,5000,NULL);
		SendMessage(hDlg,WM_TIMER,1,0);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			EndDialog(hDlgMain,0);
			return TRUE;
		case ID_BTNCHANGE:
			SendMessage(hDlg,WM_TIMER,1,0);
			SetTimer(hDlg,1,5000,NULL);
			return TRUE;
		}
		return FALSE;
	case WM_TIMER:
		SendDlgItemMessage(hDlg,IDC_BMPLIST,LB_GETTEXT,idx,(LPARAM)Path);
		if (lstrlen(Path) != 0) {
			SystemParametersInfo(SPI_SETDESKWALLPAPER,0,Path,SPIF_UPDATEINIFILE);
		}
		idx++;
		if (idx >= SendDlgItemMessage(hDlg,IDC_BMPLIST,LB_GETCOUNT,0,0)) {
			idx=0;
		}
		return TRUE;
	case WM_DESTROY:
		KillTimer(hDlg,1);
		return TRUE;
	}
	return FALSE;
}
