#include <windows.h>
#include "resource.h"
#include "GrpBtn2.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("GrpBtn2Test");

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
	
	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

GrpBtn2 Btn1(10,10,16,16,GBS_PUSH,1,NULL);
GrpBtn2 Btn2(30,10,32,32,GBS_PUSH,2,NULL);
GrpBtn2 Btn3(80,10,24,24,GBS_CHECK,3,NULL);
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes="E:사용금지, S:보이기, M:이동, C:체크";

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		Btn1.ChangeParent(hWnd);
		Btn2.ChangeParent(hWnd);
		Btn3.ChangeParent(hWnd);
		Btn1.SetImage(IDB_BTN1NORMAL,IDB_BTN1HOT,IDB_BTN1DOWN,IDB_BTN1DISABLE);
		Btn2.SetImage(IDB_BTN2NORMAL,IDB_BTN2HOT,IDB_BTN2DOWN,IDB_BTN2DISABLE);
		Btn3.SetImage(IDB_BTN3NORMAL,IDB_BTN3NORMAL,IDB_BTN3DOWN,IDB_BTN3NORMAL);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case 1:
			MessageBeep(0);
			break;
		case 2:
			MessageBox(hWnd,"두 번째 버튼을 눌렀습니다.","알림",MB_OK);
			break;
		case 3:
			if (Btn3.GetState() == GB_DOWN) {
				SetWindowText(hWnd,"체크 되었음");
			} else {
				SetWindowText(hWnd,"체크 안되었음");
			}
			break;
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,10,100,Mes,lstrlen(Mes));
		Btn1.OnPaint(hdc);
		Btn2.OnPaint(hdc);
		Btn3.OnPaint(hdc);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		Btn1.OnDown(lParam);
		Btn2.OnDown(lParam);
		Btn3.OnDown(lParam);
		return 0;
	case WM_MOUSEMOVE:
		Btn1.OnMove(lParam);
		Btn2.OnMove(lParam);
		Btn3.OnMove(lParam);
		return 0;
	case WM_LBUTTONUP:
		Btn1.OnUp(lParam);
		Btn2.OnUp(lParam);
		Btn3.OnUp(lParam);
		return 0;
	case WM_TIMER:
		if (wParam==1234) {
			Btn1.OnTimer();
			Btn2.OnTimer();
			Btn3.OnTimer();
		}
		return 0;
	case WM_KEYDOWN:
		switch (wParam) {
		case 'E':
			Btn2.Enable(!Btn2.IsEnabled());
			break;
		case 'S':
			Btn2.Show(!Btn2.IsShow());
			break;
		case 'M':
			Btn2.Move(Btn2.GetX(),Btn2.GetY()+10);
			break;
		case 'C':
			Btn3.SetState(Btn3.GetState()==GB_DOWN ? GB_NORMAL:GB_DOWN);
			break;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
