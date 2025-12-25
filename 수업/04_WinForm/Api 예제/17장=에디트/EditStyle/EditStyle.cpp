#include <windows.h>
#include "resource.h"

BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
         ,LPSTR lpszCmdParam,int nCmdShow)
{
	DialogBox(hInstance,MAKEINTRESOURCE(IDD_DIALOG1),HWND_DESKTOP,MainDlgProc);
	return 0;
}

BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	TCHAR str[128];
    switch (iMessage) {
    case WM_INITDIALOG:
		SetDlgItemText(hDlg,IDC_EDIT12,TEXT("읽기 전용"));
		SetDlgItemText(hDlg,IDC_EDIT4,TEXT("여러 줄을 편집할 수 있는 멀티 라인 ")
			TEXT("에디트입니다.\r\nES_MULTILINE 스타일을 사용하며 스크롤 바를 ")
			TEXT("달 수도 있습니다."));
		// SendDlgItemMessage(hDlg,IDC_EDIT13,EM_SETPASSWORDCHAR,(WPARAM)'#',0);
		return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK:
        case IDCANCEL:
            EndDialog(hDlg,0);
            return TRUE;
		case IDC_BTNREADPASS:
			GetDlgItemText(hDlg,IDC_EDIT13,str,128);
			MessageBox(hDlg,str,"암호",MB_OK);
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}