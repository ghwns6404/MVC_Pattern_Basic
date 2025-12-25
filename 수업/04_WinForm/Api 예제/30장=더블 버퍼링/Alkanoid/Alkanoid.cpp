#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <mmsystem.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("Alkanoid");

#define TS 24
#define BW 10
#define BH 10
#define RY 450
#define LEFT TS
#define TOP TS
#define RIGHT (BW*2+1)*TS
enum { SOUND_RACKET, SOUND_BRICK, SOUND_BOARD, SOUND_DIE, SOUND_CLEAR };
enum tag_Status { GAMEWAIT, GAMERUN, GAMEPAUSE };

// 게임판 배열은 10*10 = 480*240픽셀. +1은 널종료 문자분
char arStage[][BH][BW+1]={
	{
	"0000000000",
	"0000000000",
	"0000000000",
	"1111111111",
	"1111111110",
	"1111111100",
	"1111111000",
	"1111110000",
	"1111100000",
	"1110000000",
	},
	{
	"0000000000",
	"1111001111",
	"1111001111",
	"1111001111",
	"1111001111",
	"0000000000",
	"0002222000",
	"0002222000",
	"0000000000",
	"0000000000",
	},
	{
	"0000000000",
	"0000000000",
	"0000000000",
	"1111111111",
	"1122222211",
	"1123333211",
	"1122222211",
	"1111111111",
	"0000000000",
	"0000000000",
	},
	{
	"0000000000",
	"0000000000",
	"3333333333",
	"3333003333",
	"2220000222",
	"1110000111",
	"2220000222",
	"3333003333",
	"3333333333",
	"1111111111",
	},
	{
	"1111111111",
	"2222222222",
	"3333333333",
	"3333333333",
	"3333333333",
	"3333333333",
	"3333333333",
	"3333333333",
	"2222222222",
	"1111111111",
	},
	{
	"0000000000",
	"0000000000",
	"2222222222",
	"3333333333",
	"3300000033",
	"3300000033",
	"0033003300",
	"0000000000",
	"0000000000",
	"3333333330",
	},
};
int nStage;
HBITMAP hBall,hRacket[3],hBrick[3],hBackGround;
struct tag_Cell {
	char brick;
	char item;					// 셀에 숨은 아이템. 현재 미구현
};
tag_Cell ns[BH][BW];
int rx,rw;
double bx,by;
const int bw=11;
int dx,dy;
double distance, angle;
HBITMAP hBackBit;
BOOL bMute=FALSE;
tag_Status Status=GAMEWAIT;
int nRacket;

LRESULT OnCreate(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnDestroy(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnPaint(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnLButtonDown(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnMouseMove(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnTimer(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnKeyDown(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnActivateApp(HWND hWnd,WPARAM wParam,LPARAM lParam);

void DrawBitmap(HDC hdc,int x,int y,HBITMAP hBit);
void InitStage();
void InitBall();
void TransBlt(HDC hdc, int x, int y, HBITMAP hbitmap, COLORREF clrMask);
BOOL CheckStrike(double x,double y,int mx,int my);
void Sound(int idx);
void ExclusiveCursor(BOOL bExclusive);
BOOL TestGameEnd();

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;
	
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=NULL;
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=NULL;
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		0,0,0,0,NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	hWndMain=hWnd;
	
	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage) {
	case WM_CREATE:
		return OnCreate(hWnd,wParam,lParam);
	case WM_DESTROY:
		return OnDestroy(hWnd,wParam,lParam);
	case WM_PAINT:
		return OnPaint(hWnd,wParam,lParam);
	case WM_TIMER:
		return OnTimer(hWnd,wParam,lParam);
	case WM_LBUTTONDOWN:
		return OnLButtonDown(hWnd,wParam,lParam);
	case WM_MOUSEMOVE:
		return OnMouseMove(hWnd,wParam,lParam);
	case WM_KEYDOWN:
		return OnKeyDown(hWnd,wParam,lParam);
	case WM_ACTIVATEAPP:
		return OnActivateApp(hWnd,wParam,lParam);
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

LRESULT OnCreate(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	RECT rt={0,0,640,480};
	AdjustWindowRect(&rt,WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,FALSE);
	SetWindowPos(hWnd,NULL,0,0,rt.right-rt.left,rt.bottom-rt.top,SWP_NOMOVE | SWP_NOZORDER);

	hBall=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BALL));
	hRacket[0]=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_RACKETSMALL));
	hRacket[1]=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_RACKETMID));
	hRacket[2]=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_RACKETBIG));
	hBrick[0]=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BRICK1));
	hBrick[1]=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BRICK2));
	hBrick[2]=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BRICK3));
	hBackGround=LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_BACKGROUND));
	nStage=0;
	nRacket=4;
	InitStage();
	SetTimer(hWnd,0,10,NULL);
	return 0;
}

