#include <windows.h>
#include <scrnsave.h>
#include <time.h>
#include "resource.h"
#include "ShReg.h"

#define KEY "Software\\SangHyungSoft\\TestScrnSave\\"
TCHAR g_Mes[256];
int g_Speed;

LRESULT WINAPI ScreenSaverProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	static int SX, SY;

	switch (iMessage) {
	case WM_CREATE:
		SHRegReadString(HKEY_CURRENT_USER,KEY"Setting","String","테스트 스크린 세이버",g_Mes,256);
		g_Speed = SHRegReadInt(HKEY_CURRENT_USER,KEY"Setting","Speed",2);
		switch (g_Speed) {
		case 1:
			SetTimer(hWnd, 0, 2000, NULL);
			break;
		case 2:
			SetTimer(hWnd, 0, 1000, NULL);
			break;
		case 3:
			SetTimer(hWnd, 0, 300, NULL);
			break;
		}
		srand(time(NULL));
		SX=GetSystemMetrics(SM_CXSCREEN);
		SY=GetSystemMetrics(SM_CYSCREEN);
		return 0;
	case WM_TIMER:
		hdc=GetDC(hWnd);
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(rand()%256, rand()%256, rand()%256));
		TextOut(hdc, rand()%SX, rand()%SY,g_Mes,lstrlen(g_Mes));
		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd,0);
		PostQuitMessage(0);
		return 0;
	}
	return DefScreenSaverProc(hWnd, iMessage, wParam, lParam);
}

BOOL WINAPI ScreenSaverConfigureDialog(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	TCHAR sStr[256];
	int nSpeed;

	switch (iMessage) {
	case WM_INITDIALOG:
		SHRegReadString(HKEY_CURRENT_USER,KEY"Setting","String","테스트 스크린 세이버",sStr,256);
		SetDlgItemText(hWnd, IDC_EDSTRING, sStr);
		nSpeed = SHRegReadInt(HKEY_CURRENT_USER,KEY"Setting","Speed",2);
		if (nSpeed == 1) 
			SendMessage(GetDlgItem(hWnd, IDC_RASPEED1), BM_SETCHECK, BST_CHECKED,0);
		if (nSpeed == 2) 
			SendMessage(GetDlgItem(hWnd, IDC_RASPEED2), BM_SETCHECK, BST_CHECKED,0);
		if (nSpeed == 3) 
			SendMessage(GetDlgItem(hWnd, IDC_RASPEED3), BM_SETCHECK, BST_CHECKED,0);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			GetDlgItemText(hWnd, IDC_EDSTRING, sStr, 256);
			SHRegWriteString(HKEY_CURRENT_USER,KEY"Setting","String",sStr);
			if (SendMessage(GetDlgItem(hWnd, IDC_RASPEED1), BM_GETCHECK,0,0) == BST_CHECKED) 
				nSpeed = 1;
			if (SendMessage(GetDlgItem(hWnd, IDC_RASPEED2), BM_GETCHECK,0,0) == BST_CHECKED) 
				nSpeed = 2;
			if (SendMessage(GetDlgItem(hWnd, IDC_RASPEED3), BM_GETCHECK,0,0) == BST_CHECKED) 
				nSpeed = 3;
			SHRegWriteInt(HKEY_CURRENT_USER,KEY"Setting","Speed",nSpeed);
			EndDialog(hWnd, IDOK);
			return TRUE;
		case IDCANCEL:
			EndDialog(hWnd, IDCANCEL);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL WINAPI CustomDlgProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage) {
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			EndDialog(hWnd, 0);
			return TRUE;
		}
	}
	return FALSE;
}


BOOL WINAPI RegisterDialogClasses(HANDLE hInst)
{
	DialogBox(hMainInstance, MAKEINTRESOURCE(IDD_DIALOG1),hMainWindow, CustomDlgProc);
	return FALSE;
//	return TRUE;
}

