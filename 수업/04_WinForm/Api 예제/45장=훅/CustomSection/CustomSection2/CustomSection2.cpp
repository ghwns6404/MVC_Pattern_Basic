#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("CustomSection2");

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
		10,400,600,300,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);

	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

__declspec(dllimport) TCHAR buf[1024];
#define WM_SYNCSHAREMEMORY WM_USER+1
#define MAXSHAREMEMORY 1024
#define ID_EDIT 100
HWND hEdit;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HWND hTarget;
	TCHAR *Mes=TEXT("공유 섹션을 사용하여 두 프로세스가 메모리를 공유하는 예제");

	switch (iMessage) {
	case WM_CREATE:
		hEdit=CreateWindow("edit",NULL,WS_CHILD | WS_VISIBLE | WS_BORDER | 
			WS_VSCROLL | ES_MULTILINE,10,10,500,200,hWnd,(HMENU)ID_EDIT,g_hInst,NULL);
		SendMessage(hEdit,EM_LIMITTEXT,MAXSHAREMEMORY,0);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_EDIT:
			switch (HIWORD(wParam)) {
			case EN_CHANGE:
				GetWindowText(hEdit,buf,MAXSHAREMEMORY);
				hTarget=FindWindow(NULL,"CustomSection1");
				if (hTarget)
					SendMessage(hTarget,WM_SYNCSHAREMEMORY,0,0);
				break;
			}
		}
		return 0;
	case WM_SYNCSHAREMEMORY:
		SetWindowText(hEdit,buf);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,10,220,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
