#include <windows.h>
#include <commctrl.h>
#include "resource.h"

BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);
HINSTANCE g_hInst;
HWND hDlgMain;

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
         ,LPSTR lpszCmdParam,int nCmdShow)
{
	g_hInst=hInstance;
	INITCOMMONCONTROLSEX icex;

	icex.dwSize=sizeof(icex);
	icex.dwICC=ICC_DATE_CLASSES;
	InitCommonControlsEx(&icex);
		
	DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), HWND_DESKTOP, MainDlgProc);

    return 0;
}

// st 날짜의 절대 날짜를 구한다. 1601년 1월 1일을 기준(0일)으로 경과한 날짜 수를 세 준다.
DWORD MyGetAbsDay(SYSTEMTIME st)
{
	INT64 i64;
	FILETIME fst;

	st.wHour=st.wMinute=st.wSecond=st.wMilliseconds=st.wDayOfWeek=0;
	SystemTimeToFileTime(&st,&fst);
	i64=(((INT64)fst.dwHighDateTime) << 32) + fst.dwLowDateTime;
	i64 = i64 / 864000000000;
	return (DWORD)i64;
}

// 절대 날짜를 시스템 타임으로 바꾼다.
void MyAbsToSystem(DWORD Abs, SYSTEMTIME &st)
{
	INT64 i64;
	FILETIME fst;

	i64=Abs * 864000000000;
	fst.dwHighDateTime = (DWORD)(i64 >> 32);
	fst.dwLowDateTime = (DWORD)(i64 & 0xffffffff);
	FileTimeToSystemTime(&fst, &st);
}

void CalcDate()
{
	HWND hStart, hEnd, hResult;
	SYSTEMTIME sStart, sEnd;
	DWORD dStart, dEnd,dEllipse;
	TCHAR str[255];

	hStart=GetDlgItem(hDlgMain,IDC_DTPSTART);
	hEnd=GetDlgItem(hDlgMain,IDC_DTPEND);
	hResult=GetDlgItem(hDlgMain,IDC_RESULT);

	DateTime_GetSystemtime(hStart,&sStart);
	DateTime_GetSystemtime(hEnd,&sEnd);
	dStart=MyGetAbsDay(sStart);
	dEnd=MyGetAbsDay(sEnd);
	dEllipse=dEnd-dStart;

	wsprintf(str,"경과일은 총 %d일입니다.",dEllipse);
	SetWindowText(hResult,str);
}

BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
    switch (iMessage) {
    case WM_INITDIALOG:
        hDlgMain = hDlg;
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK:
			CalcDate();
			return TRUE;
        case IDCANCEL:
            EndDialog(hDlgMain,0);
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}
