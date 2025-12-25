#include <windows.h>
#include <wininet.h>
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

// URL에서 파일을 다운로드받아 Path 파일로 저장한다.
BOOL bStopHttpDownLoad, bDownLoading=FALSE;
BOOL HTTPDown(HWND hWnd,TCHAR *URL, TCHAR *Path)
{
	HINTERNET hInternet, hURL;
	HANDLE hFile;
	DWORD Size;
	DWORD dwRead,dwWritten,Recv=0;
	TCHAR buf[100000];
	MSG Message;

	// 인터넷 연결
	hInternet=InternetOpen("HTTPTEST", INTERNET_OPEN_TYPE_PRECONFIG,
		NULL, NULL, 0);
	if (hInternet == NULL) {
		return FALSE;
	}

	// URL을 연다
	hURL=InternetOpenUrl(hInternet,URL,NULL,0,INTERNET_FLAG_RELOAD,0);
	if (hURL==NULL) {
		InternetCloseHandle(hInternet);
		return FALSE;
	}

	// 다운로드중임과 취소는 하지 않음을 전역변수에 기록한다.
	bStopHttpDownLoad=FALSE;
	bDownLoading=TRUE;

	// 로컬 파일을 생성한다.
	hFile=CreateFile(Path,GENERIC_WRITE,0,NULL,
		CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	do {
		// 다운로드받아 로컬 파일에 기록한다.
		InternetQueryDataAvailable(hURL,&Size,0,0);
		InternetReadFile(hURL,buf,Size,&dwRead);
		WriteFile(hFile,buf,dwRead,&dwWritten,NULL);

		// 다운로드받은 양을 메시지로 전달
		Recv+=dwWritten;
		SendMessage(hWnd,WM_USER+10,0,(LPARAM)Recv);

		// 부모 윈도우의 메시지 처리
		if (PeekMessage(&Message,NULL,0,0,PM_REMOVE)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		// 취소 처리
		if (bStopHttpDownLoad == TRUE) {
			SendMessage(hWnd,WM_USER+12,0,(LPARAM)Recv);
			break;
		}
	} while (dwRead != 0);

	InternetCloseHandle(hURL);
	InternetCloseHandle(hInternet);
	CloseHandle(hFile);
	bDownLoading=FALSE;

	// 다운로드 완료 처리(취소한 경우는 제외)
	if (bStopHttpDownLoad == FALSE)
		SendMessage(hWnd,WM_USER+11,0,(LPARAM)Recv);
	return TRUE;
}

BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	TCHAR szURL[MAX_PATH];
	TCHAR szLocal[MAX_PATH];
	TCHAR szTmp[100];

    switch (iMessage) {
    case WM_INITDIALOG:
		SetDlgItemText(hDlg,IDC_URL,"http://www.winapi.co.kr/images/logo.jpg");
		SetDlgItemText(hDlg,IDC_LOCAL,"c:\\logo.jpg");
        hDlgMain = hDlg;
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK:
			if (bDownLoading==FALSE)
	            EndDialog(hDlgMain,0);
            return TRUE;
		case IDC_BTNDOWN:
			if (bDownLoading==FALSE) {
				GetDlgItemText(hDlg,IDC_URL,szURL,MAX_PATH);
				GetDlgItemText(hDlg,IDC_LOCAL,szLocal,MAX_PATH);
				if (HTTPDown(hDlg,szURL,szLocal)==FALSE) {
					MessageBox(hDlg,"인터넷 연결이 되지 않았거나 "
						"URL을 찾을 수 없습니다","에러",MB_OK);
				}
			}
			return TRUE;
		case IDC_BTNCANCEL:
			bStopHttpDownLoad=TRUE;
			return TRUE;
        }
        return FALSE;
	case WM_USER+10:
		wsprintf(szTmp,"%d 바이트 받고 있습니다",lParam);
		SetDlgItemText(hDlg,IDC_STATUS,szTmp);
		return TRUE;
	case WM_USER+11:
		SetDlgItemText(hDlg,IDC_STATUS,"다운로드 완료");
		return TRUE;
	case WM_USER+12:
		SetDlgItemText(hDlg,IDC_STATUS,"다운로드 취소");
		return TRUE;
    }
    return FALSE;
}

