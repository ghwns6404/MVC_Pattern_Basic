#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ItemData");

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

struct tagMyCom {
	TCHAR CPU[20];
	int Memory;
	int HDD;
	TCHAR VGA[20];
	BOOL bCDRom;
} MyCom[3] = {
	{TEXT("Pentium 166"), 64, 12000, TEXT("WinX Perfect II"), TRUE},
	{TEXT("Pentium 100"), 32, 2500 , TEXT("Trident"),FALSE},
	{TEXT("Pentinum MMX 166"), 48, 2100, TEXT("Cyrus Logic"), TRUE}};
TCHAR *MySystem[3]={TEXT("Desktop"),TEXT("Server"),TEXT("Notebook")};
#define ID_LISTBOX 100
HWND hList;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int i, index;
	TCHAR str[128];
	tagMyCom *pCom;
	switch (iMessage) {
	case WM_CREATE:
		hList=CreateWindow(TEXT("listbox"),NULL,WS_CHILD | WS_VISIBLE | 
			WS_BORDER |	WS_VSCROLL | LBS_NOTIFY,
			10,10,100,200,hWnd,(HMENU)ID_LISTBOX,g_hInst,NULL);
		for (i=0;i<sizeof(MySystem)/sizeof(MySystem[0]);i++) {
			SendMessage(hList,LB_ADDSTRING,0,(LPARAM)MySystem[i]);
			SendMessage(hList,LB_SETITEMDATA,i,(LPARAM)&MyCom[i]);
		}
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_LISTBOX:
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				InvalidateRect(hWnd,NULL,TRUE);
				break;
			}
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		index=SendMessage(hList,LB_GETCURSEL,0,0);
		if (index != -1) {
			pCom=(tagMyCom *)SendMessage(hList,LB_GETITEMDATA,index,0);
			wsprintf(str,TEXT("CPU : %s"),pCom->CPU);
			TextOut(hdc,120,10,str,lstrlen(str));
			wsprintf(str,TEXT("Memory : %dMBytes"),pCom->Memory);
			TextOut(hdc,120,30,str,lstrlen(str));
			wsprintf(str,TEXT("HDD : %dMBytes"),pCom->HDD);
			TextOut(hdc,120,50,str,lstrlen(str));
			wsprintf(str,TEXT("CD-ROM : %s"),pCom->bCDRom ? TEXT("Yes"):TEXT("No"));
			TextOut(hdc,120,70,str,lstrlen(str));
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

