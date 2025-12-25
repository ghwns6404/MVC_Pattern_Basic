#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("TreeDrag");

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

#include <commctrl.h>
#include "resource.h"

HWND hTree;
HIMAGELIST Image;

// 우편 번호 구조체
struct tag_Post {
	int iParent;		// 부모 노드의 배열 인덱스
	TCHAR Title[30];	// 동 이름
	TCHAR Num[10];		// 우편 번호
	int iImage;			// 아이콘 이미지 인덱스
};

// 우편 번호 구조체 배열
tag_Post arPost[]={
	{-1,"우편번호부","000-000",0},
	{0,"서울특별시","000-000",1},
	{0,"경기도","000-000",1},
	{0,"강원도","000-000",1},
	{0,"충청남도","000-000",1},
	{0,"충청북도","000-000",1},
	{1,"강남구","000-000",2},
	{1,"강동구","000-000",2},
	{1,"강북구","000-000",2},
	{1,"강서구","000-000",2},
	{1,"관악구","000-000",2},
	{1,"광진구","000-000",2},
	{1,"구로구","000-000",2},
	{1,"금천구","000-000",2},
	{1,"노원구","000-000",2},
	{1,"도봉구","000-000",2},
	{1,"동대문구","000-000",2},
	{1,"동작구","000-000",2},
	{2,"고양시","000-000",2},
	{2,"일산시","000-000",2},
	{2,"과천시","000-000",2},
	{2,"광명시","000-000",2},
	{2,"구리시","000-000",2},
	{2,"군포시","000-000",2},
	{2,"남양주시","000-000",2},
	{2,"동두천시","000-000",2},
	{2,"부천시","000-000",2},
	{3,"강릉시","000-000",2},
	{3,"삼척시","000-000",2},
	{6,"개포동","135-240",3},
	{6,"논현동","135-010",3},
	{6,"대치동","135-280",3},
	{6,"도곡동","135-270",3},
	{6,"삼성동","135-090",3},
	{6,"세곡동","135-190",3},
	{6,"수서동","135-220",3},
	{6,"신사동","135-120",3},
	{6,"압구정동","135-110",3},
	{6,"역삼동","135-080",3},
	{6,"율현동","135-210",3},
	{7,"고덕동","134-080",3},
	{8,"미아동","142-100",3},
	{9,"가양동","157-200",3},
	{10,"남현동","151-080",3},
	{10,"봉천동","151-050",3},
	{10,"신림동","151-010",3},
	{11,"광장동","143-210",3},
	{12,"가리봉동","152-020",3},
	{13,"가산동","153-023",3},
	{14,"공릉동","139-240",3},
	{15,"도봉동","132-010",3},
	{16,"답십리동","130-030",3},
	{17,"노량진동","156-050",3},
	{18,"강매동","412-290",3},
	{18,"고양동","412-500",3},
	{18,"관산동","412-470",3},
	{18,"내곡동","412-260",3},
	{18,"내유동","412-520",3},
	{18,"대자동","412-480",3},
	{18,"대장동","412-250",3},
	{18,"덕은동","412-170",3},
	{18,"도내동","412-060",3},
	{18,"동산동","412-090",3},
	{19,"가좌동","411-440",3},
	{19,"구산동","411-430",3},
	{20,"갈현동","427-100",3},
	{20,"과천동","427-060",3},
	{20,"관문동","427-020",3},
	{20,"막계동","427-080",3},
	{20,"문원동","427-090",3},
	{20,"별양동","427-040",3},
	{21,"가학동","423-070",3},
	{22,"갈매동","471-080",3},
	{23,"광정동","435-045",3},
	{24,"가운동","472-060",3},
	{25,"걸산동","483-070",3},
	{26,"계수동","422-070",3},
};

// lParam으로 대입되는 구조체
struct tag_Param {
	TCHAR Num[10];
};

// Parent 노드의 차일드를 모두 찾아 트리에 추가한다. 각 차일드에 대해서도 이
// 함수를 재귀적으로 호출함으로써 모든 트리를 다 입력한다.
// pNode:부모의 노드
// pid:부모의 인덱스 번호
void InsertChild(HTREEITEM pNode,int pid)
{
	TVINSERTSTRUCT TI;
	HTREEITEM Node;
	int i;
	tag_Param *pParam;

	for (i=0;i<sizeof(arPost)/sizeof(arPost[0]);i++) {
		if (arPost[i].iParent==pid) {
			TI.hParent=pNode;
			TI.hInsertAfter=TVI_LAST;
			TI.item.mask=TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
			TI.item.iImage=arPost[i].iImage;
			TI.item.iSelectedImage=arPost[i].iImage+4;
			TI.item.pszText=arPost[i].Title;
			// 파라미터에 구조체의 포인터를 저장한다.
			pParam=(tag_Param *)malloc(sizeof(tag_Param));
			lstrcpy(pParam->Num,arPost[i].Num);
			TI.item.lParam=(LPARAM)pParam;
			Node=TreeView_InsertItem(hTree, &TI);
			// 차일드의 차일드를 채워 넣는다.
			InsertChild(Node,i);
		}
	}
}

