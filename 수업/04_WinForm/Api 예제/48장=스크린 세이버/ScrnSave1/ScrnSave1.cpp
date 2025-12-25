#include <windows.h>
#include <scrnsave.h>

LRESULT WINAPI ScreenSaverProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes = "스크린 세이버 테스트";

	switch (iMessage) {
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(255,255,255));
		TextOut(hdc, 100, 100, Mes, lstrlen(Mes));
		EndPaint(hWnd, &ps);
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
