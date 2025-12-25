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

int Frame=0;
HICON arIcon[11];
BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	int i;
    switch (iMessage) {
    case WM_INITDIALOG:
		for (i=0;i<11;i++) {
			arIcon[i]=LoadIcon(g_hInst,MAKEINTRESOURCE(IDI_ICON1+i));
		}
		SetTimer(hDlg,1,200,NULL);
        return TRUE;
	case WM_TIMER:
		SendDlgItemMessage(hDlg,IDC_STICON,STM_SETIMAGE,(WPARAM)IMAGE_ICON, 
			(LPARAM)arIcon[Frame]);
		Frame++;
		if (Frame == 11)
			Frame=0;
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