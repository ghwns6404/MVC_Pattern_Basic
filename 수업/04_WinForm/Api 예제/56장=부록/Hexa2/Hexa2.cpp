#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("Hexa2");

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
	WndClass.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style=0;
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT,CW_USEDEFAULT,0,0,NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);

	HACCEL hAccel=LoadAccelerators(hInstance,MAKEINTRESOURCE(IDR_ACCELERATOR1));
	while (GetMessage(&Message,NULL,0,0)) {
		if (!TranslateAccelerator(hWnd,hAccel,&Message)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}
	return (int)Message.wParam;
}

// 함수 원형
void DrawScreen(HDC hdc);
void MakeNewBrick();
int GetAround(int x,int y);
BOOL MoveDown();
void TestFull();
void PrintTile(HDC hdc,int x,int y,int c);
void DrawBitmap(HDC hdc,int x,int y,HBITMAP hBit);

// 매크로 및 전역 변수들
#define BW 10
#define BH 20
#define random(n) (rand()%n)
#define TS 24
enum { EMPTY, B1, B2, B3, B4, B5, B6, B7, B8, B9, B10, WALL };
int board[BW+2][BH+2];
int nx,ny;
int brick[3];
int nbrick[3];
int score;
int bricknum;
int level;
enum tag_Status { GAMEOVER, RUNNING, PAUSE };
tag_Status GameStatus;
int Interval;
HBITMAP hBit[12];

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	int i;
	RECT crt;
	HDC hdc;
	PAINTSTRUCT ps;
	int x,y;
	int t;

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		SetRect(&crt,0,0,(BW+12)*TS,(BH+2)*TS);
		AdjustWindowRect(&crt,WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,TRUE);
		SetWindowPos(hWndMain,NULL,0,0,crt.right-crt.left,crt.bottom-crt.top,
			SWP_NOMOVE | SWP_NOZORDER);

		GameStatus=GAMEOVER;
		srand(GetTickCount());
		for (i=0;i<12;i++) {
			hBit[i]=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP1+i));
		}
		return 0;
	case WM_COMMAND:
	switch (LOWORD(wParam)) {
		case IDM_GAME_START:
			if (GameStatus != GAMEOVER) {
				break;
			}
			for (x=0;x<BW+2;x++) {
				for (y=0;y<BH+2;y++) {
					board[x][y] = (y==0 || y==BH+1 || x==0 || x==BW+1) ? WALL:EMPTY;
				}
			}
			level=6;
			score=0;
			bricknum=0;
			GameStatus=RUNNING;
			MakeNewBrick();
			MakeNewBrick();
			Interval=1000;
			SetTimer(hWnd,1,Interval,NULL);
			break;
		case IDM_GAME_PAUSE:
			if (GameStatus == RUNNING) {
				GameStatus=PAUSE;
				KillTimer(hWnd,1);
			} else if (GameStatus == PAUSE) {
				GameStatus=RUNNING;
				SetTimer(hWnd,1,Interval,NULL);
			}
			break;
		case IDM_GAME_EXIT:
			DestroyWindow(hWnd);
			break;
		}
		return 0;
	case WM_TIMER:
		switch (wParam) {
		case 1:
			if (MoveDown() == TRUE) {
				MakeNewBrick();
			}
			break;
		}
		return 0;
	case WM_KEYDOWN:
		if (GameStatus != RUNNING)
			return 0;
		switch (wParam) {
		case VK_LEFT:
			if (GetAround(nx-1,ny) == EMPTY) {
				nx--;
				InvalidateRect(hWnd,NULL,FALSE);
			}
			break;
		case VK_RIGHT:
			if (GetAround(nx+1,ny) == EMPTY) {
				nx++;
				InvalidateRect(hWnd,NULL,FALSE);
			}
			break;
		case VK_UP:
			t=brick[0];
			brick[0]=brick[1];
			brick[1]=brick[2];
			brick[2]=t;
			InvalidateRect(hWnd,NULL,FALSE);
			break;
		case VK_DOWN:
			if (MoveDown() == TRUE) {
				MakeNewBrick();
			}
			break;
		case VK_SPACE:
			while(MoveDown()==FALSE) {;}
			MakeNewBrick();
			break;
		case VK_NEXT:
			if (level > 2) {
				level--;
				InvalidateRect(hWnd,NULL,FALSE);
			}
			break;
		case VK_PRIOR:
			if (level < 10) {
				level++;
				InvalidateRect(hWnd,NULL,FALSE);
			}
			break;
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		DrawScreen(hdc);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		KillTimer(hWndMain,1);
		for (i=0;i<12;i++) {
			DeleteObject(hBit[i]);
		}
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

void DrawScreen(HDC hdc)
{
	int x,y,i;
	TCHAR str[128];

	// 테두리 그림
	for (x=0;x<BW+1;x++) {
		PrintTile(hdc,x,0,WALL);
		PrintTile(hdc,x,BH+1,WALL);
	}
	for (y=0;y<BH+2;y++) {
		PrintTile(hdc,0,y,WALL);
		PrintTile(hdc,BW+1,y,WALL);
	}

	// 게임판 그림
	for (x=1;x<BW+1;x++) {
		for (y=1;y<BH+1;y++) {
			PrintTile(hdc,x,y,board[x][y]);
		}
	}

	// 이동중인 벽돌 그림
	if (GameStatus != GAMEOVER && brick[0] != -1) {
		for (i=0;i<3;i++) {
			PrintTile(hdc,nx,ny+i,brick[i]);
		}
	}

	// 다음 벽돌 그림
	for (x=BW+3;x<=BW+11;x++) {
		for (y=BH-5;y<=BH+1;y++) {
			if (x==BW+3 || x==BW+11 || y==BH-5 || y==BH+1) {
				PrintTile(hdc,x,y,WALL);
			} else {
				PrintTile(hdc,x,y,0);
			}
		}
	}
	if (GameStatus != GAMEOVER) {
		for (i=0;i<3;i++) {
			PrintTile(hdc,BW+7,BH-3+i,nbrick[i]);
		}
	}

	// 정보 출력
	lstrcpy(str,"Hexa Ver 1.1");
	TextOut(hdc,(BW+3)*TS,30,str,lstrlen(str));
	lstrcpy(str,"좌우:이동, 위:회전, 아래:내림");
	TextOut(hdc,(BW+3)*TS,50,str,lstrlen(str));
	lstrcpy(str,"공백:전부 내림");
	TextOut(hdc,(BW+3)*TS,70,str,lstrlen(str));
	lstrcpy(str,"P:정지,PgUp,PgDn:난이도 조절");
	TextOut(hdc,(BW+3)*TS,90,str,lstrlen(str));

	wsprintf(str,"난이도 : %d   ",level);
	TextOut(hdc,(BW+3)*TS,200,str,lstrlen(str));
	wsprintf(str,"점수 : %d   ",score);
	TextOut(hdc,(BW+3)*TS,220,str,lstrlen(str));
	wsprintf(str,"벽돌 : %d 개   ",bricknum);
	TextOut(hdc,(BW+3)*TS,240,str,lstrlen(str));
}

void MakeNewBrick()
{
	int i;

	bricknum++;
	memcpy(brick,nbrick,sizeof(brick));
	do {
		for (i=0;i<3;i++) {
			nbrick[i]=random(level)+1;
		}
	} while (nbrick[0]==nbrick[1] && nbrick[1]==nbrick[2] && nbrick[0]==nbrick[2]);
	nx=BW/2;
	ny=1;
	InvalidateRect(hWndMain,NULL,FALSE);
	if (GetAround(nx,ny) != EMPTY) {
		KillTimer(hWndMain,1);
		GameStatus=GAMEOVER;
		MessageBox(hWndMain,"게임이 끝났습니다. 다시 시작하려면 게임/시작"
			" 항목(S)을 선택해 주십시오.","알림",MB_OK);
	}
}

int GetAround(int x,int y)
{
	int i,k=EMPTY;

	for (i=0;i<3;i++) {
		k=max(k,board[x][y+i]);
	}
	return k;
}

BOOL MoveDown()
{
	if (GetAround(nx,ny+1) != EMPTY) {
		TestFull();
		return TRUE;
	}
	ny++;
	InvalidateRect(hWndMain,NULL,FALSE);
	UpdateWindow(hWndMain);
	return FALSE;
}

void TestFull()
{
	int i,x,y;
	int t,ty;
	BOOL Remove;
	static int arScoreInc[]={ 0,1,3,7,15,30,100,500 };
	int count=0;
	BOOL Mark[BW+2][BH+2];
	HDC hdc;

	// 배열에 기록
	for (i=0;i<3;i++) {
		board[nx][ny+i]=brick[i];
	}
	brick[0]=-1;

	for (;;) {
		// 연속 무늬 점검
		memset(Mark,0,sizeof(Mark));
		Remove=FALSE;
		for (y=1;y<BH+1;y++) {
			for (x=1;x<BW+1;x++) {
				t=board[x][y];
				if (t==EMPTY) continue;
				
				// 수평
				if (board[x-1][y]==t && board[x+1][y]==t) {
					for (i=-1;i<=1;i++) Mark[x+i][y]=TRUE;
					Remove=TRUE;
				}
				// 수직
				if (board[x][y-1]==t && board[x][y+1]==t) {
					for (i=-1;i<=1;i++) Mark[x][y+i]=TRUE;
					Remove=TRUE;
				}
				// 우하향
				if (board[x-1][y-1]==t && board[x+1][y+1]==t) {
					for (i=-1;i<=1;i++) Mark[x+i][y+i]=TRUE;
					Remove=TRUE;
				}
				// 좌하향
				if (board[x+1][y-1]==t && board[x-1][y+1]==t) {
					for (i=-1;i<=1;i++) Mark[x-i][y+i]=TRUE;
					Remove=TRUE;
				}
			}
		}

		if (Remove == FALSE) return;

		// 제거 애니메이션
		hdc=GetDC(hWndMain);
		for (i=0;i<6;i++) {
			for (y=1;y<BH+1;y++) {
				for (x=1;x<BW+1;x++) {
					if (board[x][y]!=EMPTY && Mark[x][y]==TRUE) {
						PrintTile(hdc,x,y,i%2 ? EMPTY:board[x][y]);
					}
				}
			}
			Sleep(150);
		}
		ReleaseDC(hWndMain,hdc);

		// 연속된 무늬 삭제
		for (y=1;y<BH+1;y++) {
			for (x=1;x<BW+1;x++) {
				if (board[x][y]!=EMPTY && Mark[x][y]==TRUE) {
					for (ty=y;ty>1;ty--) {
						board[x][ty]=board[x][ty-1];
					}
					board[x][1]=EMPTY;
					count++;
				}
			}
		}

		score += arScoreInc[min(count/3,7)];
		InvalidateRect(hWndMain,NULL,FALSE);
		UpdateWindow(hWndMain);
	}
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

void PrintTile(HDC hdc,int x,int y,int c)
{
	DrawBitmap(hdc,x*TS,y*TS,hBit[c]);
	return;
}

