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
	TCHAR name[20];
	TCHAR tel[20];
	TCHAR addr[50];
	BOOL male;
} people[]={
	{"김상형","123-4567","서울시 강남구 논현동",TRUE},
	{"이송우","543-9876","서울시 구의동",TRUE},
	{"박다희","111-3333","경기도 광명시",FALSE},
	{"오궁섭","236-1818","서울시 강남구 반포동",TRUE},
	{"조기순","358-2277","서울시 압구정동",FALSE},
	{"오뱅훈","548-1109","서울시 신사동",TRUE},
};

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	LVCOLUMN COL;
	LVITEM LI;
	int i;
	HIMAGELIST hImgSm, hImgLa;
	static HWND hList;
	static TCHAR Mes[255]="선택된 항목이 없습니다";
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR Caption[255];

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
		return 0;
	case WM_NOTIFY:
		LPNMHDR hdr;
		LPNMLISTVIEW nlv;
		LPNMITEMACTIVATE nia;
		hdr=(LPNMHDR)lParam;
		nlv=(LPNMLISTVIEW)lParam;
		if (hdr->hwndFrom == hList) {
			switch (hdr->code) {
//* 선택된 항목을 보여준다.
			case LVN_ITEMCHANGED:
				if (nlv->uChanged == LVIF_STATE && nlv->uNewState == 
						(LVIS_SELECTED | LVIS_FOCUSED)) {
					ListView_GetItemText(hList,nlv->iItem,0,Caption,255);
					wsprintf(Mes, "현재 선택된 항목 = %s", Caption);
					InvalidateRect(hWnd, NULL, TRUE);
				}
				break;
//*/
/* 세 번째 항목 선택 금지
			case LVN_ITEMCHANGING:
				if (nlv->uChanged == LVIF_STATE && nlv->iItem == 2 && 
					nlv->uNewState == (LVIS_SELECTED | LVIS_FOCUSED)) {
					wsprintf(Mes, "이 항목은 선택할 수 없습니다");
					InvalidateRect(hWnd, NULL, TRUE);
					return TRUE;
				} else {
					return FALSE;
				}
//*/
/*
			case NM_CLICK:
				nia=(LPNMITEMACTIVATE)lParam;
				ListView_GetItemText(hList,nia->iItem,0,Caption,255);
				wsprintf(Mes, "클릭좌표=(%d,%d), 항목=(%d,%d), 텍스트=%s",
					nia->ptAction.x, nia->ptAction.y, nia->iItem, nia->iSubItem, Caption);
				InvalidateRect(hWnd, NULL, TRUE);
				break;
//*/
			}
		}
		return 0;
	case WM_LBUTTONDOWN:
		int idx;
		idx=ListView_GetNextItem(hList,-1,LVNI_ALL | LVNI_SELECTED);
		if (idx == -1)  {
			MessageBox(hWnd,"선택된 항목이 없음","알림",MB_OK);
		} else {
			do {
				ListView_GetItemText(hList, idx, 0, Caption, 255);
				MessageBox(hWnd,Caption,"선택된 항목",MB_OK);
				idx=ListView_GetNextItem(hList,idx,LVNI_ALL | LVNI_SELECTED);
			} while (idx != -1);
		}
		return 0;
	case WM_RBUTTONDOWN:
		ListView_SetItemState(hList,-1,0,LVIS_FOCUSED | LVIS_SELECTED);
		ListView_SetItemState(hList,1,LVIS_FOCUSED | LVIS_SELECTED,
			LVIS_FOCUSED | LVIS_SELECTED);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		SetBkMode(hdc,TRANSPARENT);
		TextOut(hdc,10,220,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

