#include <windows.h>
#include <math.h>

#pragma warning (disable:4244)

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("PangPang");

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;

	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);;
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=NULL;
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,WS_BORDER | WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT,CW_USEDEFAULT,640,400,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);

	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

//#define STRESS

#define MAXBALL 1000			// 공의 최대 개수
#ifdef STRESS
#define MAXBULLET 25
#define BULGAP 15
#define BULSPEED 15
#else
#define MAXBULLET 3
#define BULGAP 40
#define BULSPEED 6
#endif
#define MANSPEED 5
const double A=0.15;			// 중력 가속도
const double G=0.01;			// 중력
const double AR=0.0;			// 공기의 저항
struct tag_Ball
{
	BOOL Exist;					// 공의 존재 유무
	double x,y;					// 현재 위치
	int r;						// 반지름
	double dx,dy;				// 델타값
	COLORREF col;				// 색상
};
tag_Ball arBall[MAXBALL];
struct tag_Bullet
{
	BOOL Exist;
	int x,y;
};
tag_Bullet arBullet[MAXBULLET];
int cx;
const int cy=360;

HBITMAP hBit;
enum { STOP, RUN } Status;
int nStage;
int nLife;
int nScore;

void DrawBitmap(HDC hdc,int x,int y,HBITMAP hBit);
void OnTimer(HWND hWnd);
void StartStage(BOOL bReset);
void NewBall(BOOL bLeft);
void MoveBall();
void MoveBullet();
void SplitBall(int idx);

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes="게임을 시작하려면 엔터키를 씩씩하게 눌러라";

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		srand(GetTickCount());
		Status=STOP;
		return 0;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_RETURN:
			if (Status == STOP) {
				Status=RUN;
#ifdef STRESS
				nStage=10;
#else
				nStage=1;
#endif
				nLife=5;
				nScore=0;
				StartStage(TRUE);
				SetTimer(hWnd,1,20,NULL);
			}
			break;
		}
		return 0;
	case WM_TIMER:
		OnTimer(hWnd);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		if (Status == RUN) {
			DrawBitmap(hdc,0,0,hBit);
		} else {
			TextOut(hdc,10,10,Mes,lstrlen(Mes));
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd,1);
		DeleteObject(hBit);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

