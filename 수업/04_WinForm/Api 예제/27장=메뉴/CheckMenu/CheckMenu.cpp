#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("CheckMenu");

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
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);

	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

BOOL Top=FALSE;
enum tag_View { TEXT, IMAGE, HEXA } View=TEXT;
BOOL PreView=FALSE;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static TCHAR NowView[128]="텍스트 보기";
	TCHAR Mes[256];

	switch (iMessage) {
	case WM_INITMENU:
		if (Top)
			CheckMenuItem((HMENU)wParam, ID_VIEW_TOP, MF_BYCOMMAND | MF_CHECKED);
		else 
			CheckMenuItem((HMENU)wParam, ID_VIEW_TOP, MF_BYCOMMAND | MF_UNCHECKED);

/* 체크 표시하기
		if (View == TEXT)
			CheckMenuItem((HMENU)wParam, ID_VIEW_TEXT, MF_BYCOMMAND | MF_CHECKED);
		else 
			CheckMenuItem((HMENU)wParam, ID_VIEW_TEXT, MF_BYCOMMAND | MF_UNCHECKED);
		
		if (View == IMAGE)
			CheckMenuItem((HMENU)wParam, ID_VIEW_IMAGE, MF_BYCOMMAND | MF_CHECKED);
		else 
			CheckMenuItem((HMENU)wParam, ID_VIEW_IMAGE, MF_BYCOMMAND | MF_UNCHECKED);

		if (View == HEXA)
			CheckMenuItem((HMENU)wParam, ID_VIEW_HEXA, MF_BYCOMMAND | MF_CHECKED);
		else 
			CheckMenuItem((HMENU)wParam, ID_VIEW_HEXA, MF_BYCOMMAND | MF_UNCHECKED);
//*/
//* 라디오 그룹으로 표시하기
		CheckMenuRadioItem(GetSubMenu((HMENU)wParam, 0),2,4,View+2,MF_BYPOSITION);
//*/

		if (PreView)
			CheckMenuItem((HMENU)wParam, ID_VIEW_PRE, MF_BYCOMMAND | MF_CHECKED);
		else 
			CheckMenuItem((HMENU)wParam, ID_VIEW_PRE, MF_BYCOMMAND | MF_UNCHECKED);
		if (View == IMAGE)
			EnableMenuItem((HMENU)wParam, ID_VIEW_PRE, MF_BYCOMMAND | MF_ENABLED);
		else 
			EnableMenuItem((HMENU)wParam, ID_VIEW_PRE, MF_BYCOMMAND | MF_GRAYED);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_VIEW_TOP:
			Top = !Top;
			SetWindowPos(hWnd,Top ? HWND_TOPMOST:HWND_NOTOPMOST,
				0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
			break;
		case ID_VIEW_TEXT:
			View=TEXT;
			lstrcpy(NowView,"텍스트 보기");
			break;
		case ID_VIEW_IMAGE:
			View=IMAGE;
			lstrcpy(NowView,"이미지 보기");
			break;
		case ID_VIEW_HEXA:
			View=HEXA;
			lstrcpy(NowView,"헥사 보기");
			break;
		case ID_VIEW_PRE:
			PreView = !PreView;
			break;
		}
		InvalidateRect(hWnd,NULL,TRUE);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		wsprintf(Mes,"%s%s - %s",NowView,PreView ? " - 그림 미리 보기 상태":"",
			Top ? "항상 위":"항상 위 아님");
		TextOut(hdc,10,10,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
