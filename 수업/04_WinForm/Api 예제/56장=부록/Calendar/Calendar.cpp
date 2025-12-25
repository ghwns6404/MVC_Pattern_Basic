#define _WIN32_WINNT 0x400
#define _WIN32_WINDOWS 0x401
#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("Calendar");

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
	WndClass.style=0;
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

void DrawBitmap(HDC hdc,int x,int y,HBITMAP hBit);
void DrawCalendar();
int GetYoil(int year,int month,int day);
int GetMonthEnd(int year,int month);
void DrawMonth(HDC hdc,int year,int month,int sx,int sy);
int NowYear;
int NowMonth;
SYSTEMTIME Today;
HBITMAP hBit;
#define DGAP 25
#define LGAP 20
int MXGAP=200;
int MYGAP=180;

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMessage) {
	case WM_CREATE:
		GetLocalTime(&Today);
		NowYear=Today.wYear;
		NowMonth=1;
		hWndMain=hWnd;
		return 0;
	case WM_MOUSEWHEEL:
		if ((short)HIWORD(wParam) > 0) {
			SendMessage(hWnd,WM_KEYDOWN,VK_UP,0);
		} else {
			SendMessage(hWnd,WM_KEYDOWN,VK_DOWN,0);
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		if (hBit==NULL) {
			DrawCalendar();
		}
		DrawBitmap(hdc,0,0,hBit);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_DOWN:
			if (NowMonth == 10) {
				NowMonth = 1;
				NowYear++;
			} else {
				NowMonth += 3;
			}
			SendMessage(hWnd,WM_SIZE,0,0);
			break;
		case VK_UP:
			if (NowMonth == 1) {
				NowMonth = 10;
				NowYear--;
			} else {
				NowMonth -= 3;
			}
			SendMessage(hWnd,WM_SIZE,0,0);
			break;
		case VK_NEXT:
			NowYear++;
			SendMessage(hWnd,WM_SIZE,0,0);
			break;
		case VK_PRIOR:
			NowYear--;
			SendMessage(hWnd,WM_SIZE,0,0);
			break;
		}
		return 0;
	case WM_SIZE:
		if (wParam != SIZE_MINIMIZED) {
			DeleteObject(hBit);
			hBit=NULL;
			InvalidateRect(hWnd,NULL,FALSE);
		}
		return 0;
	case WM_DESTROY:
		DeleteObject(hBit);
		hBit=NULL;
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

int GetYoil(int year,int month,int day)
{
	SYSTEMTIME st;
	FILETIME ft;

	memset(&st,0,sizeof(st));
	st.wYear=year;
	st.wMonth=month;
	st.wDay=day;
	SystemTimeToFileTime(&st,&ft);
	FileTimeToSystemTime(&ft,&st);
	return st.wDayOfWeek;
}

int GetMonthEnd(int year,int month)
{
	static int days[]={0,31,28,31,30,31,30,31,31,30,31,30,31};
	int lastday;
	
	lastday=days[month];
	if (month == 2 && ((year%4 == 0 && year%100!=0) || year%400==0)) {
		lastday=29;
	}
	return lastday;
}

void DrawMonth(HDC hdc,int year,int month,int sx,int sy)
{
	int yoil;
	int day;
	int MonthEnd;
	TCHAR str[128];
	RECT rt;
	HBRUSH Brush,OldBrush;
	HFONT hFont, OldFont;
	HPEN Pen,OldPen;

	// 월 표시 사각형 출력
	SetRect(&rt,sx,sy,sx+DGAP*7,sy+30);
	Brush=CreateSolidBrush(RGB(0,0,255));
	FillRect(hdc,&rt,Brush);
	DeleteObject(Brush);

	// 년월 출력
	SetBkMode(hdc,TRANSPARENT);
	SetTextAlign(hdc,TA_LEFT);
	SetTextColor(hdc,RGB(255,255,255));
	wsprintf(str,"%d년 %d월",year,month);
	hFont=CreateFont(20,0,0,0,FW_BOLD,0,0,0,HANGEUL_CHARSET,3,2,1,
		VARIABLE_PITCH | FF_MODERN,"굴림");
	OldFont=(HFONT)SelectObject(hdc,hFont);
	TextOut(hdc,sx+5,sy+5,str,lstrlen(str));
	SelectObject(hdc,OldFont);
	DeleteObject(hFont);
	sy+=35;

	// 달력 출력
	yoil=GetYoil(year,month,1);
	MonthEnd=GetMonthEnd(year,month);
	SetTextAlign(hdc,TA_RIGHT);
	for (day=1;day<=MonthEnd;day++) {
		if (year==Today.wYear && month==Today.wMonth && day==Today.wDay) {
			Pen=CreatePen(PS_SOLID,2,RGB(0,0,0));
			OldPen=(HPEN)SelectObject(hdc,Pen);
			OldBrush=(HBRUSH)SelectObject(hdc,GetStockObject(NULL_BRUSH));
			Rectangle(hdc,sx+(yoil+1)*DGAP-18,sy-2,sx+(yoil+1)*DGAP+3,sy+18);
			DeleteObject(SelectObject(hdc,OldPen));
			SelectObject(hdc,OldBrush);
		}
		wsprintf(str,"%d",day);
		if (yoil==0) {
			SetTextColor(hdc,RGB(255,0,0));
		} else {
			SetTextColor(hdc,RGB(96,96,96));
		}
		TextOut(hdc,sx+(yoil+1)*DGAP,sy,str,lstrlen(str));
		if (yoil==0) {
			TextOut(hdc,sx+(yoil+1)*DGAP+1,sy,str,lstrlen(str));
		}
		yoil++;
		if (yoil==7) {
			yoil=0;
			sy+=LGAP;
		}
	}
}

void DrawCalendar()
{
	int x=0,y=0;
	int month,year;
	RECT crt;
	HDC hdc,hMemDC;
	HBITMAP OldBit;

	GetClientRect(hWndMain,&crt);
	hdc=GetDC(hWndMain);
	if (hBit==NULL) {
		hBit=CreateCompatibleBitmap(hdc,crt.right,crt.bottom);
	}
	hMemDC=CreateCompatibleDC(hdc);
	OldBit=(HBITMAP)SelectObject(hMemDC,hBit);
	FillRect(hMemDC,&crt,GetSysColorBrush(COLOR_WINDOW));

	for (year=NowYear,month=NowMonth;;month++) {
		DrawMonth(hMemDC,year,month,x,y);
		if (month%3 == 0) {
			x=0;
			y+=MYGAP;
			if (month == 12) {
				month=0;
				year++;
			}
		} else {
			x+=MXGAP;
		}
		if (y > crt.bottom) break;
	}
	SelectObject(hMemDC,OldBit);
	DeleteDC(hMemDC);
	ReleaseDC(hWndMain,hdc);
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

