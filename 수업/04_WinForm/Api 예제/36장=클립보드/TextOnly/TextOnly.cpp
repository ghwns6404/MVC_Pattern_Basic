#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("TextOnly");

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;

	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=NULL;
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,500,100,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);

	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

HWND hNext;
BOOL bAuto=FALSE;
int Count;
void TextOnly()
{
	HGLOBAL hmem;
	TCHAR *ptr, *ptr2;
	UINT Format=0;
	BOOL bOther=FALSE;

	// 일단 텍스트가 들어 있어야 한다.
	if (!IsClipboardFormatAvailable(CF_TEXT)) {
		return;
	}

	// 텍스트외에 다른 포맷도 들어 있어야 한다.
	for (;;) {
		OpenClipboard(hWndMain);
		Format=EnumClipboardFormats(Format);
		if (Format==0) break;
		if (Format!=CF_TEXT && Format!=CF_OEMTEXT && 
			Format!=CF_UNICODETEXT && Format!=CF_LOCALE) {
			bOther=TRUE;
			break;
		}
	}
	CloseClipboard();

	// 텍스트만 있다면 변환할 필요가 없다.
	if (bOther==FALSE) {
		return;
	}

	OpenClipboard(hWndMain);
	hmem=GetClipboardData(CF_TEXT);
	ptr=(TCHAR *)GlobalLock(hmem);
	ptr2=(TCHAR *)calloc(lstrlen(ptr)+1,1);
	lstrcpy(ptr2,ptr);
	GlobalUnlock(hmem);
	CloseClipboard();

	hmem=GlobalAlloc(GHND, lstrlen(ptr2)+1);
	ptr=(TCHAR *)GlobalLock(hmem);
	memcpy(ptr,ptr2,lstrlen(ptr2)+1);
	GlobalUnlock(hmem);
	free(ptr2);
	if (OpenClipboard(hWndMain)) {
		EmptyClipboard();
		SetClipboardData(CF_TEXT,hmem);
		CloseClipboard();
	}
	Count++;
	InvalidateRect(hWndMain,NULL,TRUE);
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes="클립보드의 데이터 중 텍스트만 남기고 모두 삭제한다.";
	TCHAR *Mes2="자동 변환 토글 : A, 수동 변환 단축키 : Ctrl+Shift+C";
	TCHAR str[256];

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		RegisterHotKey(hWnd,0x0000,MOD_SHIFT | MOD_CONTROL,'C');
		hNext=SetClipboardViewer(hWnd);
		return 0;
	case WM_CHANGECBCHAIN:
		if ((HWND)wParam==hNext) {
			hNext=(HWND)lParam;
		}
		if (hNext) {
			SendMessage(hNext,WM_CHANGECBCHAIN,wParam,lParam);
		}
		return 0;
	case WM_DRAWCLIPBOARD:
		if (hNext) {
			SendMessage(hNext,WM_DRAWCLIPBOARD,wParam,lParam);
		}
		if (!bAuto) return 0;
	case WM_HOTKEY:
		TextOnly();
		return 0;
	case WM_KEYDOWN:
		if (wParam == 'A') {
			bAuto=!bAuto;
			InvalidateRect(hWnd,NULL,TRUE);
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,5,5,Mes,lstrlen(Mes));
		TextOut(hdc,5,25,Mes2,lstrlen(Mes2));
		wsprintf(str,"%s동 변환 상태. 총 처리 회수 = %d",
			(bAuto ? "자":"수"),Count);
		TextOut(hdc,5,45,str,lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		UnregisterHotKey(hWnd,0);
		ChangeClipboardChain(hWnd,hNext);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
