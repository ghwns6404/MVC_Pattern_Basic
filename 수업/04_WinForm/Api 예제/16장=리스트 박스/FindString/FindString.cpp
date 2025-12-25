#include <windows.h>
#include "resource.h"

BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
         ,LPSTR lpszCmdParam,int nCmdShow)
{
	DialogBox(hInstance,MAKEINTRESOURCE(IDD_DIALOG1),HWND_DESKTOP,MainDlgProc);
	return 0;
}

TCHAR *Items[]={"abacus","abadon","able","abort","about","above","absolute",
	"acacia","accent","accept","access","accident","account","ace","achieve",
	"acid","acquire","across","act","active","actual","ada"};
BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	int i;
	TCHAR str[128];
	int idx;
    switch (iMessage) {
    case WM_INITDIALOG:
		for (i=0;i<sizeof(Items)/sizeof(Items[0]);i++) {
			SendDlgItemMessage(hDlg,IDC_LIST1,LB_ADDSTRING,0,(LPARAM)Items[i]);
		}
        return TRUE;
    case WM_COMMAND:
		GetWindowText(GetDlgItem(hDlg,IDC_EDIT1),str,128);
		idx=SendDlgItemMessage(hDlg,IDC_LIST1,LB_GETCURSEL,0,0);
        switch (LOWORD(wParam)) {
        case IDOK:
        case IDCANCEL:
            EndDialog(hDlg,0);
            return TRUE;
		case IDC_BTNFIND:
			idx=SendDlgItemMessage(hDlg,IDC_LIST1,LB_FINDSTRING,idx,(LPARAM)str);
			SendDlgItemMessage(hDlg,IDC_LIST1,LB_SETCURSEL,idx,0);
			return TRUE;
		case IDC_BTNFINDEXACT:
			idx=SendDlgItemMessage(hDlg,IDC_LIST1,LB_FINDSTRINGEXACT,idx,(LPARAM)str);
			SendDlgItemMessage(hDlg,IDC_LIST1,LB_SETCURSEL,idx,0);
			return TRUE;
		case IDC_BTNSEL:
			idx=SendDlgItemMessage(hDlg,IDC_LIST1,LB_SELECTSTRING,idx,(LPARAM)str);
			return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}