void OnTimer(HWND hWnd)
{
	HDC hdc;
	int i,j,maxy;
	HBRUSH hBrush,hOldBrush;
	RECT crt;
	HDC hMemDC;
	HBITMAP OldBit;
	TCHAR Mes[128];
	RECT ballrt,bulrt,irt,mrt;
	HRGN ballrgn;
	tag_Ball b;

	hdc=GetDC(hWnd);
	GetClientRect(hWnd,&crt);
	if (hBit==NULL) {
		hBit=CreateCompatibleBitmap(hdc,crt.right,crt.bottom);
	}
	hMemDC=CreateCompatibleDC(hdc);
	OldBit=(HBITMAP)SelectObject(hMemDC,hBit);
	FillRect(hMemDC,&crt,GetSysColorBrush(COLOR_WINDOW));
	SelectObject(hMemDC,GetStockObject(NULL_PEN));

	// 공 및 총알 이동
	MoveBall();
	MoveBullet();

	// 키입력 처리
	if (GetKeyState(VK_LEFT) < 0) {
		if (cx > 10) cx-=MANSPEED;
	}
	if (GetKeyState(VK_RIGHT) < 0) {
		if (cx < 630) cx+=MANSPEED;
	}
	if (GetKeyState(VK_SPACE) < 0) {
		for (i=0,maxy=-1;i<MAXBULLET;i++) {
			if (arBullet[i].Exist == TRUE) maxy=max(arBullet[i].y,maxy);
		}
		for (i=0;i<MAXBULLET;i++) {
			if (arBullet[i].Exist == FALSE) break;
		}
		// 빈칸이 있고 최후 발사된 총알이 BULGAP보다 더 위에 있어야 한다.
		if (i != MAXBULLET && cy-maxy > BULGAP) {
			arBullet[i].Exist=TRUE;
			arBullet[i].x=cx;
			arBullet[i].y=cy-10;
		}
	}

	// 캐릭터 출력
	for (i=0;i<MAXBALL;i++) {
		if (arBall[i].Exist) {
			hBrush=CreateSolidBrush(arBall[i].col);
			hOldBrush=(HBRUSH)SelectObject(hMemDC,hBrush);
			Ellipse(hMemDC,int(arBall[i].x-arBall[i].r),int(arBall[i].y-arBall[i].r),
				int(arBall[i].x+arBall[i].r),int(arBall[i].y+arBall[i].r));
			DeleteObject(SelectObject(hMemDC,hOldBrush));
		}
	}

	SetTextAlign(hMemDC,TA_CENTER);
	SetBkMode(hMemDC,TRANSPARENT);
	TextOut(hMemDC,cx,cy,"A",1);
	for (i=0;i<MAXBULLET;i++) {
		if (arBullet[i].Exist == TRUE) {
			TextOut(hMemDC,arBullet[i].x,arBullet[i].y,"i",1);
		}
	}
	wsprintf(Mes,"스테이지=%d,생명=%d,점수=%d",nStage,nLife,nScore);
	TextOut(hMemDC,320,5,Mes,lstrlen(Mes));
	
	SelectObject(hMemDC,OldBit);
	DeleteDC(hMemDC);
	ReleaseDC(hWnd,hdc);
	InvalidateRect(hWnd,NULL,FALSE);

	// 공과 총알의 충돌 판정
	for (i=0;i<MAXBALL;i++) {
		if (arBall[i].Exist == FALSE) continue;
		b=arBall[i];
		SetRect(&ballrt,b.x-b.r,b.y-b.r,b.x+b.r,b.y+b.r);
		for (j=0;j<MAXBULLET;j++) {
			if (arBullet[j].Exist == FALSE) continue;
			SetRect(&bulrt,arBullet[j].x-3,arBullet[j].y,arBullet[j].x+3,arBullet[j].y+16);
			IntersectRect(&irt,&ballrt,&bulrt);
			if (IsRectEmpty(&irt) == FALSE) {
				ballrgn=CreateEllipticRgn(b.x-b.r,b.y-b.r,b.x+b.r,b.y+b.r);
				if (RectInRegion(ballrgn,&bulrt)) {
					MessageBeep(0);
					SplitBall(i);
					arBullet[j].Exist=FALSE;
				}
				DeleteObject(ballrgn);
			}
		}
	}

	// 공과 사람의 충돌 판정
	SetRect(&mrt,cx-6,cy+5,cx+6,cy+16);
	for (i=0;i<MAXBALL;i++) {
		if (arBall[i].Exist == FALSE) continue;
		b=arBall[i];
		SetRect(&ballrt,b.x-b.r,b.y-b.r,b.x+b.r,b.y+b.r);
		IntersectRect(&irt,&ballrt,&mrt);
		if (IsRectEmpty(&irt) == FALSE) {
			ballrgn=CreateEllipticRgn(b.x-b.r,b.y-b.r,b.x+b.r,b.y+b.r);
			if (RectInRegion(ballrgn,&mrt)) {
				nLife--;
				if (nLife == 0) {
					Status=STOP;
					KillTimer(hWndMain,1);
					InvalidateRect(hWndMain,NULL,TRUE);
				} else {
					UpdateWindow(hWndMain);
					Sleep(1000);
					StartStage(TRUE);
				}
			}
			DeleteObject(ballrgn);
		}
	}
}

void StartStage(BOOL bReset)
{
	int i;

	for (i=0;i<MAXBULLET;i++) {
		arBullet[i].Exist=FALSE;
	}
	for (i=0;i<MAXBALL;i++) {
		arBall[i].Exist=FALSE;
	}
	for (i=0;i<nStage;i++) {
		NewBall(i % 2 == 0);
	}
	if (bReset) cx=320;
}

