#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("MultiSel");

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
#define ID_BTNADD 101
#define ID_BTNDELETE 102
HWND hList, hBtnAdd, hBtnDelete;
TCHAR *data[]={"하나","둘","삼","넷","오","여섯","칠","팔","아홉","공"};
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	int i,num;
	int buffer[100];
	switch (iMessage) {
	case WM_CREATE:
		// 리스트 박스와 버튼 두 개를 생성한다.
		hList=CreateWindow(TEXT("listbox"),NULL,WS_CHILD | WS_VISIBLE | WS_BORDER |
			WS_VSCROLL | LBS_MULTIPLESEL,
			10,10,200,300,hWnd,(HMENU)ID_LISTBOX,g_hInst,NULL);
		hBtnAdd=CreateWindow(TEXT("button"), TEXT("Add Strings"), 
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			220,50,100,25,hWnd,(HMENU)ID_BTNADD,g_hInst,NULL);
		hBtnDelete=CreateWindow(TEXT("button"), TEXT("Delete String"), 
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			220,80,100,25,hWnd,(HMENU)ID_BTNDELETE,g_hInst,NULL);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		// 아홉개의 문자열을 리스트 박스에 추가한다.
		case ID_BTNADD:
			SendMessage(hList, LB_RESETCONTENT, 0, 0);
			for (i=0;i<10;i++) {
				SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)data[i]);
			}
			break;
		// 선택된 모든 항목들을 삭제한다.
		case ID_BTNDELETE:
			num=SendMessage(hList, LB_GETSELCOUNT,0,0);
			if (num!=0) {
				SendMessage(hList, LB_GETSELITEMS,100,(LPARAM)buffer);
				for (i=num-1;i>-1;i--)
					SendMessage(hList,LB_DELETESTRING,buffer[i],0);
			}
			break;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
