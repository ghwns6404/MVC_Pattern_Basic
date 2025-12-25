#include <windows.h>
#include <commctrl.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("TreeSave");

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
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	hWndMain=hWnd;
	
	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

int TreeView_GetChildCount(HWND hTree,HTREEITEM hItem)
{
	HTREEITEM hChild;
	int Count;

	hChild=TreeView_GetChild(hTree,hItem);
	if (hChild==NULL) return 0;
	for (Count=1;;Count++) {
		hChild=TreeView_GetNextSibling(hTree,hChild);
		if (hChild==NULL) {
			return Count;
		}
	}
}

LPARAM TreeView_GetParam(HWND hTree,HTREEITEM hItem)
{
	TVITEM Tv;

	Tv.mask=TVIF_PARAM;
	Tv.hItem=hItem;
	TreeView_GetItem(hTree,&Tv);
	return Tv.lParam;
}

void TreeView_GetText(HWND hTree,HTREEITEM hItem,TCHAR *Text,int len)
{
	TVITEM Tv;

	Tv.mask=TVIF_TEXT;
	Tv.hItem=hItem;
	Tv.pszText=Text;
	Tv.cchTextMax=len;
	TreeView_GetItem(hTree,&Tv);
}

void TreeView_SetText(HWND hTree,HTREEITEM hItem,TCHAR *Text)
{
	TVITEM Tv;

	Tv.mask=TVIF_TEXT;
	Tv.hItem=hItem;
	Tv.pszText=Text;
	TreeView_SetItem(hTree,&Tv);
}

#define IDC_NAME 1
#define IDC_NUM 2
#define IDC_STR 3
#define IDC_ADD 4
#define IDC_DEL 5
#define IDM_NEW 7
#define IDM_LOAD 8
#define IDM_SAVE 9
#define MAXITEM 1000

void Add();
void View(HTREEITEM hItem);
void Update(HTREEITEM hItem);
void Del();
void NewFile();
void LoadFile();
void SaveFile();

// 파일 저장과 읽기를 위한 임시 구조체. Parent는 부모의 배열 첨자이다.
struct Node {
	int Parent;
	int ID;
	TCHAR Name[30];
	int Num;
	TCHAR *Str;
	int len;
};

struct Header {
	int Num;
};