// 드래그 & 드롭에 사용되는 전역 변수들
HIMAGELIST himl;
BOOL g_bDrag;
HTREEITEM hDrag;
HTREEITEM hTarget, hOldTarget=NULL;

// 트리의 항목 하나를 복사하고 복사한 항목의 핸들을 리턴한다.
HTREEITEM CopyTreeItem(HWND hTree, HTREEITEM hSrc, HTREEITEM hDest)
{
	HTREEITEM hNew;
	TVINSERTSTRUCT TI;
	TVITEMEX TvEx;
	TCHAR Text[256];
	tag_Param *pParam;

	TvEx.hItem=hSrc;
	TvEx.mask=TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	TvEx.pszText=Text;
	TvEx.cchTextMax=256;
	TreeView_GetItem(hTree,&TvEx);

	TI.itemex.mask=TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	TI.itemex.pszText=Text;

	// lParam의 메모리는 재할당하여 복사해야 한다.
	pParam=(tag_Param *)malloc(sizeof(tag_Param));
	memcpy(pParam,(tag_Param *)TvEx.lParam, sizeof(tag_Param));
	TI.itemex.lParam=(LPARAM)pParam;
	TI.itemex.iImage=TvEx.iImage;
	TI.itemex.iSelectedImage=TvEx.iSelectedImage;
	TI.hParent=hDest;
	TI.hInsertAfter=TVI_LAST;
	hNew=TreeView_InsertItem(hTree,&TI);
	return hNew;
}

