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

HIMAGELIST himl;					// 드래그 이미지 리스트의 핸들
BOOL g_bDrag=FALSE;				// TRUE이면 현재 드래그 중
int hDrag;							// 드래그 중인 항목
int hTarget;						// 드롭 타겟 항목
int hOldTarget=-1;				// 이전 드롭 타겟 항목 

void MoveListItem(HWND hList, int idx, int to)
{
	LVITEM LI;
	int nColumn, i;
	TCHAR Cap1[256];

	// 같은 항목이거나 이동 위치가 정해지지 않았으면 리턴
	if ((idx==to) || (to < 0))
		return;

	// 컬럼의 개수를 구한다.
	nColumn=Header_GetItemCount(ListView_GetHeader(hList));

	// 원본 항목의 정보를 읽는다.
	LI.iItem=idx;
	LI.mask=LVIF_IMAGE | LVIF_TEXT | LVIF_PARAM | LVIF_STATE;
	LI.stateMask=LVIS_FOCUSED | LVIS_SELECTED | LVIS_STATEIMAGEMASK | LVIS_OVERLAYMASK;
	LI.pszText=Cap1;
	LI.cchTextMax=256;
	LI.iSubItem=0;
	ListView_GetItem(hList,&LI);

	// 새 위치에 삽입한다.
	LI.iItem=to;
	ListView_InsertItem(hList,&LI);

	// 삽입된 항목이 원본보다 앞에 있으면 원본의 인덱스가 하나 증가해야 한다.
	if (idx > to)
		idx++;

	// 컬럼 개수만큼의 세부 항목들을 복사한다.
	for (i=1;i<nColumn;i++) {
		ListView_GetItemText(hList,idx,i,Cap1,256);
		ListView_SetItemText(hList,to,i,Cap1);
	}

	// 원본 항목은 삭제한다.
	ListView_DeleteItem(hList,idx);
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	LVCOLUMN COL;
	LVITEM LI;
	int i;
	HIMAGELIST hImgSm, hImgLa;
	static HWND hList;
	TCHAR szTmp[50];
	RECT rcItem;
	int hotx, hoty;
	POINT pt;

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
		for (;i<=50;i++) {
			LI.mask=LVIF_TEXT | LVIF_IMAGE;
			LI.iImage=rand()%2;
			LI.iSubItem=0;
			LI.iItem=i;
			wsprintf(szTmp,"%d번째 항목",i);
			LI.pszText=szTmp;
			ListView_InsertItem(hList,&LI);

			wsprintf(szTmp,"%d번의 전화번호",i);
			ListView_SetItemText(hList,i,1,szTmp);
			wsprintf(szTmp,"%d번의 주소",i);
			ListView_SetItemText(hList,i,2,szTmp);
		}
		return 0;
	case WM_NOTIFY:
		LPNMHDR hdr;
		LPNMLISTVIEW nlv;
		hdr=(LPNMHDR)lParam;
		nlv=(LPNMLISTVIEW)lParam;
		if (hdr->hwndFrom == hList) {
			switch (hdr->code) {
			case LVN_BEGINDRAG:
				// 드래그되는 항목 저장
				hDrag=nlv->iItem;

				// 드래그 이미지 리스트 만듦
				himl=ListView_CreateDragImage(hList, hDrag, &pt);
				if (himl==NULL)
					return 0;

				// 핫 스팟. 커서 위치에서 이미지 좌상단 좌표를 뺀 좌표로 계산된다.
				hotx=nlv->ptAction.x-pt.x;
				hoty=nlv->ptAction.y-pt.y;
				ImageList_BeginDrag(himl,0,hotx,hoty);

				// 현재 마우스 커서 위치에 드래그 이미지 그림
				pt.x=nlv->ptAction.x;
				pt.y=nlv->ptAction.y;
				ClientToScreen(hWnd,&pt);
				ImageList_DragEnter(NULL,pt.x, pt.y);

				// 계속적인 드래그 동작을 위해 커서를 캡처한다.
				SetCapture(hWnd);
				g_bDrag=TRUE;

				return 0;
			}
		}
		break;
	case WM_MOUSEMOVE:
		LVHITTESTINFO hti;
		if (g_bDrag) {
			// 마우스 위치를 화면 좌표로 바꾸어 드래그 이미지를 그린다.
			pt.x=LOWORD(lParam);
			pt.y=HIWORD(lParam);
			ClientToScreen(hWnd,&pt);
			ImageList_DragMove(pt.x, pt.y);

			// 커서 위치의 항목을 조사한다.
			GetWindowRect(hList, &rcItem);
			ScreenToClient(hWnd,(POINT *)&rcItem);
			hti.pt.x=LOWORD(lParam)-rcItem.left;
			hti.pt.y=HIWORD(lParam)-rcItem.top;
			hTarget=ListView_HitTest(hList,&hti);

			// 드롭 타겟 표시
			if (hTarget != -1) {
				ImageList_DragLeave(NULL);
				ListView_SetItemState(hList,hTarget,LVIS_DROPHILITED,LVIS_DROPHILITED);

				if (hTarget != hOldTarget) {
					ListView_SetItemState(hList,hOldTarget,0,LVIS_DROPHILITED);
					hOldTarget=hTarget;
				}

				UpdateWindow(hList);
				ImageList_DragEnter(NULL,pt.x, pt.y);
			}

			// 커서가 리스트의 위에 있으면 위로 스크롤
			GetWindowRect(hList, &rcItem);
			if ((short)pt.y < rcItem.top) {
				SetTimer(hWnd,2,200,NULL);
				SendMessage(hList,WM_VSCROLL,MAKEWPARAM(SB_LINEUP,0),NULL);
			} else {
				KillTimer(hWnd,2);
			}

			// 커서가 리스트의 아래에 있으면 아래로 스크롤
			if ((short)pt.y > rcItem.bottom) {
				SetTimer(hWnd,3,200,NULL);
				SendMessage(hList,WM_VSCROLL,MAKEWPARAM(SB_LINEDOWN,0),NULL);
			} else {
				KillTimer(hWnd,3);
			}
		}
		return 0;
	case WM_LBUTTONUP:
		if (g_bDrag) {
			// 드래그를 종료한다.
			ImageList_DragLeave(NULL);
			ImageList_EndDrag();
			ReleaseCapture();
			g_bDrag=FALSE;
			ImageList_Destroy(himl);

			// 드롭 타겟 표시 제거
			ListView_SetItemState(hList,hTarget,0,LVIS_DROPHILITED);
			MoveListItem(hList,hDrag,hTarget);

			// 스크롤 타이머 파괴
			KillTimer(hWnd,2);
			KillTimer(hWnd,3);
		}
		return 0;
	case WM_TIMER:
		// 0.2초에 한 번씩 위로 스크롤
		if (wParam == 2) {
			SendMessage(hList,WM_VSCROLL,MAKEWPARAM(SB_LINEUP,0),NULL);
		}
		// 0.2초에 한 번씩 아래로 스크롤
		if (wParam == 3) {
			SendMessage(hList,WM_VSCROLL,MAKEWPARAM(SB_LINEDOWN,0),NULL);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

