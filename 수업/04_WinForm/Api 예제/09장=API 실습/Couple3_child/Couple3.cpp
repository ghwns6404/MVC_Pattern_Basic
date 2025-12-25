#include <windows.h>
#include "resource.h"
#define random(n) (rand()%n)

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK ChildProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("Couple3_Child");

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
	WndClass.hIcon=LoadIcon(g_hInst,MAKEINTRESOURCE(IDI_COUPLE));
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=NULL;
	WndClass.style=0;
	RegisterClass(&WndClass);

	// 차일드 윈도우 클래스. 여분 8바이트, 0=숫자, 4=상태
	WndClass.cbWndExtra=8;
	WndClass.hbrBackground=NULL;
	WndClass.lpfnWndProc=ChildProc;
	WndClass.lpszClassName="CoupleChild";
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | 
		WS_BORDER | WS_CLIPCHILDREN,
		CW_USEDEFAULT,CW_USEDEFAULT,0,0,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	
	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

enum Status { HIDDEN, FLIP, TEMPFLIP };
int count;
HBITMAP hShape[9];
enum { RUN, HINT, VIEW } GameStatus;
HWND hChild[16];
#define EB_NUM 0
#define EB_STATUS 4

void InitGame();
HWND GetTempFlip();
int GetRemain();
void DrawBitmap(HDC hdc,int x,int y,HBITMAP hBit);

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int i;
	RECT crt;
	TCHAR Mes[128];

	switch (iMessage) {
	case WM_CREATE:
		SetRect(&crt,0,0,64*4+250,64*4);
		AdjustWindowRect(&crt,WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | 
			WS_MINIMIZEBOX | WS_BORDER | WS_CLIPCHILDREN,FALSE);
		SetWindowPos(hWnd,NULL,0,0,crt.right-crt.left,crt.bottom-crt.top,
			SWP_NOMOVE | SWP_NOZORDER);
		hWndMain=hWnd;
		for (i=0;i<sizeof(hShape)/sizeof(hShape[0]);i++) {
			hShape[i]=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_SHAPE1+i));
		}
		// 차일드 생성
		for (i=0;i<16;i++) {
			hChild[i]=CreateWindow("CoupleChild",NULL,WS_CHILD | WS_VISIBLE,
				(i/4)*64,(i%4)*64,64,64,hWnd,(HMENU)i,g_hInst,NULL);
		}
		srand(GetTickCount());
		InitGame();
		return 0;
	case WM_TIMER:
		switch (wParam) {
		case 0:
			KillTimer(hWnd,0);
			GameStatus=RUN;
			for (i=0;i<16;i++) {
				InvalidateRect(hChild[i],NULL,FALSE);
			}
			break;
		case 1:
			KillTimer(hWnd,1);
			GameStatus=RUN;
			for (i=0;i<16;i++) {
				if (GetWindowLong(hChild[i],EB_STATUS) == TEMPFLIP) {
					SetWindowLong(hChild[i],EB_STATUS,HIDDEN);
					InvalidateRect(hChild[i],NULL,FALSE);
				}
			}
			break;
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		lstrcpy(Mes,"짝 맞추기 게임 Ver 1.2");
		TextOut(hdc,300,10,Mes,lstrlen(Mes));
		wsprintf(Mes,"총 시도 회수 : %d",count);
		TextOut(hdc,300,50,Mes,lstrlen(Mes));
		wsprintf(Mes,"아직 못 찾은 것 : %d   ",GetRemain());
		TextOut(hdc,300,70,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		for (i=0;i<sizeof(hShape)/sizeof(hShape[0]);i++) {
			DeleteObject(hShape[i]);
		}
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

LRESULT CALLBACK ChildProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HWND hBrother;
	int image;

	switch (iMessage) {
	case WM_CREATE:
		SetWindowLong(hWnd,EB_NUM,0);
		SetWindowLong(hWnd,EB_STATUS,(LONG)HIDDEN);
		return 0;
	case WM_LBUTTONDOWN:
		if (GameStatus != RUN || GetWindowLong(hWnd,EB_STATUS) != HIDDEN) {
			return 0;
		}

		// 일단 클릭하면 조건에 상관없이 다시 그려야 함. 부모도 마찬가지
		InvalidateRect(hWnd,NULL,FALSE);
		InvalidateRect(hWndMain,NULL,FALSE);

		hBrother=GetTempFlip();
		// 열려진 형제가 없으면 자신만 연다.
		if (hBrother == NULL) {
			SetWindowLong(hWnd,EB_STATUS,TEMPFLIP);
		} else {
			count++;
			// 맞으면 둘 다 오픈
			if (GetWindowLong(hWnd,EB_NUM) == GetWindowLong(hBrother,EB_NUM)) {
				SetWindowLong(hWnd,EB_STATUS,FLIP);
				SetWindowLong(hBrother,EB_STATUS,FLIP);
				if (GetRemain() == 0) {
					MessageBox(hWndMain,"축하합니다. 다시 시작합니다.","알림",MB_OK);
					InitGame();
				}
			// 틀리면 둘 다 잠시 보여 주고 다시 숨김
			} else {
				SetWindowLong(hWnd,EB_STATUS,TEMPFLIP);
				GameStatus=VIEW;
				SetTimer(hWndMain,1,1000,NULL);
			}
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		if (GameStatus==HINT || GetWindowLong(hWnd,EB_STATUS)!=HIDDEN) {
			image=GetWindowLong(hWnd,EB_NUM)-1;
		} else {
			image=8;
		}
		DrawBitmap(hdc,0,0,hShape[image]);
		EndPaint(hWnd, &ps);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

void InitGame()
{
	int i,j;
	int c;

	count=0;
	// 차일드 초기화
	for (i=0;i<16;i++) {
		SetWindowLong(hChild[i],EB_STATUS,HIDDEN);
		SetWindowLong(hChild[i],EB_NUM,0);
	}

	// 차일드에 숫자를 난수로 배치한다.
	for (i=1;i<=8;i++) {
		for (j=0;j<2;j++) {
			do {
				c=random(16);
			} while (GetWindowLong(hChild[c],EB_NUM) != 0);
			SetWindowLong(hChild[c],EB_NUM,i);
		}
	}

	// 2초간 힌트를 보여준다.
	GameStatus=HINT;
	for (i=0;i<16;i++) {
		InvalidateRect(hChild[i],NULL,FALSE);
	}
	InvalidateRect(hWndMain,NULL,TRUE);
	SetTimer(hWndMain,0,2000,NULL);
}

HWND GetTempFlip()
{
	int i;
	for (i=0;i<16;i++) {
		if (GetWindowLong(hChild[i],EB_STATUS) == TEMPFLIP) {
			return hChild[i];
		}
	}
	return NULL;
}

int GetRemain()
{
	int i;
	int remain=16;

	for (i=0;i<16;i++) {
		if (GetWindowLong(hChild[i],EB_STATUS) == FLIP) {
			remain--;
		}
	}
	return remain;
}

void DrawBitmap(HDC hdc,int x,int y,HBITMAP hBit)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx,by;
	BITMAP bit;

	MemDC=CreateCompatibleDC(hdc);
	OldBitmap=(HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit,sizeof(BITMAP),&bit);
	bx=bit.bmWidth;
	by=bit.bmHeight;

	BitBlt(hdc,x,y,bx,by,MemDC,0,0,SRCCOPY);

	SelectObject(MemDC,OldBitmap);
	DeleteDC(MemDC);
}