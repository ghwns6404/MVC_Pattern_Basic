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

HANDLE hSaveIn, hRead, hWrite, hWriteDup;
BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	SECURITY_ATTRIBUTES sec;
	STARTUPINFO si={sizeof(STARTUPINFO),};
	PROCESS_INFORMATION pi;
	DWORD dwWritten;
	HWND hChild;
	TCHAR str[128];
	
	switch (iMessage) {
	case WM_INITDIALOG:
		hDlgMain = hDlg;
		MoveWindow(hDlg,100,100,500,150,TRUE);

		// 원래의 표준 입력을 저장한다.
		hSaveIn=GetStdHandle(STD_INPUT_HANDLE);

		// 상속 가능한 이름없는 파이프 생성
		sec.nLength=sizeof(SECURITY_ATTRIBUTES);
		sec.bInheritHandle=TRUE;
		sec.lpSecurityDescriptor=NULL;
		CreatePipe(&hRead, &hWrite, &sec, 0);

		// 표준 입력을 읽기 핸들로 재지향하여 상속시킨다. 쓰기 핸들은 상속하지 않는다.
		SetStdHandle(STD_INPUT_HANDLE, hRead);
		DuplicateHandle(GetCurrentProcess(),hWrite,GetCurrentProcess(),
			&hWriteDup,0,FALSE,DUPLICATE_SAME_ACCESS);
		CloseHandle(hWrite);

		// 차일드 생성. 이때 표준 입력 핸들로 읽기 핸들이 상속된다.
		if (CreateProcess(NULL,"AnonPipe2.exe",NULL,NULL,TRUE,0,
			NULL,NULL,&si,&pi)==FALSE) {
			MessageBox(hDlgMain, "차일드를 실행할 수 없습니다","에러",MB_OK);
		}
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		// 읽기 핸들은 닫는다.
		CloseHandle(hRead);

		// 표준 읽기 핸들을 복구시킨다.
		SetStdHandle(STD_INPUT_HANDLE,hSaveIn);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_SEND:
			hChild=FindWindow(NULL,"AnonChild");
			if (hChild == NULL) {
				MessageBox(hDlgMain, "차일드를 찾을 수 없습니다. 다시 실행시켜 주십시오.",
					"에러",MB_OK);
			} else {
				// 에디트의 내용을 쓰기 핸들에 쓴다.
				GetWindowText(GetDlgItem(hDlgMain,IDC_EDIT1),str,128);
				WriteFile(hWriteDup, str, lstrlen(str)+1, &dwWritten,NULL);
				SendMessage(hChild,WM_USER+1,0,0);
				SendMessage(GetDlgItem(hDlgMain,IDC_EDIT1),EM_SETSEL,0,-1);
			}
			return TRUE;
		case IDCANCEL:
			// 핸들을 닫고 차일드도 종료시킨다.
			CloseHandle(hWriteDup);
			hChild=FindWindow(NULL,"AnonChild");
			SendMessage(hChild,WM_CLOSE,0,0);
			EndDialog(hDlgMain,0);
			return TRUE;
		}
		return FALSE;
	}
    return FALSE;
}
