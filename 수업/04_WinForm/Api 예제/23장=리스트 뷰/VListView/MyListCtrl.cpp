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

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	LV_COLUMN COL;
	LV_ITEM LI;
	int i;
	HIMAGELIST hImgSm, hImgLa;
	static HWND hList;
	static TCHAR *Cols[6]={"이름","전화번호","주소","성별","핸드폰","생년"};
	static int ColWidth[6]={200,150,100,100,100,100};

	switch (iMessage) {
	case WM_CREATE:
		InitCommonControls();

		// 리스트 컨트롤을 자세히 보기로 만든다.
		hList=CreateWindow(WC_LISTVIEW,NULL,WS_CHILD | WS_VISIBLE | WS_BORDER |
			LVS_REPORT | LVS_SHOWSELALWAYS | LVS_OWNERDATA,
			10,10,600,300,hWnd,NULL,g_hInst,NULL);

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
		for (i=0;i<6;i++) {
			COL.cx=ColWidth[i];
			COL.pszText=Cols[i];
			ListView_InsertColumn(hList,i,&COL);
		}

		ListView_SetItemCount(hList,10000);
		return 0;
	case WM_NOTIFY:
		LPNMHDR hdr;
		NMLVDISPINFO *ndi;
		hdr=(LPNMHDR)lParam;
		ndi=(NMLVDISPINFO *)lParam;
		TCHAR tmp[300];
		if (hdr->hwndFrom == hList) {
			switch (hdr->code) {
			case LVN_GETDISPINFO:
				if (ndi->item.iSubItem == 0) {
					if ((ndi->item.mask & LVIF_TEXT) == LVIF_TEXT) {
						wsprintf(tmp, "%d번째 사람의 이름", ndi->item.iItem);
						lstrcpy(ndi->item.pszText, tmp);
					}
					if ((ndi->item.mask & LVIF_IMAGE) == LVIF_IMAGE) {
						if ((ndi->item.iItem % 2) == 0)
							ndi->item.iImage=0;
						else
							ndi->item.iImage=1;
					}
				} else {
					switch (ndi->item.iSubItem) {
					case 1:
						wsprintf(tmp, "전화번호 %d번째", ndi->item.iItem);
						break;
					case 2:
						wsprintf(tmp, "주소 %d번지", ndi->item.iItem);
						break;
					case 3:
						wsprintf(tmp, "%s", (ndi->item.iItem % 2) ? "남자":"여자");
						break;
					case 4:
						wsprintf(tmp, "핸드폰 %d번", ndi->item.iItem);
						break;
					case 5:
						wsprintf(tmp, "생년월일 %d", ndi->item.iItem);
						break;
					}
					lstrcpy(ndi->item.pszText, tmp);
				}
				return 0;
			case LVN_ODCACHEHINT:
				return 0;
			case LVN_ODFINDITEM:
				return -1;
			}
		}
		break;
	case WM_SIZE:
		MoveWindow(hList,0,0,LOWORD(lParam),HIWORD(lParam),TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

