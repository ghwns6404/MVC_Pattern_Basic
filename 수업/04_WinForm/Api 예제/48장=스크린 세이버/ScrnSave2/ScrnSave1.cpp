#include <windows.h>
#include <scrnsave.h>
#include <time.h>

LRESULT WINAPI ScreenSaverProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	TCHAR *Mes = "스크린 세이버 테스트";
	static int SX, SY;

	switch (iMessage) {
	case WM_CREATE:
		SetTimer(hWnd, 0, 1000, NULL);
		srand(time(NULL));
		SX=GetSystemMetrics(SM_CXSCREEN);
		SY=GetSystemMetrics(SM_CYSCREEN);
		return 0;
	case WM_TIMER:
		hdc=GetDC(hWnd);
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(rand()%256, rand()%256, rand()%256));
		TextOut(hdc, rand()%SX, rand()%SY,Mes,lstrlen(Mes));
		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd,0);
		return 0;
	}
	return DefScreenSaverProc(hWnd, iMessage, wParam, lParam);
}

BOOL WINAPI ScreenSaverConfigureDialog(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage) {
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			EndDialog(hWnd, IDOK);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL WINAPI RegisterDialogClasses(HANDLE hInst)
{
	return TRUE;
}