HWND hTree;
int NextID;

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMessage) {
	case WM_CREATE:
		InitCommonControls();
		// 트리 뷰 컨트롤을 만든다.
		hTree=CreateWindow(WC_TREEVIEW, "", WS_CHILD | WS_VISIBLE | WS_BORDER |
			TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES | TVS_SHOWSELALWAYS,
			10,10,200,300,hWnd,NULL,g_hInst,NULL);

		// 정보를 입력받기 위한 컨트롤들을 만든다.
		CreateWindow("static","이름",WS_CHILD | WS_VISIBLE,
			220,70,80,25,hWnd,(HMENU)-1,g_hInst,NULL);
		CreateWindow("edit",NULL,WS_CHILD | WS_VISIBLE | WS_BORDER | 
			ES_AUTOHSCROLL,300,70,90,25,hWnd,(HMENU)IDC_NAME,g_hInst,NULL);
		CreateWindow("static","숫자",WS_CHILD | WS_VISIBLE,
			220,100,80,25,hWnd,(HMENU)-1,g_hInst,NULL);
		CreateWindow("edit",NULL,WS_CHILD | WS_VISIBLE | WS_BORDER | 
			ES_AUTOHSCROLL,300,100,90,25,hWnd,(HMENU)IDC_NUM,g_hInst,NULL);
		CreateWindow("static","문자열",WS_CHILD | WS_VISIBLE,
			220,130,80,25,hWnd,(HMENU)-1,g_hInst,NULL);
		CreateWindow("edit",NULL,WS_CHILD | WS_VISIBLE | WS_BORDER | 
			ES_AUTOHSCROLL,300,130,300,25,hWnd,(HMENU)IDC_STR,g_hInst,NULL);

		// 명령 버튼
		CreateWindow("button","추가",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			220,170,90,25,hWnd,(HMENU)IDC_ADD,g_hInst,NULL);
		CreateWindow("button","삭제",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			320,170,90,25,hWnd,(HMENU)IDC_DEL,g_hInst,NULL);
		CreateWindow("button","새파일",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			220,200,90,25,hWnd,(HMENU)IDM_NEW,g_hInst,NULL);
		CreateWindow("button","읽기",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			320,200,90,25,hWnd,(HMENU)IDM_LOAD,g_hInst,NULL);
		CreateWindow("button","저장",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			420,200,90,25,hWnd,(HMENU)IDM_SAVE,g_hInst,NULL);

		NewFile();
		return 0;
	// 버튼의 명령들을 처리한다.
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDM_NEW:
			NewFile();
			break;
		case IDM_LOAD:
			LoadFile();
			break;
		case IDM_SAVE:
			SaveFile();
			break;
		case IDC_ADD:
			Add();
			break;
		case IDC_DEL:
			Del();
			break;
		}
		return 0;
	case WM_NOTIFY:
		LPNMHDR hdr;
		LPNMTREEVIEW ntv;
		Node *tNode;
		hdr=(LPNMHDR)lParam;
		ntv=(LPNMTREEVIEW)lParam;
		if (hdr->hwndFrom == hTree) {
			switch (hdr->code) {
			case TVN_SELCHANGED:
				Update(ntv->itemOld.hItem);
				View(ntv->itemNew.hItem);
				break;
			case TVN_DELETEITEM:
				tNode=(Node *)ntv->itemOld.lParam;
				free(tNode->Str);
				free(tNode);
				break;
			}
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

// 항목추가
void Add()
{
	HTREEITEM hNow,hChild;
	TVINSERTSTRUCT TI;
	Node *tNode,*pNode;

	hNow=TreeView_GetSelection(hTree);
	if (hNow==NULL) {
		MessageBox(hWndMain,"추가할 부모 노드를 먼저 선택해 주십시오.","알림",MB_OK);
	} else {
		tNode=(Node *)calloc(sizeof(Node),1);
		GetDlgItemText(hWndMain,IDC_NAME,tNode->Name,30);
		tNode->Num=GetDlgItemInt(hWndMain,IDC_NUM,NULL,FALSE);
		tNode->len=GetWindowTextLength(GetDlgItem(hWndMain,IDC_STR));
		tNode->Str=(TCHAR *)calloc(tNode->len+1,1);
		GetDlgItemText(hWndMain,IDC_STR,tNode->Str,tNode->len+1);
		tNode->ID=++NextID;
		pNode=(Node *)TreeView_GetParam(hTree,hNow);
		tNode->Parent=pNode->ID;

		// 항목을 삽입한다.
		TI.hParent=hNow;
		TI.hInsertAfter=TVI_LAST;
		TI.item.mask=TVIF_TEXT | TVIF_PARAM;
		TI.item.pszText=tNode->Name;
		TI.item.lParam=(LPARAM)tNode;
		hChild=TreeView_InsertItem(hTree, &TI);

		// 삽입한 항목을 선택한다.
		TreeView_SelectItem(hTree,hChild);
	}
}

// 항목을 보여준다.
void View(HTREEITEM hItem)
{
	Node *tNode;

	tNode=(Node *)TreeView_GetParam(hTree,hItem);
	SetDlgItemText(hWndMain,IDC_NAME,tNode->Name);
	SetDlgItemInt(hWndMain,IDC_NUM,tNode->Num,FALSE);
	SetDlgItemText(hWndMain,IDC_STR,tNode->Str);
}

// 항목을 수정한다. 선택이 바뀔 때 이전 노드의 항목을 수정해야 한다.
void Update(HTREEITEM hItem)
{
	Node *tNode;
	int len;

	// todo : 좀 더 연구가 필요함
	if (0 && hItem) {
		tNode=(Node *)TreeView_GetParam(hTree,hItem);
		GetDlgItemText(hWndMain,IDC_NAME,tNode->Name,30);
		tNode->Num=GetDlgItemInt(hWndMain,IDC_NUM,NULL,FALSE);
		len=GetWindowTextLength(GetDlgItem(hWndMain,IDC_STR));
		if (len > tNode->len) {
			tNode->len=len;
			tNode->Str=(TCHAR *)realloc(tNode->Str,len+1);
		}
		GetDlgItemText(hWndMain,IDC_STR,tNode->Str,len+1);
		TreeView_SetText(hTree,hItem,tNode->Name);
	}
}

// 항목삭제
void Del() 
{
	HTREEITEM hNow;

	hNow=TreeView_GetSelection(hTree);
	if (hNow == TreeView_GetRoot(hTree)) {
		MessageBox(hWndMain, "루트는 삭제할 수 없습니다","알림",MB_OK);
	}
	if (hNow!=NULL) {
		TreeView_DeleteItem(hTree,hNow);
	}
}

// 새 파일 : 모두 지우고 루트만 만들어 놓는다.
void NewFile() 
{
	Node *tNode;
	TVINSERTSTRUCT TI;
	HTREEITEM hRoot;

	TreeView_DeleteAllItems(hTree);
	NextID=0;

	tNode=(Node *)calloc(sizeof(Node),1);
	lstrcpy(tNode->Name,"루트");
	tNode->Num=0;
	tNode->Str=(TCHAR *)calloc(2,1);
	tNode->ID=NextID;
	tNode->Parent=-1;
	lstrcpy(tNode->Str,"");

	TI.hParent=NULL;
	TI.hInsertAfter=TVI_LAST;
	TI.item.mask=TVIF_TEXT | TVIF_PARAM;
	TI.item.pszText=tNode->Name;
	TI.item.lParam=(LPARAM)tNode;
	hRoot=TreeView_InsertItem(hTree, &TI);
	TreeView_SelectItem(hTree,hRoot);
}

// arNode 배열에서 hItem의 자식들을 추출하여 추가한다.
void LoadNode(Node **arNode,int Num,HTREEITEM hItem)
{
	int i;
	Node *tNode;
	TVINSERTSTRUCT TI;
	HTREEITEM hChild;

	tNode=(Node *)TreeView_GetParam(hTree,hItem);
	NextID=max(NextID,tNode->ID);
	for (i=0;i<Num;i++) {
		if (arNode[i]->Parent == tNode->ID) {
			TI.hParent=hItem;
			TI.hInsertAfter=TVI_LAST;
			TI.item.mask=TVIF_TEXT | TVIF_PARAM;
			TI.item.pszText=arNode[i]->Name;
			TI.item.lParam=(LPARAM)arNode[i];
			hChild=TreeView_InsertItem(hTree, &TI);
			LoadNode(arNode,Num,hChild);
		}
	}
}

// 파일 읽기
void LoadFile() 
{
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH]="";
	HANDLE hFile;
	DWORD dwRead;
	Header H;
	Node **arNode;
	int i;
	TVINSERTSTRUCT TI;
	HTREEITEM hRoot;

	// 읽을 파일명 얻음
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner=hWndMain;
	OFN.lpstrFilter="TTTFile\0*.ttt\0Every File(*.*)\0*.*\0";
	OFN.lpstrFile=lpstrFile;
	OFN.nMaxFile=256;
	if (GetOpenFileName(&OFN)==0) {
		return;
	}

	TreeView_DeleteAllItems(hTree);
	NextID=0;

	hFile=CreateFile(lpstrFile,GENERIC_READ,0,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	ReadFile(hFile,&H,sizeof(Header),&dwRead,NULL);
	arNode=(Node **)malloc(H.Num*sizeof(Node *));
	for (i=0;i<H.Num;i++) {
		arNode[i]=(Node *)calloc(sizeof(Node),1);
		ReadFile(hFile,arNode[i],sizeof(Node),&dwRead,NULL);
		arNode[i]->Str=(TCHAR *)calloc(arNode[i]->len+1,1);
		ReadFile(hFile,arNode[i]->Str,arNode[i]->len,&dwRead,NULL);
	}
	
	TI.hParent=NULL;
	TI.hInsertAfter=TVI_LAST;
	TI.item.mask=TVIF_TEXT | TVIF_PARAM;
	TI.item.pszText=arNode[0]->Name;
	TI.item.lParam=(LPARAM)arNode[0];
	hRoot=TreeView_InsertItem(hTree, &TI);
	LoadNode(arNode,H.Num,hRoot);
	free(arNode);
	CloseHandle(hFile);
}

void SaveNode(HANDLE hFile,HTREEITEM hItem)
{
	Node *cNode;
	DWORD dwWritten;
	HTREEITEM hChild;

	for (hChild=TreeView_GetChild(hTree,hItem);hChild!=NULL;) {
		cNode=(Node *)TreeView_GetParam(hTree,hChild);
		WriteFile(hFile,cNode,sizeof(Node),&dwWritten,NULL);
		WriteFile(hFile,cNode->Str,cNode->len,&dwWritten,NULL);
		if (TreeView_GetChildCount(hTree,hItem) != 0) {
			SaveNode(hFile,hChild);
		}
		hChild=TreeView_GetNextSibling(hTree,hChild);
	}
}

// 파일 저장
void SaveFile() 
{
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH]="";
	HANDLE hFile;
	DWORD dwWritten;
	HTREEITEM hRoot;
	Header H;
	Node *tNode;

	// 저장할 파일명을 얻어온다.
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner=hWndMain;
	OFN.lpstrFilter="TTTFile\0*.ttt\0Every File(*.*)\0*.*\0";
	OFN.lpstrFile=lpstrFile;
	OFN.nMaxFile=MAX_PATH;
	OFN.lpstrDefExt="ttt";
	if (GetSaveFileName(&OFN)==0) {
		return;
	}

	hFile=CreateFile(lpstrFile,GENERIC_WRITE,0,NULL,
		CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	H.Num=TreeView_GetCount(hTree);
	WriteFile(hFile,&H,sizeof(Header),&dwWritten,NULL);

	// 루트 이하의 모든 노드를 파일로 기록한다.
	hRoot=TreeView_GetRoot(hTree);
	tNode=(Node *)TreeView_GetParam(hTree,hRoot);
	WriteFile(hFile,tNode,sizeof(Node),&dwWritten,NULL);
	WriteFile(hFile,tNode->Str,tNode->len,&dwWritten,NULL);
	SaveNode(hFile,hRoot);
	CloseHandle(hFile);
}