LRESULT OnDestroy(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	PostQuitMessage(0);
	DeleteObject(hBall);
	DeleteObject(hRacket[0]);
	DeleteObject(hRacket[1]);
	DeleteObject(hRacket[2]);
	DeleteObject(hBrick[0]);
	DeleteObject(hBrick[1]);
	DeleteObject(hBrick[2]);
	DeleteObject(hBackGround);
	DeleteObject(hBackBit);
	KillTimer(hWnd,0);
	return 0;
}

LRESULT OnPaint(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT crt;
	HDC hMemDC;
	HBITMAP hOldBitmap;
	int x,y;

	hdc=BeginPaint(hWnd, &ps);
	hMemDC=CreateCompatibleDC(hdc);
	GetClientRect(hWnd,&crt);
	if (hBackBit == NULL) {
		hBackBit=CreateCompatibleBitmap(hdc,crt.right,crt.bottom);
	}
	hOldBitmap=(HBITMAP)SelectObject(hMemDC,hBackBit);
	DrawBitmap(hMemDC,0,0,hBackGround);

	for (y=0;y<BH;y++) {
		for (x=0;x<BW;x++) {
			switch (ns[y][x].brick) {
			case '0':
				break;
			case '1':
				DrawBitmap(hMemDC,x*TS*2+TS,y*TS+TS,hBrick[0]);
				break;
			case '2':
				DrawBitmap(hMemDC,x*TS*2+TS,y*TS+TS,hBrick[1]);
				break;
			case '3':
				DrawBitmap(hMemDC,x*TS*2+TS,y*TS+TS,hBrick[2]);
				break;
			}
		}
	}
	switch (rw) {
	case 32:
		TransBlt(hMemDC,rx,RY,hRacket[0],RGB(255,0,255));
		break;
	case 64:
		TransBlt(hMemDC,rx,RY,hRacket[1],RGB(255,0,255));
		break;
	case 100:
		TransBlt(hMemDC,rx,RY,hRacket[2],RGB(255,0,255));
		break;
	}
	TransBlt(hMemDC,(int)bx,(int)by,hBall,RGB(255,0,255));

	for (y=320,x=0;x<nRacket;x++,y+=20) {
		TransBlt(hMemDC,540,y,hRacket[1],RGB(255,0,255));
	}

	BitBlt(hdc,0,0,crt.right,crt.bottom,hMemDC,0,0,SRCCOPY);
	SelectObject(hMemDC,hOldBitmap);
	DeleteDC(hMemDC);
	EndPaint(hWnd, &ps);
	return 0;
}

LRESULT OnTimer(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	double mx,my;
	int bc,offset;
	double dist16;
	int times;
	int oldbx=-1,oldby=-1;

	if (Status != GAMERUN) return 0;

	dist16=distance/16;
	for (times=0;times<16;times++) {
		// 충돌 판정을 하기 전에 좌표부터 갱신한다.
		mx=cos(angle*3.14/180)*dist16*dx;
		my=sin(angle*3.14/180)*dist16*dy;
		bx+=mx;
		by+=my;

		if ((int)bx == oldbx && (int)by == oldby) {
			continue;
		}
		oldbx=(int)bx;
		oldby=(int)by;
		
		// 왼쪽 벽
		if (dx < 0) {
			if (bx <= LEFT) {
				dx*=-1;
				Sound(SOUND_BOARD);
			}
		// 오른쪽 벽
		} else {
			if (bx+bw >= RIGHT) {
				dx*=-1;
				Sound(SOUND_BOARD);
			}
		}

		// 아래쪽. 라켓 y에 6만큼 여유를 준다.
		if (dy > 0) {
			bc=int(bx+bw/2);
			if (bc >= rx && bc <= rx+rw && by+bw >= RY && by+bw <= RY+6) {
				dy*=-1;
				// 라켓 중심에서의 거리
				offset=int(rw/2-(bc-rx));
				// 중심 거리의 절대값으로 각도 계산(10~80)
				angle=-140*abs(offset)/rw+80;
				// 라켓 중심에서 맞은 방향으로 이동하도록
				if (offset < 0) {
					dx=1;
				} else {
					dx=-1;
				}
				Sound(SOUND_RACKET);
			}
		// 위쪽
		} else {
			if (by <= TOP) {
				dy*=-1;
				Sound(SOUND_BOARD);
			}
		}

		// 이동 방향에 따라 꼭 필요한 곳만 체크한다. 그렇지 않으면 공이 벽돌에 묻혔을 때 엉뚱하게 이동한다.
		// 한쪽이 맞으면 나머지는 체크하지 않는다. 
		BOOL bStrike=FALSE;
		if (dy == -1) bStrike=CheckStrike(bx+bw/2,by,1,-1);
		if (dx == 1 && bStrike==FALSE) bStrike=CheckStrike(bx+bw,by+bw/2,-1,1);
		if (dx == -1 && bStrike==FALSE) bStrike=CheckStrike(bx,by+bw/2,-1,1);
		if (dy == 1 && bStrike==FALSE) bStrike=CheckStrike(bx+bw/2,by+bw,1,-1);
	}

	// 라켓 아래쪽
	if (by > RY+16) {
		Sound(SOUND_DIE);
		Status=GAMEWAIT;
		ExclusiveCursor(FALSE);
		if (nRacket > 0) {
			nRacket--;
			InitBall();
		} else {
			nStage=0;
			nRacket=4;
			InitStage();
		}
	}

	if (TestGameEnd()) {
		Sound(SOUND_CLEAR);
		nStage++;
		if (nStage == sizeof(arStage)/sizeof(arStage[0]))
			nStage=0;
		InitStage();
		InvalidateRect(hWnd,NULL,TRUE);
		Status=GAMEWAIT;
		ExclusiveCursor(FALSE);
		return 0;
	}

	InvalidateRect(hWnd,NULL,TRUE);
	return 0;
}

