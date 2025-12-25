#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ListTyle");

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

#define ID_LISTBOX 100
TCHAR *Items[]={"Apple","Orange","Melon","Grape","Strawberry",
"서울", "부산", "대전", "대구", "광주", "인천", "울산", "제주",
"이선히", "전영룩", "주용필", "강술지", "서태쥐", "양바", "설온도",
"신운경", "박중훤", "이경엉", "재시라", "진채영", "김휘선", "김해수"};
HWND hList;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	int i;
	TCHAR str[128];
	switch (iMessage) {
	case WM_CREATE:
		hList=CreateWindow(TEXT("listbox"),NULL,WS_CHILD | WS_VISIBLE | WS_BORDER |	
			WS_VSCROLL, 
			10,10,300,200,hWnd,(HMENU)ID_LISTBOX,g_hInst,NULL);
//		SendMessage(hList, LB_SETCOLUMNWIDTH, 100, 0);
		for (i=0;i<sizeof(Items)/sizeof(Items[0]);i++) {
			SendMessage(hList,LB_ADDSTRING,0,(LPARAM)Items[i]);
		}
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case ID_LISTBOX:
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				i=SendMessage(hList, LB_GETCURSEL,0,0);
				SendMessage(hList, LB_GETTEXT, i, (LPARAM)str);
				SetWindowText(hWnd, str);
				break;
			}
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
