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

BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	unsigned int Num;
	TCHAR Str[128];
	static BOOL EditByCode=FALSE;

	switch (iMessage) {
	case WM_INITDIALOG:
		hDlgMain = hDlg;
		SetFocus(GetDlgItem(hDlg,IDC_EDDEC));
		return FALSE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_EDDEC:
			switch (HIWORD(wParam)) {
			case EN_CHANGE:
				if (EditByCode==FALSE) {
					EditByCode=TRUE;
					Num=GetDlgItemInt(hDlg,IDC_EDDEC,NULL,FALSE);
					itoa(Num,Str,16);
					SetDlgItemText(hDlg,IDC_EDHEX,Str);
					itoa(Num,Str,2);
					SetDlgItemText(hDlg,IDC_EDBIN,Str);
					EditByCode=FALSE;
				}
				return TRUE;
			}
			break;
		case IDC_EDHEX:
			switch (HIWORD(wParam)) {
			case EN_CHANGE:
				if (EditByCode==FALSE) {
					EditByCode=TRUE;
					GetDlgItemText(hDlg,IDC_EDHEX,Str,128);
					Num=strtoul(Str,NULL,16);
					SetDlgItemInt(hDlg,IDC_EDDEC,Num,FALSE);
					itoa(Num,Str,2);
					SetDlgItemText(hDlg,IDC_EDBIN,Str);
					EditByCode=FALSE;
				}
				return TRUE;
			}
			break;
		case IDCANCEL:
			EndDialog(hDlg,IDCANCEL);
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}