LRESULT OnLButtonDown(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	switch (Status) {
	case GAMEWAIT:
		Status=GAMERUN;
		ExclusiveCursor(TRUE);
		break;
	case GAMERUN:
		Status=GAMEPAUSE;
		ExclusiveCursor(FALSE);
		break;
	case GAMEPAUSE:
		Status=GAMERUN;
		ExclusiveCursor(TRUE);
		break;
	}
	return 0;
}

LRESULT OnMouseMove(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	if (Status != GAMEPAUSE) {
		rx=LOWORD(lParam);
		rx=max(rx,TS);
		rx=min(rx,RIGHT-rw);

		// 대기중일 때만 화면 무효화. 그 외의 경우는 타이머에서 매번 그리므로 무효화할 필요없다.
		if (Status == GAMEWAIT) {
			bx=rx+rw*2/3;
			InvalidateRect(hWnd,NULL,TRUE);
		}
	}
	return 0;
}

LRESULT OnKeyDown(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	switch (wParam) {
	// 공 이동의 정밀 체크를 위한 타이머 제어 코드. 
	//distance가 클 때의 동작을 관찰하기 위해 필요하다.
#ifdef _DEBUG
	case 'Z':
		SetTimer(hWnd,0,50,NULL);
		break;
#endif
	case 'S':
		bMute=!bMute;
		break;
	case VK_DOWN:
		if (distance > 1.0) distance -= 0.5;
		break;
	case VK_UP:
		if (distance < 10.0) distance += 0.5;
		break;
	case VK_LEFT:
		if (Status == GAMEWAIT) {
			if (nStage > 0) {
				nStage--;
				InitStage();
				InvalidateRect(hWnd,NULL,TRUE);
				ExclusiveCursor(FALSE);
			}
		}
		break;
	case VK_RIGHT:
		if (Status == GAMEWAIT) {
			if (nStage < sizeof(arStage)/sizeof(arStage[0])-1) {
				nStage++;
				InitStage();
				InvalidateRect(hWnd,NULL,TRUE);
				ExclusiveCursor(FALSE);
			}
		}
		break;
	}
	return 0;
}

LRESULT OnActivateApp(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	if (Status == GAMERUN && wParam==FALSE) {
		Status=GAMEPAUSE;
		ExclusiveCursor(FALSE);
	}
	return TRUE;
}

// 일반 함수들

void InitStage()
{
	int x,y;

	for (y=0;y<BH;y++) {
		for (x=0;x<BW;x++) {
			ns[y][x].brick=arStage[nStage][y][x];
		}
	}
	rx=(BW*TS*2+TS+TS)/2;
	rw=100;
	distance=3.0;
	InitBall();
}

void InitBall()
{
	bx=rx+rw*2/3;
	by=RY-bw;
	dx=1;
	dy=-1;
	angle=50;
}

void Sound(int idx)
{
	if (bMute) return;
	switch (idx) {
	case SOUND_RACKET:
		PlaySound(MAKEINTRESOURCE(IDR_WAVE1), g_hInst, SND_RESOURCE | SND_ASYNC);
		break;
	case SOUND_BRICK:
		PlaySound(MAKEINTRESOURCE(IDR_WAVE2), g_hInst, SND_RESOURCE | SND_ASYNC);
		break;
	case SOUND_BOARD:
		PlaySound(MAKEINTRESOURCE(IDR_WAVE3), g_hInst, SND_RESOURCE | SND_ASYNC);
		break;
	case SOUND_DIE:
		PlaySound(MAKEINTRESOURCE(IDR_WAVE4), g_hInst, SND_RESOURCE | SND_ASYNC);
		break;
	case SOUND_CLEAR:
		PlaySound(MAKEINTRESOURCE(IDR_WAVE5), g_hInst, SND_RESOURCE);
		break;
	}
}