// 트리의 항목을 차일드까지 한꺼번에 복사한다.
HTREEITEM CopyTreeItems(HWND hTree, HTREEITEM hSrc, HTREEITEM hDest)
{
	HTREEITEM hNew, hChild;

	hNew=CopyTreeItem(hTree, hSrc, hDest);

	hChild=TreeView_GetChild(hTree, hSrc);
	while (hChild != NULL) {
		CopyTreeItems(hTree,hChild, hNew);
		hChild=TreeView_GetNextSibling(hTree,hChild);
	}

	return hNew;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	POINT pt;
	int hotx,hoty;
	RECT rt;
	HTREEITEM hItem;
	TCHAR str[255];
	tag_Param *pParam;
	
	switch (iMessage) {
	case WM_CREATE:
		InitCommonControls();
		// 트리 뷰 컨트롤을 만든다.
		hTree=CreateWindow(WC_TREEVIEW, "", WS_CHILD | WS_VISIBLE | WS_BORDER |
			TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES | TVS_SHOWSELALWAYS,
			10,10,200,300,hWnd,NULL,g_hInst,NULL);

		// 트리뷰에 사용할 이미지 리스트를 만들어 트리뷰에 연결한다.
		Image=ImageList_LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1), 
			16, 1, RGB(255,255,255));
		TreeView_SetImageList(hTree, Image, TVSIL_NORMAL);

		// 트리에 데이터를 입력한다.
		InsertChild((HTREEITEM)0,-1);
		return 0;
	case WM_NOTIFY:
		LPNMHDR hdr;
		LPNMTREEVIEW ntv;
		hdr=(LPNMHDR)lParam;
		ntv=(LPNMTREEVIEW)lParam;
		if (hdr->hwndFrom == hTree) {
			switch (hdr->code) {
			// 항목의 데이터를 삭제한다.
			case TVN_DELETEITEM:
				pParam=(tag_Param *)ntv->itemOld.lParam;
				free(pParam);
				return 0;
			case TVN_BEGINDRAG:
				// 드래그 이미지를 만든다.
				hDrag=ntv->itemNew.hItem;
				himl=TreeView_CreateDragImage(hTree,hDrag);

				// 핫 스팟 계산. GetItemRect는 이미지 폭을 빼고 계산해 주므로 18만큼 이미지와
				// 그 여백을 계산에 포함시켜야 한다.
				TreeView_GetItemRect(hTree,hDrag,&rt,TRUE);
				hotx=ntv->ptDrag.x-(rt.left-18);
				hoty=ntv->ptDrag.y-rt.top;
				ImageList_BeginDrag(himl,0,hotx,hoty);
				wsprintf(str, "%d,%d,%d,%d\r\n",ntv->ptDrag.x,ntv->ptDrag.y,rt.left,rt.top);
				OutputDebugString(str);

				// 드래그 이미지 그림
				pt.x=ntv->ptDrag.x;
				pt.y=ntv->ptDrag.y;
				ClientToScreen(hWnd,&pt);
				ImageList_DragEnter(NULL,pt.x,pt.y);

				// 커서 캡처
				SetCapture(hWnd);
				g_bDrag=TRUE;
				return 0;
			}
		}
		break;
	case WM_MOUSEMOVE:
		TVHITTESTINFO hti;
		
		if (g_bDrag) {
			// 드래그중인 이미지 그림
			pt.x=LOWORD(lParam);
			pt.y=HIWORD(lParam);
			ClientToScreen(hWnd,&pt);
			ImageList_DragMove(pt.x,pt.y);

			GetWindowRect(hTree,&rt);
			ScreenToClient(hWnd,(POINT *)&rt);
			hti.pt.x=LOWORD(lParam)-rt.left;
			hti.pt.y=HIWORD(lParam)-rt.top;
			hTarget=TreeView_HitTest(hTree,&hti);
			if (hTarget != NULL) {
				ImageList_DragLeave(NULL);
				TreeView_SelectDropTarget(hTree,hTarget);
				UpdateWindow(hTree);
				ImageList_DragEnter(NULL,pt.x,pt.y);

				// 드롭 타겟이 바뀌면 다시 1초동안 기다린다.
				if (hTarget != hOldTarget) {
					hOldTarget=hTarget;
					KillTimer(hWnd,1);
					SetTimer(hWnd,1,1000,NULL);
				}
			}

			// 커서가 리스트의 위에 있으면 위로 스크롤. 마우스 위치나 화면 위치 
			// 둘 다 화면 좌표로 바꾼후 비교해야 한다. 커서 값이 음수가 될 수 
			// 있으므로 (short) 캐스팅도 생략할 수 없다.
			GetWindowRect(hTree,&rt);
			if ((short)pt.y < rt.top) {
				SetTimer(hWnd,2,200,NULL);
				ImageList_DragLeave(NULL);
				SendMessage(hTree,WM_VSCROLL,MAKEWPARAM(SB_LINEUP,0),NULL);
				ImageList_DragEnter(NULL,pt.x,pt.y);
			} else {
				KillTimer(hWnd,2);
			}

			// 커서가 리스트의 아래에 있으면 아래로 스크롤
			if ((short)pt.y > rt.bottom) {
				SetTimer(hWnd,3,200,NULL);
				ImageList_DragLeave(NULL);
				SendMessage(hTree,WM_VSCROLL,MAKEWPARAM(SB_LINEDOWN,0),NULL);
				ImageList_DragEnter(NULL,pt.x,pt.y);
			} else {
				KillTimer(hWnd,3);
			}
		}
		return 0;
	case WM_LBUTTONUP:
		if (g_bDrag) {
			// 스크롤 타이머 파괴
			KillTimer(hWnd,1);
			KillTimer(hWnd,2);
			KillTimer(hWnd,3);

			ImageList_DragLeave(NULL);
			ImageList_EndDrag();
			TreeView_SelectDropTarget(hTree,NULL);
			ReleaseCapture();
			g_bDrag=FALSE;
			ImageList_Destroy(himl);

			// 드래그된 항목과 드롭된 항목이 같으면 리턴
			if (hDrag == hTarget)
				return 0;

			// 드롭된 항목이 드래그된 항목의 자식 항목이면 리턴
			hItem=hTarget;
			while (hItem != NULL) {
				hItem=TreeView_GetParent(hTree,hItem);
				if (hItem == hDrag)
					return 0;
			}

			// 드래그된 항목을 드롭된 항목 아래로 옮기고 타겟을 확장한다.
			CopyTreeItems(hTree, hDrag, hTarget);
			TreeView_Expand(hTree,hTarget,TVE_EXPAND);
			TreeView_DeleteItem(hTree,hDrag);
		}
		return 0;
	case WM_TIMER:
		if (wParam == 1) {
			// 드롭 타겟 위에서 1초간 머무르면 타겟을 확장한다.
			hItem=TreeView_GetDropHilight(hTree);
			if ((hItem != NULL) && (hItem == hTarget)) {
				ImageList_DragShowNolock(FALSE);
				TreeView_Expand(hTree,hTarget,TVE_EXPAND);
				UpdateWindow(hTree);
				ImageList_DragShowNolock(TRUE);
			}
		}
		if (wParam == 2) {
			SendMessage(hTree,WM_VSCROLL,MAKEWPARAM(SB_LINEUP,0),NULL);
		}
		if (wParam == 3) {
			SendMessage(hTree,WM_VSCROLL,MAKEWPARAM(SB_LINEDOWN,0),NULL);
		}
		return 0;
	case WM_DESTROY:
		ImageList_Destroy(Image);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
