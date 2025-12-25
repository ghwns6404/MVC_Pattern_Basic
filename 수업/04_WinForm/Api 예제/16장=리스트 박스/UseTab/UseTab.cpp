#include <windows.h>
#include "resource.h"

BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
         ,LPSTR lpszCmdParam,int nCmdShow)
{
	DialogBox(hInstance,MAKEINTRESOURCE(IDD_DIALOG1),HWND_DESKTOP,MainDlgProc);
	return 0;
}

TCHAR *data[3]={TEXT("대한민국\t서울\t한민족"),
	TEXT("일본\t도쿄\t대화족"),
	TEXT("중국\t베이징\t한족")};
int arTab[2]={50,100};
BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	int i;
    switch (iMessage) {
    case WM_INITDIALOG:
		for (i=0;i<3;i++) {
			SendDlgItemMessage(hDlg,IDC_LIST1,LB_ADDSTRING,0,(LPARAM)data[i]);
		}
		SendDlgItemMessage(hDlg,IDC_LIST1,LB_SETTABSTOPS,2,(LPARAM)arTab);
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