BOOL CheckStrike(double x,double y,int mx,int my)
{
	int bpx,bpy;

	bpx=(int)(x-TS)/(TS*2);
	bpy=(int)(y-TS)/TS;
	// 게임판 안쪽이고 빈 공간이 아닐 경우
	if (bpx < 10 && bpy < 10 && ns[bpy][bpx].brick != '0') {
		Sound(SOUND_BRICK);
		ns[bpy][bpx].brick--;
		if (distance < 20) distance+=0.1;
		dx*=mx;
		dy*=my;
		return TRUE;
	}
	return FALSE;
}

void ExclusiveCursor(BOOL bExclusive)
{
	RECT crt;
	int count;

	if (bExclusive) {
		GetClientRect(hWndMain,&crt);
		ClientToScreen(hWndMain,(LPPOINT)&crt);
		ClientToScreen(hWndMain,(LPPOINT)&crt.right);
		ClipCursor(&crt);
		count=ShowCursor(FALSE);
		if (count==-2) {
			ShowCursor(TRUE);
		}
	} else {
		ClipCursor(NULL);
		count=ShowCursor(TRUE);
		if (count==1) {
			ShowCursor(FALSE);
		}
	}
}

BOOL TestGameEnd()
{
	int x,y;

	for (y=0;y<BH;y++) {
		for (x=0;x<BW;x++) {
			if (ns[y][x].brick != '0') 
				return FALSE;
		}
	}
	return TRUE;
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

void TransBlt(HDC hdc, int x, int y, HBITMAP hbitmap, COLORREF clrMask)
{
	BITMAP bm;
	COLORREF cColor;
	HBITMAP bmAndBack, bmAndObject, bmAndMem, bmSave;
	HBITMAP bmBackOld, bmObjectOld, bmMemOld, bmSaveOld;
	HDC		hdcMem, hdcBack, hdcObject, hdcTemp, hdcSave;
	POINT	ptSize;

	hdcTemp = CreateCompatibleDC(hdc);
	SelectObject(hdcTemp, hbitmap);
	GetObject(hbitmap, sizeof(BITMAP), (LPSTR)&bm);
	ptSize.x = bm.bmWidth;
	ptSize.y = bm.bmHeight;
	DPtoLP(hdcTemp, &ptSize,1);

	hdcBack   = CreateCompatibleDC(hdc);
	hdcObject = CreateCompatibleDC(hdc);
	hdcMem    = CreateCompatibleDC(hdc);
	hdcSave   = CreateCompatibleDC(hdc);

	bmAndBack   = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
	bmAndObject = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
	bmAndMem    = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
	bmSave      = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);

	bmBackOld   = (HBITMAP) SelectObject(hdcBack, bmAndBack);
	bmObjectOld = (HBITMAP) SelectObject(hdcObject, bmAndObject);
	bmMemOld    = (HBITMAP) SelectObject(hdcMem, bmAndMem);
	bmSaveOld   = (HBITMAP) SelectObject(hdcSave, bmSave);

	SetMapMode(hdcTemp, GetMapMode(hdc));

	BitBlt(hdcSave, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);

	cColor = SetBkColor(hdcTemp, clrMask);

	BitBlt(hdcObject, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);

	SetBkColor(hdcTemp, cColor);

	BitBlt(hdcBack, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, NOTSRCCOPY);
	BitBlt(hdcMem , 0, 0, ptSize.x, ptSize.y, hdc      , x, y, SRCCOPY);
	BitBlt(hdcMem , 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, SRCAND);
	BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcBack  , 0, 0, SRCAND);
	BitBlt(hdcMem , 0, 0, ptSize.x, ptSize.y, hdcTemp  , 0, 0, SRCPAINT);
	BitBlt(hdc    , x, y, ptSize.x, ptSize.y, hdcMem   , 0, 0, SRCCOPY);
	BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcSave  , 0, 0, SRCCOPY);

	DeleteObject(SelectObject(hdcBack, bmBackOld));
	DeleteObject(SelectObject(hdcObject, bmObjectOld));
	DeleteObject(SelectObject(hdcMem, bmMemOld));
	DeleteObject(SelectObject(hdcSave, bmSaveOld));
	
	DeleteDC(hdcMem);
	DeleteDC(hdcBack);
	DeleteDC(hdcObject);
	DeleteDC(hdcSave);
	DeleteDC(hdcTemp);
}

