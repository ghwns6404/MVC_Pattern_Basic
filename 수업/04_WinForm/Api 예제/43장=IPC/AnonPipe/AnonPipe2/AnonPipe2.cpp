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
	TCHAR str[128]={0,};
	DWORD dwRead;

    switch (iMessage) {
    case WM_INITDIALOG:
        hDlgMain = hDlg;
		MoveWindow(hDlg,100,300,500,150,TRUE);
        return TRUE;
	case WM_USER+1:
		// 표준 입력으로부터 문자열을 읽어 에디트에 출력한다.
		ReadFile(GetStdHandle(STD_INPUT_HANDLE),str,128,&dwRead,NULL);
		SetWindowText(GetDlgItem(hDlg,IDC_EDIT1),str);
		return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK:
        case IDCANCEL:
            EndDialog(hDlgMain,0);
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}
