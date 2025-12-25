#include <windows.h>
#include "resource.h"

BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
         ,LPSTR lpszCmdParam,int nCmdShow)
{
	DialogBox(hInstance,MAKEINTRESOURCE(IDD_DIALOG1),HWND_DESKTOP,MainDlgProc);
	return 0;
}

TCHAR *Items[]={TEXT("Apple"),TEXT("Orange"),TEXT("Melon"),
	TEXT("Grape"),TEXT("Great"), TEXT("Strawberry")};
BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	int i;
    switch (iMessage) {
    case WM_INITDIALOG:
		for (i=0;i<sizeof(Items)/sizeof(Items[0]);i++)
			SendDlgItemMessage(hDlg,IDC_LIST1,LB_ADDSTRING,0,(LPARAM)Items[i]);
        return TRUE;
	case WM_VKEYTOITEM:
		if (LOWORD(wParam) == 'D') {
			i=SendDlgItemMessage(hDlg,IDC_LIST1,LB_GETCURSEL,0,0);
			if (i != LB_ERR) {
				SendDlgItemMessage(hDlg,IDC_LIST1,LB_DELETESTRING,i,0);
				return (BOOL)-2;
			}
		}
		return (BOOL)-1;
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
