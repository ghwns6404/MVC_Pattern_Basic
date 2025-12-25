#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("Fighter2");

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

	hWnd=CreateWindow(lpszClass,lpszClass,WS_BORDER | WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT,CW_USEDEFAULT,640,480,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);

	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

// 물체의 개수및 속도
#define MAXENEMY 20
#define MAXBALL 20
#define MAXBULLET 5
#define BALLSPEED 3
#define BULSPEED 7
#define BULGAP 40
#define FIGHTERSPEED 10
struct tag_Enemy
{
	BOOL exist;
	int Type;
	int x,y;
	int Delta;
	int Speed;
} Enemy[MAXENEMY];
struct tag_Ball
{
	BOOL exist;
	int x,y;
} Ball[MAXBALL];
struct tag_Bullet
{
	BOOL exist;
	int x,y;
} Bullet[MAXBULLET];

int fx;
const int fy=420;
HBITMAP hFighter,hEnemy[3],hFBullet,hEBullet;
HBITMAP hBit;
// 파이터와 적, 총알의 크기. 적은 0번이 좀 크고 1,2번은 작다.
const int fw=30,fh=32;
#define ew(t) (t==0 ? 30:26)
#define eh(t) (t==0 ? 32:20)
const int bw=6,bh=12;
#define random(n) (rand()%n)

void DrawBitmap(HDC hdc,int x,int y,HBITMAP hBit);
void OnTimer(HWND hWnd);

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int i;

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		srand(GetTickCount());
		hFighter=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP1));
		hEnemy[0]=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP2));
		hEnemy[1]=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP3));
		hEnemy[2]=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP4));
		hFBullet=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP5));
		hEBullet=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BITMAP6));
		SendMessage(hWnd,WM_KEYDOWN,VK_RETURN,0);
		return 0;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_RETURN:
			fx=320;
			for (i=0;i<MAXBULLET;i++) {
				Bullet[i].exist=FALSE;
			}
			for (i=0;i<MAXBALL;i++) {
				Ball[i].exist=FALSE;
			}
			for (i=0;i<MAXENEMY;i++) {
				Enemy[i].exist=FALSE;
			}
			SetTimer(hWnd,1,20,NULL);
			break;
		}
		return 0;
	case WM_TIMER:
		OnTimer(hWnd);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		DrawBitmap(hdc,0,0,hBit);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		DeleteObject(hFighter);
		DeleteObject(hEnemy[0]);
		DeleteObject(hEnemy[1]);
		DeleteObject(hEnemy[2]);
		DeleteObject(hFBullet);
		DeleteObject(hEBullet);
		KillTimer(hWnd,1);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

