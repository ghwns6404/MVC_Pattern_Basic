#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("HighCounter");

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

#include <stdio.h>
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	LARGE_INTEGER st,ed,freq;
	int i;
	int a,b=123,c=456;
	double gap;
	static TCHAR Mes[128]="마우스 왼쪽 버튼을 누르면 카운트를 시작합니다";
	static TCHAR Result[128];

	switch (iMessage) {
	case WM_CREATE:
		QueryPerformanceCounter(&st);
		if (st.QuadPart == 0) {
			MessageBox(hWnd, "이 시스템은 고해상도 타이머를 지원하지 않습니다",
				"알림",MB_OK);
			DestroyWindow(hWnd);
		}
		return 0;
	case WM_LBUTTONDOWN:
		SetCursor(LoadCursor(NULL,IDC_WAIT));
		QueryPerformanceCounter(&st);

		// 뎃셈 5억번 하기
		for (i=0;i<500000000;i++) {
			a=b+c;
		}

		QueryPerformanceCounter(&ed);
		QueryPerformanceFrequency(&freq);
		gap=((double)(ed.QuadPart-st.QuadPart))/((double)freq.QuadPart);
		sprintf(Mes, "덧셈 5억번에 총 %.4f초가 걸렸습니다.",gap);
		wsprintf(Result, "Frequency = %d", freq.QuadPart);
		InvalidateRect(hWnd,NULL,TRUE);
		SetCursor(LoadCursor(NULL,IDC_ARROW));
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,50,50,Mes,lstrlen(Mes));
		TextOut(hdc,50,70,Result,lstrlen(Result));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

