#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("RegFormat");

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

UINT MyFormat;
struct tag_Movie {
	TCHAR Name[32];
	int MinuteLength;
	unsigned Year;
	TCHAR Star[16];
};
tag_Movie Movie[]={
	{"Mr.소크라테스",120,2005,"김래운"},
	{"웰컴 투 동막골",110,2005,"마을 사람들"},
	{"투캅스",120,2005,"박중혼,안송기"},
};
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HGLOBAL hmem;
	tag_Movie *pMovie;
	TCHAR *Mes="1~3 : 영화 정보 복사, 마우스 클릭 : 포맷 붙여넣기";
	static TCHAR str[256];

	switch (iMessage) {
	case WM_CREATE:
		MyFormat=RegisterClipboardFormat("MOVIEINFO_BYWINAPI");
		if (MyFormat==0) {
			// 등록 실패시의 에러 처리
		}
		return 0;
	case WM_CHAR:
		if (wParam >= '1' && wParam <= '3') {
			hmem=GlobalAlloc(GHND,sizeof(tag_Movie));
			pMovie=(tag_Movie *)GlobalLock(hmem);
			memcpy(pMovie,&Movie[wParam-'1'],sizeof(tag_Movie));
			GlobalUnlock(hmem);
			if (OpenClipboard(hWnd)) {
				EmptyClipboard();
				SetClipboardData(MyFormat,hmem);
				CloseClipboard();
			}
		}
		return 0;
	case WM_LBUTTONDOWN:
		if (IsClipboardFormatAvailable(MyFormat)) {
			OpenClipboard(hWnd);
			hmem=GetClipboardData(MyFormat);
			pMovie=(tag_Movie *)GlobalLock(hmem);
			wsprintf(str, "제목 = %s, 상영시간 = %d분, 개봉년도 = %d, 주연 = %s",
				pMovie->Name, pMovie->MinuteLength, pMovie->Year, pMovie->Star);
			GlobalUnlock(hmem);
			CloseClipboard();
		} else {
			lstrcpy(str,"클립보드에 영화 정보가 없습니다.");
		}
		InvalidateRect(hWnd,NULL,TRUE);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,10,10,Mes,lstrlen(Mes));
		TextOut(hdc,10,30,str,lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

