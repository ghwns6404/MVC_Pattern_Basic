#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("SavePosition");

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
	WndClass.lpszMenuName=NULL;
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	hWndMain=hWnd;
	
	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

#include "ShReg.h"
#define KEY "Software\\MiyoungSoft\\SavePosition\\"
void SavePosition(HWND hWnd, TCHAR *Key);
void LoadPosition(HWND hWnd, TCHAR *Key, RECT *Def=NULL);

void SavePosition(HWND hWnd, TCHAR *Key)
{
	WINDOWPLACEMENT wndpl;

	wndpl.length=sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hWnd,&wndpl);

	SHRegWriteInt(SHCU,Key,"showCmd", wndpl.showCmd);
	SHRegWriteInt(SHCU,Key,"left", wndpl.rcNormalPosition.left);
	SHRegWriteInt(SHCU,Key,"top", wndpl.rcNormalPosition.top);
	SHRegWriteInt(SHCU,Key,"right", wndpl.rcNormalPosition.right);
	SHRegWriteInt(SHCU,Key,"bottom",wndpl.rcNormalPosition.bottom);
}

void LoadPosition(HWND hWnd, TCHAR *Key, RECT *Def/*=NULL*/)
{
	WINDOWPLACEMENT wndpl;
	RECT drt;

	if (Def==NULL) {
		SetRect(&drt,10,10,600,400);
	} else {
		CopyRect(&drt,Def);
	}
	wndpl.length=sizeof(WINDOWPLACEMENT);
	wndpl.flags=0;
	wndpl.rcNormalPosition.left=SHRegReadInt(SHCU,Key,"left",drt.left);
	wndpl.rcNormalPosition.top=SHRegReadInt(SHCU,Key,"top",drt.top);
	wndpl.rcNormalPosition.right=SHRegReadInt(SHCU,Key,"right",drt.right);
	wndpl.rcNormalPosition.bottom=SHRegReadInt(SHCU,Key,"bottom",drt.bottom);
	wndpl.showCmd=SHRegReadInt(SHCU,Key,"showCmd",drt.bottom);

	if (wndpl.showCmd == SW_SHOWMINIMIZED) {
		wndpl.showCmd=SW_RESTORE;
	}

	wndpl.ptMinPosition.x=wndpl.ptMinPosition.y=0;
	wndpl.ptMaxPosition.x=wndpl.ptMaxPosition.y=0;
	SetWindowPlacement(hWnd,&wndpl);
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes="이 프로그램은 최후 실행 위치와 상태를 기억합니다";
	static bFirstActivate=TRUE;

	switch (iMessage) {
	// WinMain에서 생성 후 ShowWindow를 부르므로 최대화 상태 저장이 안됨
	case WM_CREATE:
//		LoadPosition(hWnd,KEY"Position");
		return 0;
/*	
	// 일회용 타이머를 쓰는 방법 - 노말 상태가 보인 후 최대화 되므로 안됨
	case WM_TIMER:
		switch (wParam) {
		case 0:
			KillTimer(hWnd,0);
			LoadPosition(hWnd,KEY"Position");
			break;
		}
		return 0;
	// WinMain의 CreateWindow 다음도 부적합 - 이미 윈도우가 보인 후이므로
*/
	// 처음 활성화될 때 딱 한번만.
	case WM_ACTIVATEAPP:
		if (wParam == TRUE) {
			if (bFirstActivate) {
				bFirstActivate=FALSE;
				LoadPosition(hWnd,KEY"Position");
			}
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,10,10,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		SavePosition(hWnd,KEY"Position");
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}


#if 0
// 아래 두 함수는 C++의 CRegi 클래스를 사용하는 버전임
void SavePosition(HWND hWnd, CRegi &R, TCHAR *Key)
{
	WINDOWPLACEMENT wndpl;

	wndpl.length=sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hWnd,&wndpl);

	R.WriteInt(Key,"showCmd", wndpl.showCmd);
	R.WriteInt(Key,"left", wndpl.rcNormalPosition.left);
	R.WriteInt(Key,"top", wndpl.rcNormalPosition.top);
	R.WriteInt(Key,"right", wndpl.rcNormalPosition.right);
	R.WriteInt(Key,"bottom",wndpl.rcNormalPosition.bottom);
}

void LoadPosition(HWND hWnd, CRegi &R, TCHAR *Key, RECT *Def/*=NULL*/)
{
	WINDOWPLACEMENT wndpl;
	RECT drt;

	if (Def==NULL) {
		SetRect(&drt,10,10,600,400);
	} else {
		CopyRect(&drt,Def);
	}
	wndpl.length=sizeof(WINDOWPLACEMENT);
	wndpl.flags=0;
	wndpl.rcNormalPosition.left=R.ReadInt(Key,"left",drt.left);
	wndpl.rcNormalPosition.top=R.ReadInt(Key,"top",drt.top);
	wndpl.rcNormalPosition.right=R.ReadInt(Key,"right",drt.right);
	wndpl.rcNormalPosition.bottom=R.ReadInt(Key,"bottom",drt.bottom);
	wndpl.showCmd=R.ReadInt(Key,"showCmd",drt.bottom);

	if (wndpl.showCmd == SW_SHOWMINIMIZED) {
		wndpl.showCmd=SW_RESTORE;
	}

	wndpl.ptMinPosition.x=wndpl.ptMinPosition.y=0;
	wndpl.ptMaxPosition.x=wndpl.ptMaxPosition.y=0;
	SetWindowPlacement(hWnd,&wndpl);
}
#endif