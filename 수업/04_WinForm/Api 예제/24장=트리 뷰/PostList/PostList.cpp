#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("PostList");

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
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	TVINSERTSTRUCT TI;
	HTREEITEM Post, SiDo, GuKun;
	switch (iMessage) {
	case WM_CREATE:
		InitCommonControls();
		// 트리 뷰 컨트롤을 만든다.
		hTree=CreateWindow(WC_TREEVIEW, "", WS_CHILD | WS_VISIBLE | WS_BORDER |
			TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES,
			10,10,200,300,hWnd,NULL,g_hInst,NULL);

		// 트리뷰에 사용할 이미지 리스트를 만들어 트리뷰에 연결한다.
		Image=ImageList_LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1), 
			16, 1, RGB(255,255,255));
		SendMessage(hTree, TVM_SETIMAGELIST, (WPARAM)TVSIL_NORMAL, (LPARAM)Image);

		// 항목들을 추가한다.
		TI.hParent=0;
		TI.hInsertAfter=TVI_LAST;
		TI.item.mask=TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		TI.item.iImage=0;
		TI.item.iSelectedImage=4;
		TI.item.pszText="우편번호부";
		Post=(HTREEITEM)SendMessage(hTree, TVM_INSERTITEM, 0,(LPARAM)&TI);

		TI.hParent=Post;			// 부모 항목으로 사용한다.
		TI.item.iImage=1;
		TI.item.iSelectedImage=5;
		TI.item.pszText="서울시";
		SiDo=(HTREEITEM)SendMessage(hTree, TVM_INSERTITEM, 0,(LPARAM)&TI);

		TI.hParent=SiDo;
		TI.item.iImage=2;
		TI.item.iSelectedImage=6;
		TI.item.pszText="동대문구";
		GuKun=(HTREEITEM)SendMessage(hTree, TVM_INSERTITEM, 0,(LPARAM)&TI);

		TI.hParent=GuKun;
		TI.item.iImage=3;
		TI.item.iSelectedImage=7;
		TI.item.pszText="이문동:130-080";
		SendMessage(hTree, TVM_INSERTITEM, 0,(LPARAM)&TI);

		TI.item.pszText="휘경동:130-090";
		SendMessage(hTree, TVM_INSERTITEM, 0,(LPARAM)&TI);

		TI.item.pszText="회기동:130-050";
		SendMessage(hTree, TVM_INSERTITEM, 0,(LPARAM)&TI);

		TI.hParent=SiDo;
		TI.item.iImage=2;
		TI.item.iSelectedImage=6;
		TI.item.pszText="강남구";
		GuKun=(HTREEITEM)SendMessage(hTree, TVM_INSERTITEM, 0,(LPARAM)&TI);

		TI.hParent=GuKun;
		TI.item.iImage=3;
		TI.item.iSelectedImage=7;
		TI.item.pszText="양재동:137-130";
		SendMessage(hTree, TVM_INSERTITEM, 0,(LPARAM)&TI);

		TI.item.pszText="방배동:137-060";
		SendMessage(hTree, TVM_INSERTITEM, 0,(LPARAM)&TI);

		TI.hParent=Post;
		TI.item.iImage=1;
		TI.item.iSelectedImage=5;
		TI.item.pszText="경기도";
		SiDo=(HTREEITEM)SendMessage(hTree, TVM_INSERTITEM, 0,(LPARAM)&TI);

		TI.hParent=SiDo;
		TI.item.iImage=2;
		TI.item.iSelectedImage=6;
		TI.item.pszText="동두천시";
		GuKun=(HTREEITEM)SendMessage(hTree, TVM_INSERTITEM, 0,(LPARAM)&TI);

		TI.hParent=Post;
		TI.item.iImage=1;
		TI.item.iSelectedImage=5;
		TI.item.pszText="강원도";
		SendMessage(hTree, TVM_INSERTITEM, 0,(LPARAM)&TI);
		return 0;
	case WM_DESTROY:
		ImageList_Destroy(Image);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

