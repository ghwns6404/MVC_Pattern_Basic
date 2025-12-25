#define WINVER 0x500
#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("SysColor");

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

struct tag_SysColor {
	int nIndex;
	TCHAR *Name;
}arSC[]={
	{COLOR_3DDKSHADOW,"COLOR_3DDKSHADOW"},
	{COLOR_3DFACE,"COLOR_3DFACE"},
	{COLOR_BTNFACE,"COLOR_BTNFACE"},
	{COLOR_3DHILIGHT,"COLOR_3DHILIGHT"},
	{COLOR_3DLIGHT,"COLOR_3DLIGHT"},
	{COLOR_3DSHADOW,"COLOR_3DSHADOW"},
	{COLOR_ACTIVEBORDER,"COLOR_ACTIVEBORDER"},
	{COLOR_ACTIVECAPTION,"COLOR_ACTIVECAPTION"},
	{COLOR_APPWORKSPACE,"COLOR_APPWORKSPACE"},
	{COLOR_BACKGROUND,"COLOR_BACKGROUND"},
	{COLOR_BTNTEXT,"COLOR_BTNTEXT"},
	{COLOR_CAPTIONTEXT,"COLOR_CAPTIONTEXT"},
	{COLOR_GRADIENTACTIVECAPTION,"COLOR_GRADIENTACTIVECAPTION"},
	{COLOR_GRADIENTINACTIVECAPTION,"COLOR_GRADIENTINACTIVECAPTION"},
	{COLOR_GRAYTEXT,"COLOR_GRAYTEXT"},
	{COLOR_HIGHLIGHT,"COLOR_HIGHLIGHT"},
	{COLOR_HIGHLIGHTTEXT,"COLOR_HIGHLIGHTTEXT"},
	{COLOR_HOTLIGHT,"COLOR_HOTLIGHT"},
	{COLOR_INACTIVEBORDER,"COLOR_INACTIVEBORDER"},
	{COLOR_INACTIVECAPTION,"COLOR_INACTIVECAPTION"},
	{COLOR_INACTIVECAPTIONTEXT,"COLOR_INACTIVECAPTIONTEXT"},
	{COLOR_INFOBK,"COLOR_INFOBK"},
	{COLOR_INFOTEXT,"COLOR_INFOTEXT"},
	{COLOR_MENU,"COLOR_MENU"},
	{COLOR_MENUTEXT,"COLOR_MENUTEXT"},
	{COLOR_SCROLLBAR,"COLOR_SCROLLBAR"},
	{COLOR_WINDOW,"COLOR_WINDOW"},
	{COLOR_WINDOWFRAME,"COLOR_WINDOWFRAME"},
	{COLOR_WINDOWTEXT,"COLOR_WINDOWTEXT"},
};

void PrintSysColor(HDC hdc)
{
	int i;
	TCHAR str[255];
	HBRUSH hSys,hOld;

	for (i=0;i<sizeof(arSC)/sizeof(arSC[0]);i++) {
		wsprintf(str,"%s=%X",arSC[i].Name,GetSysColor(arSC[i].nIndex));
		TextOut(hdc,0,i*20+20,str,lstrlen(str));
		hSys=GetSysColorBrush(arSC[i].nIndex);
		//hSys=CreateSolidBrush(GetSysColor(arSC[i].nIndex));
		hOld=(HBRUSH)SelectObject(hdc,hSys);
		Rectangle(hdc,300,i*20+20,400,i*20+38);
		SelectObject(hdc,hOld);
		//DeleteObject(hSys);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int arSysIndex[3]={COLOR_ACTIVECAPTION, COLOR_BACKGROUND, COLOR_WINDOW};
	COLORREF arSysColor[3]={RGB(255,0,0),RGB(0,255,0),RGB(128,128,128)};
	TCHAR *Mes="오른쪽 버튼을 누르면 제목 표시줄, 바탕화면, 윈도우 색상을 변경합니다";

	switch (iMessage) {
	case WM_RBUTTONDOWN:
		SetSysColors(3,arSysIndex,arSysColor);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		PrintSysColor(hdc);
		TextOut(hdc,0,2,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

