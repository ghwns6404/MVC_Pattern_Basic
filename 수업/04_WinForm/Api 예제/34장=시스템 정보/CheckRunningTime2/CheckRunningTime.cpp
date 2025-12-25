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

// st 날짜의 절대 초를 구한다. 2000년 1월 1일 자정 기준
DWORD MyGetAbsSecond(SYSTEMTIME st)
{
	INT64 i64;
	FILETIME fst;

	SystemTimeToFileTime(&st,&fst);
	i64=(((INT64)fst.dwHighDateTime) << 32) + fst.dwLowDateTime;
	// 초단위로 환산하고 2000년 1월 1일 자정 기준으로 바꾼다.
	i64 = i64 / 10000000 - (INT64)145731 * 86400;

	return (DWORD)i64;
}

// 절대초를 시스템 타임으로 바꾼다.
void MyAbsSecondToSystem(DWORD Abs, SYSTEMTIME &st)
{
	INT64 i64;
	FILETIME fst;

	i64=(Abs + (INT64)145731 * 86400)*10000000;
	fst.dwHighDateTime = (DWORD)(i64 >> 32);
	fst.dwLowDateTime = (DWORD)(i64 & 0xffffffff);
	FileTimeToSystemTime(&fst, &st);
}

void CheckBootTime()
{
	TCHAR str[255];
	LARGE_INTEGER now, freq;
	SYSTEMTIME st,boot;
	int Sec, Min, Hour, Day;
	DWORD abs;

	// 부팅 후 경과한 시간을 초 단위로 구한다.
	QueryPerformanceCounter(&now);
	QueryPerformanceFrequency(&freq);
	Sec=(int)(now.QuadPart / freq.QuadPart);
	Day=Sec/86400;
	Hour=(Sec % 86400)/3600;
	Min=(Sec % 3600)/60;

	// 현재 시간에서 경과 시간을 빼서 부팅한 시간을 구한다.
	GetLocalTime(&st);
	abs=MyGetAbsSecond(st);
	MyAbsSecondToSystem(abs-Sec,boot);

	// 부팅 시간 출력
	wsprintf(str, "부팅시간:%d월 %d일 %d시 %d분", 
		boot.wMonth, boot.wDay, boot.wHour, boot.wMinute);
	SetDlgItemText(hDlgMain,IDC_BOOT,str);

	// 경과 시간 출력
	wsprintf(str, "경과시간:%d일 %d시 %d분 %d초",
		Day, Hour, Min, Sec % 60);
	SetDlgItemText(hDlgMain,IDC_TIME,str);
}

BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage) {
	case WM_INITDIALOG:
		SetTimer(hDlg,1,5000,NULL);
		hDlgMain = hDlg;
		CheckBootTime();
		return TRUE;
	case WM_LBUTTONDOWN:
		return DefWindowProc(hDlg, WM_NCLBUTTONDOWN, HTCAPTION, lParam);
	case WM_TIMER:
		CheckBootTime();
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDCANCEL:
			KillTimer(hDlg,1);
			EndDialog(hDlg,0);
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}