void NewBall(BOOL bLeft)
{
	int i;
	for (i=0;i<MAXBALL;i++) {
		if (arBall[i].Exist == FALSE) {
			break;
		}
	};

	if (i != MAXBALL) {
		arBall[i].dx=2.0+(rand()%20)/10.0;
		if (bLeft) {
			arBall[i].x=45;
		} else {
			arBall[i].x=595;
			arBall[i].dx *= -1;
		}
		arBall[i].y=45;
		arBall[i].dy=(rand()%10)/10.0;
		arBall[i].r = 40;
		arBall[i].col=RGB(rand()%200+25,rand()%200+25,rand()%200+25);
		arBall[i].Exist=TRUE;
	}
}

void MoveBall()
{
	int i;
	tag_Ball B;
	RECT crt;

	GetClientRect(hWndMain,&crt);
	for (i=0;i<MAXBALL;i++) {
		if (arBall[i].Exist) {
			B=arBall[i];

			// X축의 이동 - dx는 공기저항만큼 감소하되 0의 경계를 넘지 않는다.
			if (B.dx >= 0) {
				B.dx = max(0,B.dx-AR);
			} else {
				B.dx = min(0,B.dx+AR);
			}
			B.x += B.dx;

			// 오른쪽 벽에 닿음 - 반지름을 고려하여 벗어난만큼 왼쪽으로 반사
			if (B.x+B.r >= crt.right) {
				B.x = crt.right-(B.x+B.r-crt.right)-B.r;
				B.dx *= -1;
			}
			// 왼쪽 벽에 닿음
			if (B.x-B.r <= 0) {
				B.x = 0-(B.x-B.r)+B.r;
				B.dx *= -1;
			}

			// Y축의 이동
			// 하강할 때 - dy가 가속도만큼 계속 증가한다.
			if (B.dy >= 0) {
				B.dy += A;
			// 상승할 때 - 가속도 A는 탄성에 의해 반발력이 되며 중력의 영향을 받는다.
			} else {
				B.dy += A+G;
			}
			B.y += B.dy;

			// 바닥에 닿은 경우 - dy의 부호를 바꾸어 위로 튀어 오르도록 한다.
			if (B.y+B.r >= crt.bottom) {
				B.y = crt.bottom-(B.y+B.r-crt.bottom)-B.r;
				B.dy *= -1;
				// 수직 움직임이 둔해지면 공을 제거한다.
				if (fabs(B.dy) < 1.0) {
					B.Exist = FALSE;
				}
			}

			// 수평 움직임이 둔해지면 공을 제거한다.
			if (fabs(B.dx) < 0.1) {
				B.Exist = FALSE;
			}
			arBall[i]=B;
		}
	}

}

void MoveBullet()
{
	int i;
	for (i=0;i<MAXBULLET;i++) {
		if (arBullet[i].Exist == TRUE) {
			arBullet[i].y-=BULSPEED;
			if (arBullet[i].y <= 0) {
				arBullet[i].Exist=FALSE;
			}
		}
	}
}

void SplitBall(int idx)
{
	int i;
	tag_Ball b;

	b=arBall[idx];
	if (b.r == 10) {
		nScore+=2;
		arBall[idx].Exist=FALSE;
		for (i=0;i<MAXBALL;i++) {
			if (arBall[i].Exist == TRUE) break;
		}
		if (i == MAXBALL) {
#ifdef STRESS
			nStage+=3;
#else
			nStage++;
#endif
			Sleep(500);
			StartStage(FALSE);
		}
		return;
	}

	for (i=0;i<MAXBALL;i++) {
		if (arBall[i].Exist == FALSE) break;
	}
	if (i == MAXBALL) i=idx;

	nScore++;
	arBall[i].r=arBall[idx].r=b.r-10;
	arBall[i].col=b.col;
	arBall[i].Exist=TRUE;
	arBall[i].x=b.x;
	arBall[i].y=b.y;
	arBall[i].dx=-b.dx;
#ifdef STRESS
	arBall[i].dy=arBall[idx].dy=-4;
#else
	arBall[i].dy=arBall[idx].dy=-5;
#endif
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

