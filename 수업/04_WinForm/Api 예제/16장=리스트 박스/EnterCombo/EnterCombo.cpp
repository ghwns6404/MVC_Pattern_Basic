#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("EnterCombo");

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

#define ID_COMBOBOX 100
WNDPROC OldEditProc;
LRESULT CALLBACK EditProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage) {
	case WM_KEYDOWN:
		if (wParam == VK_RETURN) {
			SendMessage(hWndMain, WM_USER+1,0,0);
			return 0;
		}
		break;
	case WM_KEYUP:
	case WM_CHAR:
		if (wParam == VK_RETURN) {
			return 0;
		}
		break;
	}

	return CallWindowProc(OldEditProc,hWnd,iMessage,wParam,lParam);
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Items[]={TEXT("±èÄ¡Âî°Ô"),TEXT("µ·°¡½º"),TEXT("°¨ÀÚÅÁ"),
		TEXT("ÇÜ¹ö°Å"),TEXT("»§")};
	int i;
	static HWND hCombo;
	static HWND hEdit;
	static TCHAR str[128]=TEXT("¸Þ´º¸¦ ¼±ÅÃÇØ ÁÖ½Ê½Ã¿À");
	static TCHAR Food[128];
	POINT pt;

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		hCombo=CreateWindow("combobox",NULL,WS_CHILD | WS_VISIBLE | CBS_DROPDOWN,
			10,10,150,200,hWnd,(HMENU)ID_COMBOBOX,g_hInst,NULL);
		for (i=0;i<5;i++)
			SendMessage(hCombo,CB_ADDSTRING,0,(LPARAM)Items[i]);
		pt.x=pt.y=5;
		hEdit=ChildWindowFromPoint(hCombo,pt);
		//hEdit=GetWindow(hCombo,GW_CHILD);
		OldEditProc=(WNDPROC)SetWindowLongPtr(hEdit,GWLP_WNDPROC,(LONG_PTR)EditProc);
		return 0;
	case WM_USER+1:
		GetWindowText(hCombo, Food, 128);
		InvalidateRect(hWnd,NULL,TRUE);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_COMBOBOX:
			switch (HIWORD(wParam)) {
			case CBN_SELCHANGE:
				i=SendMessage(hCombo, CB_GETCURSEL,0,0);
				SendMessage(hCombo, CB_GETLBTEXT, i, (LPARAM)Food);
				InvalidateRect(hWnd,NULL,TRUE);
				break;
			}
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		wsprintf(str, TEXT("¿À´Ã ¸Þ´º´Â '%s'ÀÔ´Ï´Ù"), Food);
		TextOut(hdc,10,150,str,lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		SetWindowLongPtr(hEdit,GWLP_WNDPROC,(LONG_PTR)OldEditProc);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
