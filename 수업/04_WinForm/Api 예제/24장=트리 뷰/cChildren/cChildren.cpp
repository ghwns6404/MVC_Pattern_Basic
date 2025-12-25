#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("cChildren");

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

// Parent 노드의 차일드를 모두 찾아 트리에 추가한다. 각 차일드에 대해서도 이
// 함수를 재귀적으로 호출함으로써 모든 트리를 다 입력한다.
// pNode:부모의 노드
// pid:부모의 인덱스 번호
void InsertChild(HTREEITEM pNode,int pid)
{
	TVINSERTSTRUCT TI;
	HTREEITEM Node;
	int i, j;
	TVITEM Tv;
	BOOL bHaveChild;

	for (i=0;i<sizeof(arPost)/sizeof(arPost[0]);i++) {
		if (arPost[i].iParent==pid) {
			TI.hParent=pNode;
			TI.hInsertAfter=TVI_LAST;
			TI.item.mask=TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
			TI.item.iImage=arPost[i].iImage;
			TI.item.iSelectedImage=arPost[i].iImage+4;
			TI.item.pszText=arPost[i].Title;
			TI.item.lParam=i;
			Node=TreeView_InsertItem(hTree, &TI);

			// 이 항목의 자식 항목이 존재하는지 조사한다.
			bHaveChild=FALSE;
			for (j=0;j<sizeof(arPost)/sizeof(arPost[0]);j++) {
				if (arPost[j].iParent == i) {
					bHaveChild=TRUE;
					break;
				}
			}

			// 자식 항목이 있으면 cChildren에 1을 대입하여 +버튼을 보여준다.
			if (bHaveChild == TRUE) {
				Tv.mask=TVIF_CHILDREN;
				Tv.hItem=Node;
				Tv.cChildren=1;

				TreeView_SetItem(hTree,&Tv);
			}
		}
	}
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
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
		int pid;
		TVITEM Tv;
		if (hdr->hwndFrom == hTree) {
			switch (hdr->code) {
			case TVN_ITEMEXPANDING:
				if (ntv->action==TVE_EXPAND) {
					// 자식 항목이 존재하지 않으면 실제 자식 항목을 읽어와 채워준다.
					if (TreeView_GetChild(hTree,ntv->itemNew.hItem)==NULL) {
						Tv.mask=TVIF_PARAM;
						Tv.hItem=ntv->itemNew.hItem;
						TreeView_GetItem(hTree,&Tv);
						pid=Tv.lParam;
						InsertChild(ntv->itemNew.hItem,pid);
					}
				}
				return 0;
/*			case TVN_ITEMEXPANDED:
				// 자식 항목들을 임시적으로 제거한다.
				if (ntv->action==TVE_COLLAPSE) {
					TreeView_Expand(hTree,ntv->itemNew.hItem,TVE_COLLAPSE | TVE_COLLAPSERESET);
				}
				return 0;
//*/
			}
		}
		break;
	case WM_DESTROY:
		ImageList_Destroy(Image);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
