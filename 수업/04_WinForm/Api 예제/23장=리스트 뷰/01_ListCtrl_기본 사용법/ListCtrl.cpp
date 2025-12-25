#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ListCtrl");

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;

	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)(COLOR_BTNFACE+1);
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

#include <commctrl.h>
#include "resource.h"
HWND hList;
HIMAGELIST hImgSm, hImgLa;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	LVCOLUMN COL;
	LVITEM LI;
	switch (iMessage) {
	case WM_CREATE:
		InitCommonControls();

		// 리스트 컨트롤을 자세히 보기로 만든다.
		hList=CreateWindow(WC_LISTVIEW,NULL,WS_CHILD | WS_VISIBLE | WS_BORDER |
			LVS_REPORT,10,10,500,200,hWnd,NULL,g_hInst,NULL);

		// 크기별로 두 종류의 이미지 리스트를 만들고 리스트 컨트롤에 연결한다.
		hImgSm=ImageList_LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BIT16), 
			16, 1, RGB(255,255,255));
		hImgLa=ImageList_LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BIT32), 
			32, 1, RGB(255,255,255));
		SendMessage(hList, LVM_SETIMAGELIST, (WPARAM)LVSIL_SMALL, (LPARAM)hImgSm);
		SendMessage(hList, LVM_SETIMAGELIST, (WPARAM)LVSIL_NORMAL, (LPARAM)hImgLa);

		// 헤더를 추가한다.
		COL.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT |LVCF_SUBITEM;
		COL.fmt=LVCFMT_LEFT;
		COL.cx=150;
		COL.pszText="이름";				// 첫 번째 헤더
		COL.iSubItem=0;
		SendMessage(hList, LVM_INSERTCOLUMN, 0,(LPARAM)&COL);

		COL.pszText="전화번호";			// 두 번째 헤더
		COL.iSubItem=1;
		SendMessage(hList, LVM_INSERTCOLUMN, 1,(LPARAM)&COL);

		COL.cx=300;
		COL.pszText="주소";				// 세 번째 헤더
		COL.iSubItem=2;
		SendMessage(hList, LVM_INSERTCOLUMN, 2,(LPARAM)&COL);

		// 텍스트와 이미지를 가진 아이템들을 등록한다.
		LI.mask=LVIF_TEXT | LVIF_IMAGE;
		
		LI.iImage=0;
		LI.iSubItem=0;
		LI.iItem=0;
		LI.pszText="박미영";			// 첫 번째 아이템
		SendMessage(hList, LVM_INSERTITEM, 0, (LPARAM)&LI);

		LI.iImage=-1;
		LI.iSubItem=1;
		LI.pszText="123-4567";
		SendMessage(hList, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem=2;
		LI.pszText="서울시 논현동";
		SendMessage(hList, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iImage=0;
		LI.iItem=1;
		LI.iSubItem=0;
		LI.pszText="권진숙";			// 두 번째 아이템
		SendMessage(hList, LVM_INSERTITEM, 0, (LPARAM)&LI);

		LI.iImage=-1;
		LI.iSubItem=1;
		LI.pszText="543-9876";
		SendMessage(hList, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem=2;
		LI.pszText="부산시 대신동";
		SendMessage(hList, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iImage=1;
		LI.iItem=2;
		LI.iSubItem=0;
		LI.pszText="허수진";			// 세 번째 아이템
		SendMessage(hList, LVM_INSERTITEM, 0, (LPARAM)&LI);

		LI.iImage=-1;
		LI.iSubItem=1;
		LI.pszText="101-0920";
		SendMessage(hList, LVM_SETITEM, 0, (LPARAM)&LI);

		LI.iSubItem=2;
		LI.pszText="부산시 장전동";
		SendMessage(hList, LVM_SETITEM, 0, (LPARAM)&LI);

		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

