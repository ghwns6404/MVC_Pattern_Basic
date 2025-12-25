#include <windows.h>
#include "resource.h"

BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
         ,LPSTR lpszCmdParam,int nCmdShow)
{
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), HWND_DESKTOP, MainDlgProc);
    return 0;
}

TCHAR *arItem[3]={TEXT("∂Û∏È"),TEXT("ã∂∫∫¿Ã"),TEXT("∆¢±Ë")};
BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	int i;
    switch (iMessage) {
    case WM_INITDIALOG:
		for (i=0;i<3;i++) {
			SendDlgItemMessage(hDlg,IDC_COMBO1,CB_ADDSTRING,0,(LPARAM)arItem[i]);
			SendDlgItemMessage(hDlg,IDC_COMBO2,CB_ADDSTRING,0,(LPARAM)arItem[i]);
			SendDlgItemMessage(hDlg,IDC_COMBO3,CB_ADDSTRING,0,(LPARAM)arItem[i]);
		}
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK:
        case IDCANCEL:
            EndDialog(hDlg,0);
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}