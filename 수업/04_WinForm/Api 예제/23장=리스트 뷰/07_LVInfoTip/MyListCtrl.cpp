#include <windows.h>
#include <commctrl.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("MyListCtrl");

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

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);

	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

struct tag_people {
	TCHAR name[80];
	TCHAR tel[80];
	TCHAR addr[50];
	BOOL male;
} people[]={
	{"김상형=이 프로그램을 만든 사람","아주 긴 전화번호 항목 123-4567","서울시 강남구 논현동",TRUE},
	{"이송우=ASP의 달인","이 사람은 집이 너무 가난해서 전화가 없습니다.","서울시 구의동",TRUE},
	{"박다희=MFC의 달인","집에 안들어가기 때문에 전화번호가 의미가 없습니다.","경기도 광명시",FALSE},
	{"오궁섭=자랑스런 우리의 팀장님","전화보다 핸드폰을 더 많이 사용합니다.","서울시 강남구 반포동",TRUE},
	{"조기순=설치 프로그램의 대가","358-2277","서울시 압구정동",FALSE},
	{"오뱅훈=SI의 정수","548-1109","서울시 신사동",TRUE},
};

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	LVCOLUMN COL;
	LVITEM LI;
	int i;
	HIMAGELIST hImgSm, hImgLa;
	static HWND hList;

	switch (iMessage) {
	case WM_CREATE:
		InitCommonControls();

		// 리스트 컨트롤을 자세히 보기로 만든다.
		hList=CreateWindow(WC_LISTVIEW,NULL,WS_CHILD | WS_VISIBLE | WS_BORDER |
			LVS_REPORT | LVS_SHOWSELALWAYS,
			10,10,600,200,hWnd,NULL,g_hInst,NULL);

		// 크기별로 두 종류의 이미지 리스트를 만들고 리스트 컨트롤에 연결한다.
		hImgSm=ImageList_LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BIT16), 
			16, 1, RGB(255,255,255));
		hImgLa=ImageList_LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BIT32), 
			32, 1, RGB(255,255,255));
		ListView_SetImageList(hList, hImgSm, LVSIL_SMALL);
		ListView_SetImageList(hList, hImgLa, LVSIL_NORMAL);

		// 헤더를 추가한다.
		COL.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT |LVCF_SUBITEM;
		COL.fmt=LVCFMT_LEFT;
		COL.cx=150;
		COL.pszText="이름";			// 첫번째 헤더
		COL.iSubItem=0;
		ListView_InsertColumn(hList,0,&COL);

		COL.pszText="전화번호";		// 두번째 헤더
		COL.iSubItem=1;
		ListView_InsertColumn(hList,1,&COL);

		COL.cx=300;
		COL.pszText="주소";			// 세번째 헤더
		COL.iSubItem=2;
		ListView_InsertColumn(hList,2,&COL);

		// 텍스트와 이미지를 가진 아이템들을 등록한다.
		for (i=0;i<sizeof(people)/sizeof(people[0]);i++) {
			LI.mask=LVIF_TEXT | LVIF_IMAGE;
			LI.iItem=i;
			LI.iSubItem=0;
			LI.pszText=people[i].name;
			LI.iImage=(people[i].male ? 0:1);
			ListView_InsertItem(hList,&LI);

			ListView_SetItemText(hList,i,1,people[i].tel);
			ListView_SetItemText(hList,i,2,people[i].addr);
		}
		ListView_SetExtendedListViewStyle(hList, LVS_EX_INFOTIP);
		return 0;
	case WM_NOTIFY:
		LPNMHDR hdr;
		LPNMLVGETINFOTIP ngt;
		hdr=(LPNMHDR)lParam;
		ngt=(LPNMLVGETINFOTIP)lParam;
		TCHAR str[255];
		if (hdr->hwndFrom == hList) {
			switch (hdr->code) {
			case LVN_GETINFOTIP:
				if (ngt->iItem == 0) {
					lstrcpy(ngt->pszText, "정말 멋있고 똑똑한 사람\r\n이 프로그램을 만든 사람");
				}
				wsprintf(str, "iItem=%d 항목 Text=%s",ngt->iItem, ngt->pszText);
				SetWindowText(hWnd,str);
				return 0;
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