void OnTimer(HWND hWnd)
{
	HDC hdc;
	RECT crt;
	HDC hMemDC;
	HBITMAP OldBit;
	int i,j,maxy;
	tag_Enemy e;
	tag_Bullet b;
	int w,h;
	RECT enemyrt,bulrt,ballrt,frt,irt;

	hdc=GetDC(hWnd);
	GetClientRect(hWnd,&crt);
	if (hBit==NULL) {
		hBit=CreateCompatibleBitmap(hdc,crt.right,crt.bottom);
	}
	hMemDC=CreateCompatibleDC(hdc);
	OldBit=(HBITMAP)SelectObject(hMemDC,hBit);
	FillRect(hMemDC,&crt,(HBRUSH)GetStockObject(BLACK_BRUSH));

	// 키입력 처리
	if (GetKeyState(VK_LEFT) < 0) {
		fx-=FIGHTERSPEED;
		fx=max(fw/2,fx);
	}
	if (GetKeyState(VK_RIGHT) < 0) {
		fx+=FIGHTERSPEED;
		fx=min(crt.right-fw/2,fx);
	}
	if (GetKeyState(VK_SPACE) < 0) {
		for (i=0,maxy=-1;i<MAXBULLET;i++) {
			if (Bullet[i].exist == TRUE) maxy=max(Bullet[i].y,maxy);
		}
		for (i=0;i<MAXBULLET;i++) {
			if (Bullet[i].exist == FALSE) break;
		}
		if (i != MAXBULLET && fy-maxy > BULGAP) {
			Bullet[i].exist=TRUE;
			Bullet[i].x=fx;
			Bullet[i].y=fy-bh;
		}
	}

	// 총알 이동 및 출력
	for (i=0;i<MAXBULLET;i++) {
		if (Bullet[i].exist == FALSE) continue;
		if (Bullet[i].y > 0) {
			Bullet[i].y-=BULSPEED;
			DrawBitmap(hMemDC,Bullet[i].x-bw/2,Bullet[i].y,hFBullet);
		} else {
			Bullet[i].exist=FALSE;
		}
	}

	// 파이터 출력
	DrawBitmap(hMemDC,fx-fw/2,fy,hFighter);

	// 적군 생성
	if (random(20) == 0) {
		for (i=0;i<MAXENEMY && Enemy[i].exist==TRUE;i++) {;}
		if (i != MAXENEMY) {
			Enemy[i].Type=random(3);
			if (random(2)==1) {
				Enemy[i].x=ew(0)/2;
				Enemy[i].Delta=1;
			} else {
				Enemy[i].x=crt.right-ew(0)/2;
				Enemy[i].Delta=-1;
			}
			Enemy[i].y=random(200)+50;
			Enemy[i].Speed=random(4)+3;
			Enemy[i].exist=TRUE;
		}
	}

	// 적군 이동 및 그림. 적군 총알 발사
	for (i=0;i<MAXENEMY;i++) {
		if (Enemy[i].exist==FALSE) continue;
		Enemy[i].x += Enemy[i].Speed*Enemy[i].Delta;
		if (Enemy[i].x < 0 || Enemy[i].x > crt.right) {
			Enemy[i].exist=FALSE;
		} else {
			DrawBitmap(hMemDC,Enemy[i].x-ew(Enemy[i].Type)/2,Enemy[i].y,
				hEnemy[Enemy[i].Type]);
		}
		if (random(40)==0) {
			for (j=0;j<MAXBALL && Ball[j].exist==TRUE;j++) {;}
			if (j!=MAXBALL) {
				Ball[j].x=Enemy[i].x+ew(Enemy[i].Type)/2;
				Ball[j].y=Enemy[i].y;
				Ball[j].exist=TRUE;
			}
		}
	}

	// 적군 총알 이동 및 그림
	for (i=0;i<MAXBALL;i++) {
		if (Ball[i].exist==FALSE) continue;
		if (Ball[i].y < fy) {
			Ball[i].y+=BALLSPEED;
			DrawBitmap(hMemDC,Ball[i].x-3,Ball[i].y,hEBullet);
		} else {
			Ball[i].exist=FALSE;
		}
	}


	// 적군과 총알의 충돌 판정
	for (i=0;i<MAXENEMY;i++) {
		if (Enemy[i].exist == FALSE) continue;
		e=Enemy[i];
		w=ew(Enemy[i].Type);
		h=eh(Enemy[i].Type);
		SetRect(&enemyrt,e.x-w/2,e.y,e.x+w/2,e.y+h);
		for (j=0;j<MAXBULLET;j++) {
			if (Bullet[j].exist == FALSE) continue;
			b=Bullet[j];
			SetRect(&bulrt,b.x-bw/2,b.y,b.x+bw/2,b.y+bh);
			IntersectRect(&irt,&enemyrt,&bulrt);
			if (IsRectEmpty(&irt) == FALSE) {
				Enemy[i].exist=FALSE;
				Bullet[j].exist=FALSE;
			}
		}
	}

	// 적군 총알과 아군의 충돌 판정
	SetRect(&frt,fx-fw/2,fy,fx+fw/2,fy+fh);
	for (i=0;i<MAXBALL;i++) {
		if (Ball[i].exist == FALSE) continue;
		SetRect(&ballrt,Ball[i].x-bw/2,Ball[i].y,Ball[i].x+bw/2,Ball[i].y+bh);
		IntersectRect(&irt,&frt,&ballrt);
		if (IsRectEmpty(&irt) == FALSE) {
			KillTimer(hWnd,1);
		}
	}

	SelectObject(hMemDC,OldBit);
	DeleteDC(hMemDC);
	ReleaseDC(hWnd,hdc);
	InvalidateRect(hWnd,NULL,FALSE);
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