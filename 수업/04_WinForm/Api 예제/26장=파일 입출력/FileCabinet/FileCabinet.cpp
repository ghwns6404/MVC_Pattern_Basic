// FileCabinet Project
// 2005년 4월. 김상형

// 2005년 6월 26일 FindFileRecursive 두 군데 고침.
// 1.드라이브의 루트 아래의 디렉토리 경로 조립할 때 \ 하나만
// 2.휴지통, System Volume Information은 조사하지 않기

#include <windows.h>
#include <commctrl.h>
#include <malloc.h>
#include <stdio.h>
#include <io.h>
#include <stddef.h>
#include <assert.h>
#include "resource.h"
#include "Util.h"
#include "ShReg.h"

#define GAP 3				// 차일드간의 간격
#define VERSION 101			// 파일 버전
#define NAMELIM 24			// 동적 할당하지 않을 파일명의 길이 한계
#define KEY "Software\\MiyoungSoft\\FileCabinet\\"

// 툴 버튼의 ID
#define TM_NEW				41000
#define TM_OPEN				41001
#define TM_SAVE				41002
#define TM_CLOSE			41003
#define TM_ADDVOL			41004
#define TM_DELVOL			41005

// 파일의 속성
#define FA_READONLY         0x00000001
#define FA_HIDDEN           0x00000002
#define FA_SYSTEM           0x00000004
#define FA_DIRECTORY        0x00000010
#define FA_ARCHIVE          0x00000020
#define FA_ZIPDIR			0x00010000		// 압축 파일 디렉토리
#define FA_REMARK			0x00020000		// 대체된 설명

// 미리 보기 포맷의 종류
enum tag_Preview { PV_NONE, PV_TEXT, PV_IMAGE, PV_SOUND };

// 레코드 - 파일 또는 디렉토리. 용량을 최대한 줄일 수 있도록 설계했다.
struct SRecord {
	int Id;						// 일련번호.생성시에는 첨자와 같으나 편집되면 달라질 수도 있다.
	int Parent;					// 부모의 일련번호
	__int64 Size;				// 크기
	WORD nFile,nDir;			// 파일 개수, 디렉토리 개수.
	DWORD Attr;					// 속성
	DWORD ModiTime;				// 최후 수정한 시간
//	DWORD CreateTime;			// 생성 시간. 절대초 기준
//	DWORD AccTime;				// 최후 액세스한 시간
	tag_Preview PvFormat;		// 미리보기의 포맷

	// 이하는 가변 길이 데이터이며 꼭 필요한만큼만 파일로 저장한다.
	union {
		TCHAR Name[NAMELIM];	// 파일명. NAMELIM자까지는 할당하지 않고 그냥 저장
		struct {
			TCHAR IsPtr[4];		// NAMELIM자 이상은 IsPtr에 ":::"을 넣고 pName에 파일 길이만큼 동적 할당
			TCHAR *pName;		// 동적 할당된 포인터
		} stName;
	};
	// 미리 보기 관련 멤버 - PvFormat이 PV_NONE 아닐 때만 유효하다.
	DWORD PvLen;				// 미리보기 데이터의 크기
	BYTE *PvPtr;				// 미리보기 데이터	
};

// 볼륨 - 한번 조사하는 단위
struct SVolume {
	TCHAR Path[MAX_PATH];		// 볼륨의 원래 경로
	TCHAR Label[64];			// 디스크의 라벨 명
	DWORD Serial;				// 볼륨 일련 번호
	TCHAR FilsSystem[32];		// 파일 시스템 포맷. 포맷에 따라 날짜 형식이 다름.
	TCHAR Name[64];				// 트리의 노드에 보일 제목
	TCHAR Desc[64];				// 간단한 설명
	DWORD Time;					// 빌드한 날짜
	BOOL bIncHid;				// 숨은 파일 포함 여부
	BOOL bZipDir;				// 압축 파일 조사 여부
	BOOL bMakePreview;			// 미리 보기 생성 여부
	SRecord *arRecord;			// 레코드의 배열
	int nRecord;				// 레코드 배열상의 항목 개수
	DWORD Reserved1;
	DWORD Reserved2;
	TCHAR Reserved3[64];
};

// 캐비넷 파일. 트리의 노드 하나에 저장된다. 미구현 기능까지 일단 포함시킴
struct SCabinet {
	TCHAR Magic[32];			// 매직 넘버
	int Ver;					// 버전.
	TCHAR Name[64];				// 트리의 노드에 보일 제목
	TCHAR Desc[64];				// 설명
	TCHAR Path[MAX_PATH];		// 캐비넷 파일의 경로
	int nVolume;				// 볼륨 배열상의 항목 개수. 저장하기 전에 자식 노드 개수를 구한다.
	int Compress;				// 압축 방법. 0:압축 안함. 1:ZIP
	int Encrypt;				// 암호화 여부. 0:암호화하지 않음. 1:암호화
	TCHAR Password[32];			// 암호. 간단한 암호화에만 쓸 수 있다.
	BOOL bModified;				// 수정 여부
	DWORD Reserved1;
	DWORD Reserved2;
	TCHAR Reserved3[64];
};

// 전역 변수
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("FileCabinet");
HWND hList,hTree;
HWND hState,hToolBar;
HWND hSrch,hSrchList,hSrchCombo,hSrchEdit,hSrchBtn;
HIMAGELIST TreeImage,ListImage;
int TreeWidth, SrchHeight;
SVolume *pTargetVol;
int TotalFind;
HFONT hGulim;
WNDPROC OldEditProc;
int iDrag;
enum tag_SizeType { ST_BYTE, ST_AKMG, ST_ApBKMG } SizeType;

// 함수 원형
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK SrchProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam);
LRESULT OnCreate(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnDestroy(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnPaint(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnCommand(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnNotify(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnSize(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnSetCursor(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnLButtonDown(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnMouseMove(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnLButtonUp(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnDropFiles(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT Srch_OnCreate(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT Srch_OnDestroy(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT Srch_OnPaint(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT Srch_OnCommand(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT Srch_OnNotify(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT Srch_OnSize(HWND hWnd,WPARAM wParam,LPARAM lParam);

void NewCabinet(BOOL bDefault=FALSE);
void AddVolume(HTREEITEM hCabItem);
BOOL CALLBACK AddVolumeProc(HWND,UINT,WPARAM,LPARAM);
void InsertChild(SVolume *pVol, HTREEITEM ParentNode, int pid);
void ListFile(SVolume *pVol,int pid);
HTREEITEM GetCabinetNode(HTREEITEM Node);
HTREEITEM FindVolNode(HTREEITEM Node);
SVolume *FindVolume(HTREEITEM Node);
BOOL CALLBACK NewCabinetProc(HWND,UINT,WPARAM,LPARAM);
void OnTreePopup();
void EditCabinet(HTREEITEM hCabItem);
HTREEITEM MakeVolume(SVolume *pVolInfo,HTREEITEM hParent,HTREEITEM hAfter);
void RemakeVolume(HTREEITEM hVolItem);
BOOL FindFileRecursive(TCHAR *Path);
int AddFileToVolume(TCHAR *Path,DWORD Attr,int pid,SVolume *pVol);
int FindInsertIdx(LVITEM LI, SRecord *pRec);
int ConfirmSave(HTREEITEM hCabItem);
BOOL Save(HTREEITEM hCabItem);
BOOL SaveAs(HTREEITEM hCabItem);
BOOL SaveCabinet(HTREEITEM hCabItem,TCHAR *Path);
BOOL Open();
BOOL OpenCabinet(TCHAR *Path);
LRESULT CALLBACK SrchEditSubProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam);
void StartSearch();
BOOL SearchCabinet(HTREEITEM hCabNode);
BOOL SearchVolume(HTREEITEM hVolNode,int parent=-1);
void GetRecordFullPath(SVolume *pVol,int Id,TCHAR *FullPath);
void GotoItemInTree(HTREEITEM hVolNode,TCHAR *FullPath);
void BuildDirectoryInfo(SVolume *pVol);
void CreateToolBar();
void DeleteRecord(HTREEITEM hRecNode);
void DeleteDirNode(SVolume *pVol,int id);
BOOL CALLBACK RemarkProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);
int GetIndexFromId(SVolume *pVol, int Id);
void FreeVolume(SVolume *pVol);
BOOL CALLBACK PasswordProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);
BOOL CALLBACK AboutProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);

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
	WndClass.hIcon=LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style=0;
	RegisterClass(&WndClass);

	WndClass.hIcon=NULL;
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=SrchProc;
	WndClass.lpszClassName="SrchWnd";
	WndClass.lpszMenuName=NULL;
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,"FileCabinet Ver 1.0",WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
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
	switch (iMessage) {
	case WM_CREATE:
		return OnCreate(hWnd,wParam,lParam);
	case WM_DESTROY:
		return OnDestroy(hWnd,wParam,lParam);
	case WM_PAINT:
		return OnPaint(hWnd,wParam,lParam);
	case WM_COMMAND:
		return OnCommand(hWnd,wParam,lParam);
	case WM_NOTIFY:
		return OnNotify(hWnd,wParam,lParam);
	case WM_SIZE:
		return OnSize(hWnd,wParam,lParam);
	case WM_SETCURSOR:
		return OnSetCursor(hWnd,wParam,lParam);
	case WM_LBUTTONDOWN:
		return OnLButtonDown(hWnd,wParam,lParam);
	case WM_MOUSEMOVE:
		return OnMouseMove(hWnd,wParam,lParam);
	case WM_LBUTTONUP:
		return OnLButtonUp(hWnd,wParam,lParam);
	case WM_DROPFILES:
		return OnDropFiles(hWnd,wParam,lParam);
#ifndef _DEBUG
	case WM_QUERYENDSESSION:
	case WM_CLOSE:
		HTREEITEM hRoot;
		for (hRoot=TreeView_GetRoot(hTree);hRoot;) {
			if (ConfirmSave(hRoot) == IDCANCEL) return TRUE;
			hRoot=TreeView_GetNextSibling(hTree,hRoot);
		}
		break;
#endif
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

LRESULT CALLBACK SrchProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage) {
	case WM_CREATE:
		return Srch_OnCreate(hWnd,wParam,lParam);
	case WM_DESTROY:
		return Srch_OnDestroy(hWnd,wParam,lParam);
	case WM_PAINT:
		return Srch_OnPaint(hWnd,wParam,lParam);
	case WM_COMMAND:
		return Srch_OnCommand(hWnd,wParam,lParam);
	case WM_NOTIFY:
		return Srch_OnNotify(hWnd,wParam,lParam);
	case WM_SIZE:
		return Srch_OnSize(hWnd,wParam,lParam);
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

LRESULT OnCreate(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	LVCOLUMN COL;
	int Num,i;
	TCHAR szValue[16],Path[MAX_PATH];
	SHFILEINFO si;

	hWndMain=hWnd;
	InitCommonControls();
	hGulim=CreateFont(12,0,0,0,0,0,0,0,HANGEUL_CHARSET,3,2,1,
		VARIABLE_PITCH | FF_MODERN,"굴림");

	hTree=CreateWindowEx(WS_EX_CLIENTEDGE,WC_TREEVIEW, "", WS_CHILD | WS_VISIBLE | WS_BORDER |
		TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES | TVS_SHOWSELALWAYS,
		0,0,0,0,hWnd,NULL,g_hInst,NULL);
	hList=CreateWindowEx(WS_EX_CLIENTEDGE,WC_LISTVIEW,"",WS_CHILD | WS_VISIBLE | WS_BORDER |
		LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL ,
		0,0,0,0,hWnd,NULL,g_hInst,NULL);
	ListView_SetExtendedListViewStyle(hList, LVS_EX_FULLROWSELECT);
	TreeImage=ImageList_LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_TREEIMAGE),
		16,1,RGB(255,255,255));
	TreeView_SetImageList(hTree, TreeImage, TVSIL_NORMAL);
//	ListImage=ImageList_LoadBitmap(g_hInst,MAKEINTRESOURCE(IDB_TREEIMAGE), 
//		16,1,RGB(255,255,255));
	// 시스템 이미지 리스트 사용
	ListImage=(HIMAGELIST)SHGetFileInfo(NULL,0,&si,sizeof(si),
		SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
	ListView_SetImageList(hList,ListImage,LVSIL_SMALL);

	COL.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	COL.fmt=LVCFMT_LEFT;
	COL.cx=250;
	COL.pszText="이름";
	COL.iSubItem=0;
	ListView_InsertColumn(hList,0,&COL);

	COL.pszText="크기";
	COL.fmt=LVCFMT_RIGHT;
	COL.cx=80;
	COL.iSubItem=1;
	ListView_InsertColumn(hList,1,&COL);

	COL.pszText="날짜";
	COL.fmt=LVCFMT_LEFT;
	COL.cx=150;
	COL.iSubItem=2;
	ListView_InsertColumn(hList,2,&COL);

	COL.pszText="속성";
	COL.cx=80;
	COL.iSubItem=3;
	ListView_InsertColumn(hList,3,&COL);

	CreateToolBar();
	hState=CreateStatusWindow(WS_CHILD | WS_VISIBLE, "", hWnd,0);
	hSrch=CreateWindowEx(WS_EX_CLIENTEDGE,"SrchWnd",NULL,WS_CHILD | WS_VISIBLE,
		0,0,0,0,hWnd,(HMENU)NULL,g_hInst,NULL);
	LoadPosition(hWnd,KEY"Position",NULL);

	TreeWidth=SHRegReadInt(SHCU,KEY"Option","TreeWidth",250);
	SrchHeight=SHRegReadInt(SHCU,KEY"Option","SrchHeight",200);
	SizeType=ST_ApBKMG;

	MakeAssociate(".fcf","FileCabinet File");
	DragAcceptFiles(hWnd,TRUE);

	// 인수가 전달되었을 경우 전달된 파일만 연다.
	if (__argc > 1) {
		OpenCabinet(__argv[1]);
	// 마지막 열어본 캐비넷을 읽어온다.
	} else {
		Num=SHRegReadInt(SHCU,KEY"MRU","Num",0);
		if (Num) {
			for (i=0;i<Num;i++) {
				wsprintf(szValue,"%d",i);
				SHRegReadString(SHCU,KEY"MRU",szValue,"",Path,MAX_PATH);
				OpenCabinet(Path);
			}
		}
	}
	// 첫번째 캐비넷을 선택해 준다.
	if (TreeView_GetRoot(hTree) != NULL) {
		TreeView_SelectItem(hTree,TreeView_GetRoot(hTree));
	}
	return 0;
}

LRESULT OnDestroy(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	HTREEITEM hRoot;
	int Num;
	TCHAR szValue[16];
	SCabinet *pCab;

	// 열어 놓은 캐비넷의 목록을 저장한다.
	for (hRoot=TreeView_GetRoot(hTree),Num=0;hRoot;Num++) {
		pCab=(SCabinet *)TreeView_GetParam(hTree,hRoot);
		wsprintf(szValue,"%d",Num);
		SHRegWriteString(SHCU,KEY"MRU",szValue,pCab->Path);
		hRoot=TreeView_GetNextSibling(hTree,hRoot);
	}
	SHRegWriteInt(SHCU,KEY"MRU","Num",Num);
	SHRegWriteInt(SHCU,KEY"Option","TreeWidth",TreeWidth);
	SHRegWriteInt(SHCU,KEY"Option","SrchHeight",SrchHeight);

	DeleteObject(hGulim);
	SavePosition(hWnd,KEY"Position");
	ImageList_Destroy(TreeImage);
	PostQuitMessage(0);
	return 0;
}

LRESULT OnPaint(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	hdc=BeginPaint(hWnd, &ps);
	EndPaint(hWnd, &ps);
	return 0;
}

LRESULT OnCommand(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	HTREEITEM hSel,hCabNode=NULL;
	SCabinet *pCab;

	hSel=TreeView_GetSelection(hTree);
	if (hSel) {
		hCabNode=GetCabinetNode(hSel);
	}

	switch (LOWORD(wParam)) {
	case TM_NEW:
		NewCabinet();
		break;
	case TM_OPEN:
		Open();
		break;
	case TM_SAVE:
		if (hCabNode) {
			Save(hCabNode);
		}
		break;
	case TM_CLOSE:
		if (hCabNode) {
			if (ConfirmSave(hCabNode)==IDCANCEL) {
				break;
			}
			TreeView_DeleteItem(hTree,hCabNode);
			ListView_DeleteAllItems(hList);
		} else {
			MessageBox(hWndMain,"캐비넷이 선택되어 있어야 닫을 수 있다.","어머나 ^ ^",MB_OK);
		}
		break;
	case TM_ADDVOL:
		if (hCabNode) {
			AddVolume(hCabNode);
		} else {
			MessageBox(hWndMain,"어떤 캐비넷에다 볼륨을 추가할 지 먼저 선택해야지~ 그렇지?","헷갈려..",MB_OK);
		}
		break;
	case TM_DELVOL:
		if (hCabNode && TreeView_GetDepth(hTree,hSel) == 1) {
			pCab=(SCabinet *)TreeView_GetParam(hTree,hCabNode);
			pCab->bModified=TRUE;
			TreeView_DeleteItem(hTree,hSel);
		} else {
			MessageBox(hWnd,"삭제하고 싶은 볼륨을 정확하게 선택해. 캐비넷이나 폴더는 안된다.","못해",MB_OK);
		}
		break;
	case IDM_FILE_EXIT:
		SendMessage(hWnd,WM_CLOSE,0,0);
		break;
	case IDM_HELP_ABOUT:
		DialogBox(g_hInst,MAKEINTRESOURCE(IDD_DLGABOUT),hWndMain,AboutProc);
		break;
	}
	return 0;
}

LRESULT OnNotify(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	LPNMHDR hdr;
	LPNMTREEVIEW ntv;
	LPNMITEMACTIVATE nia;
	LPNMLISTVIEW nlv;
	hdr=(LPNMHDR)lParam;
	ntv=(LPNMTREEVIEW)lParam;
	HTREEITEM hSel,hVolNode;
	SCabinet *pCab;
	SVolume *pVol;
	SRecord *pRec;
	int idx,Id;
	int Depth;
	TCHAR szStatus[512];
	SYSTEMTIME st;
	TCHAR FullPath[MAX_PATH];

	if (hdr->hwndFrom == hTree) {
		switch (hdr->code) {
		case TVN_DELETEITEM:
			Depth=TreeView_GetDepth(hTree,ntv->itemOld.hItem);
			if (Depth == 0) {
				free((SCabinet *)TreeView_GetParam(hTree,ntv->itemOld.hItem));
			}
			// 볼륨인 경우 레코드를 순회하며 레코드에 할당된 메모리도 해제
			if (Depth == 1) {
				pVol=(SVolume *)TreeView_GetParam(hTree,ntv->itemOld.hItem);
				FreeVolume(pVol);
			}
			break;
		case TVN_ITEMEXPANDING:
			if (ntv->action==TVE_EXPAND) {
				pVol=FindVolume(ntv->itemNew.hItem);
				// 자식 항목이 존재하지 않으면 실제 자식 항목을 읽어와 채워준다.
				if (TreeView_GetChild(hTree,ntv->itemNew.hItem)==NULL) {
					// 현재 노드의 자식들을 삽입한다.
					if (TreeView_GetDepth(hTree,ntv->itemNew.hItem) == 1) {
						InsertChild(pVol,ntv->itemNew.hItem,0);
					} else {
						InsertChild(pVol,ntv->itemNew.hItem,TreeView_GetParam(hTree,ntv->itemNew.hItem));
					}
				}
			}
			break;
		case TVN_SELCHANGED:
			pVol=FindVolume(ntv->itemNew.hItem);
			// 볼륨보더 더 위 = 캐비넷인 경우
			if (pVol == NULL) {
				ListView_DeleteAllItems(hList);
				pCab=(SCabinet *)TreeView_GetParam(hTree,ntv->itemNew.hItem);
				wsprintf(szStatus,"파일:%s, 설명:%s, %s",pCab->Path,pCab->Desc,
					(pCab->bModified ? "수정되었음":"저장되었음"));
				SendMessage(hState, SB_SETTEXT, 0, (LPARAM)szStatus);
			} else {
				// 볼륨인 경우 볼륨 포인터가 아닌 ID 0을 전달해야 한다.
				if (TreeView_GetParam(hTree,ntv->itemNew.hItem) == (LPARAM)pVol) {
					ListFile(pVol,0);
				} else {
					ListFile(pVol,TreeView_GetParam(hTree,ntv->itemNew.hItem));
				}
				MyAbsSecondToSystem(pVol->Time,st);
				wsprintf(szStatus,"원래 경로:%s, 파일 개수:%d, 날짜:%d/%d/%d, 설명:%s",
					pVol->Path,pVol->nRecord,st.wYear,st.wMonth,st.wDay,pVol->Desc);
				SendMessage(hState, SB_SETTEXT, 0, (LPARAM)szStatus);
			}
			break;
		case NM_RCLICK:
			OnTreePopup();
			break;
		}
	}
	if (hdr->hwndFrom == hList) {
		switch (hdr->code) {
		case NM_DBLCLK:
			switch (hdr->code) {
			case NM_DBLCLK:
				nia=(LPNMITEMACTIVATE)lParam;
				if (nia->iItem==-1 || ListView_GetItemImage(hList,nia->iItem,0)!=4) {
					return 0;
				}
				hSel=TreeView_GetSelection(hTree);
				hVolNode=FindVolNode(hSel);
				pVol=(SVolume *)TreeView_GetParam(hTree,hVolNode);
				Id=ListView_GetParam(hList,nia->iItem);
				GetRecordFullPath(pVol,Id,FullPath);
				GotoItemInTree(hVolNode,FullPath);
				break;
			}
		case LVN_ITEMCHANGED:
			nlv=(LPNMLISTVIEW)lParam;
			if (nlv->uChanged == LVIF_STATE && nlv->uNewState == (LVIS_SELECTED | LVIS_FOCUSED)) {
				hSel=TreeView_GetSelection(hTree);
				hVolNode=FindVolNode(hSel);
				pVol=(SVolume *)TreeView_GetParam(hTree,hVolNode);
				idx=GetIndexFromId(pVol,ListView_GetParam(hList,nlv->iItem));
				pRec=&pVol->arRecord[idx];

				wsprintf(szStatus,"크기:%I64d바이트",pRec->Size);
				if (pRec->PvFormat == PV_TEXT) {
					lstrcat(szStatus,",미리보기 텍스트=");
					lstrcat(szStatus,(TCHAR *)pRec->PvPtr);
				}

				SendMessage(hState, SB_SETTEXT, 0, (LPARAM)szStatus);
			}
			break;
		}
	}
	return 0;
}

LRESULT OnSize(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	RECT crt,srt;
	int StateHeight;
	int ToolHeight;

	if (wParam != SIZE_MINIMIZED) {
		SendMessage(hState, WM_SIZE, wParam, lParam);
		GetWindowRect(hState,&srt);
		StateHeight=srt.bottom-srt.top;
		GetWindowRect(hToolBar,&srt);
		ToolHeight=srt.bottom-srt.top;

		GetClientRect(hWnd,&crt);
		MoveWindow(hTree,0,ToolHeight,TreeWidth-GAP,crt.bottom-StateHeight-ToolHeight-SrchHeight,TRUE);
		MoveWindow(hList,TreeWidth,ToolHeight,crt.right-TreeWidth,crt.bottom-StateHeight-ToolHeight-SrchHeight,TRUE);
		MoveWindow(hSrch,0,crt.bottom-StateHeight-SrchHeight+GAP,crt.right,SrchHeight-GAP,TRUE);
	}
	return 0;
}

LRESULT OnSetCursor(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	POINT pt;
	RECT crt,srt;
	int StateHeight;

	GetCursorPos(&pt);
	ScreenToClient(hWnd,&pt);
	GetClientRect(hWnd,&crt);
	GetWindowRect(hState,&srt);
	StateHeight=srt.bottom-srt.top;

	if (LOWORD(lParam)==HTCLIENT) {
		if (pt.x >= TreeWidth-GAP && pt.x <= TreeWidth && pt.y < crt.bottom-StateHeight-SrchHeight) {
			SetCursor(LoadCursor(NULL,IDC_SIZEWE));
		}
		if (pt.y >= crt.bottom-StateHeight-SrchHeight && pt.y <= crt.bottom-StateHeight-SrchHeight+GAP) {
			SetCursor(LoadCursor(NULL,IDC_SIZENS));
		}
		return 0;
	} else {
		return(DefWindowProc(hWnd,WM_SETCURSOR,wParam,lParam));
	}
}

LRESULT OnLButtonDown(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	POINT pt;
	RECT crt,srt;
	int StateHeight;

	pt.x=LOWORD(lParam);
	pt.y=HIWORD(lParam);
	GetClientRect(hWnd,&crt);
	GetWindowRect(hState,&srt);
	StateHeight=srt.bottom-srt.top;

	iDrag=0;
	if (pt.x >= TreeWidth-GAP && pt.x <= TreeWidth && pt.y < crt.bottom-StateHeight-SrchHeight) {
		iDrag=1;
	}
	if (pt.y >= crt.bottom-StateHeight-SrchHeight && pt.y <= crt.bottom-StateHeight-SrchHeight+GAP) {
		iDrag=2;
	}
	if (iDrag) {
		SetCapture(hWnd);
	}
	return 0;
}

LRESULT OnMouseMove(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	RECT crt,srt;
	int StateHeight;
	if (iDrag==0) return 0;

	GetClientRect(hWnd,&crt);
	GetWindowRect(hState,&srt);
	StateHeight=srt.bottom-srt.top;
	if (iDrag==1) {
		TreeWidth=(int)(short)LOWORD(lParam);
		TreeWidth=max(100,min(TreeWidth,600));
		SendMessage(hWnd,WM_SIZE,0,0);
	}
	if (iDrag==2) {
		SrchHeight=crt.bottom-(int)(short)HIWORD(lParam)-StateHeight;
		SrchHeight=max(100,min(SrchHeight,600));
		SendMessage(hWnd,WM_SIZE,0,0);
	}
	return 0;
}

LRESULT OnLButtonUp(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	iDrag=0;
	ReleaseCapture();
	return 0;
}

LRESULT OnDropFiles(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	int nDrop,i;
	TCHAR Path[MAX_PATH];

	nDrop=DragQueryFile((HDROP)wParam,-1,NULL,0);
	for (i=0;i<nDrop;i++) {
		DragQueryFile((HDROP)wParam,i,Path,MAX_PATH);
		OpenCabinet(Path);
	}
	return 0;
}

LRESULT Srch_OnCreate(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	LVCOLUMN COL;

	hSrchCombo=CreateWindow("combobox",NULL,WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
		100,5,150,200,hWnd,(HMENU)0,g_hInst,NULL);
	SendMessage(hSrchCombo,CB_ADDSTRING,0,(LPARAM)"열려진 모든 캐비넷");
	SendMessage(hSrchCombo,CB_ADDSTRING,0,(LPARAM)"선택된 캐비넷");
	SendMessage(hSrchCombo,CB_ADDSTRING,0,(LPARAM)"선택된 볼륨");
	SendMessage(hSrchCombo,CB_ADDSTRING,0,(LPARAM)"선택된 폴더");
	SendMessage(hSrchCombo,CB_SETCURSEL,1,0);
	hSrchEdit=CreateWindowEx(WS_EX_CLIENTEDGE,"edit",NULL,WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
		350,5,150,22,hWnd,(HMENU)1,g_hInst,NULL);
	OldEditProc=(WNDPROC)SetWindowLong(hSrchEdit,GWL_WNDPROC,(LONG)SrchEditSubProc);
	hSrchBtn=CreateWindow("button","검색 시작",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		510,5,80,22,hWnd,(HMENU)2,g_hInst,NULL);
	hSrchList=CreateWindowEx(0,WC_LISTVIEW,"",WS_CHILD | WS_VISIBLE | WS_BORDER |
		LVS_REPORT | LVS_SHOWSELALWAYS,0,0,0,0,hWnd,NULL,g_hInst,NULL);
	ListView_SetExtendedListViewStyle(hSrchList, LVS_EX_FULLROWSELECT);
	SendMessage(hSrchCombo,WM_SETFONT,(WPARAM)hGulim,FALSE);
	SendMessage(hSrchEdit,WM_SETFONT,(WPARAM)hGulim,FALSE);
	SendMessage(hSrchBtn,WM_SETFONT,(WPARAM)hGulim,FALSE);

	COL.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	COL.fmt=LVCFMT_LEFT;
	COL.cx=100;
	COL.pszText="캐비넷";
	COL.iSubItem=0;
	ListView_InsertColumn(hSrchList,0,&COL);

	COL.pszText="볼륨";
	COL.fmt=LVCFMT_LEFT;
	COL.cx=140;
	COL.iSubItem=1;
	ListView_InsertColumn(hSrchList,1,&COL);

	COL.pszText="경로";
	COL.fmt=LVCFMT_LEFT;
	COL.cx=400;
	COL.iSubItem=2;
	ListView_InsertColumn(hSrchList,2,&COL);
	return 0;
}

LRESULT Srch_OnDestroy(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	SetWindowLong(hSrchEdit,GWL_WNDPROC,(LONG)OldEditProc);
	return 0;
}

LRESULT Srch_OnPaint(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HFONT hOldFont;
	static TCHAR *c[]={"검색 대상","파일 이름"};

	hdc=BeginPaint(hWnd, &ps);
	hOldFont=(HFONT)SelectObject(hdc,hGulim);
	SetBkMode(hdc,TRANSPARENT);
	TextOut(hdc,40,9,c[0],lstrlen(c[0]));
	TextOut(hdc,290,9,c[1],lstrlen(c[1]));
	SelectObject(hdc,hOldFont);
	EndPaint(hWnd, &ps);
	return 0;
}

LRESULT Srch_OnCommand(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	switch (LOWORD(wParam)) {
	case 2:
		StartSearch();
		break;
	}
	return 0;
}

LRESULT Srch_OnNotify(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	LPNMHDR hdr;
	LPNMITEMACTIVATE nia;
	TCHAR FullPath[MAX_PATH]={0,};
	HTREEITEM hVolNode;

	hdr=(LPNMHDR)lParam;
	nia=(LPNMITEMACTIVATE)lParam;
	if (hdr->hwndFrom == hSrchList) {
		switch (hdr->code) {
		case NM_DBLCLK:
			if (nia->iItem != -1) {
				hVolNode=(HTREEITEM)ListView_GetParam(hSrchList,nia->iItem);
				ListView_GetItemText(hSrchList,nia->iItem,2,FullPath,MAX_PATH);
				GotoItemInTree(hVolNode,FullPath);
			}
			break;
		}
	}
	return 0;
}

LRESULT Srch_OnSize(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	RECT crt;
	if (wParam != SIZE_MINIMIZED) {
		GetClientRect(hWnd,&crt);
		MoveWindow(hSrchList,0,35,crt.right,crt.bottom-35,TRUE);
	}
	return 0;
}


/////////////////////////////////////////////////////////////////////////
// 일반 함수들

void OnTreePopup()
{
	POINT pt;
	TVHITTESTINFO thi;
	HMENU hMenu,hPopup;
	int PopupIdx;
	WORD Cmd;
	HTREEITEM hCabNode,hRoot;
	SCabinet *pCab;
	
	GetCursorPos(&pt);
	thi.pt=pt;
	ScreenToClient(hTree,&thi.pt);
	TreeView_HitTest(hTree,&thi);
	if (thi.hItem == NULL) {
		PopupIdx=0;
	} else {
		PopupIdx=TreeView_GetDepth(hTree,thi.hItem)+1;
	}

	hMenu=LoadMenu(g_hInst,MAKEINTRESOURCE(IDR_POPUP));
	hPopup=GetSubMenu(hMenu,PopupIdx);

	Cmd=TrackPopupMenu(hPopup,TPM_LEFTALIGN | TPM_RETURNCMD,
		pt.x,pt.y,0,hWndMain,NULL);

	// 팝업 명령을 직접 처리한다. 그래야 팝업 대상 노드에 대해 정확하게 동작할 수 있다.
	switch (Cmd) {
	case IDM_EMPTY_NEWCABINET:
		NewCabinet();
		break;
	case IDM_EMPTY_OPENCABINET:
		Open();
		break;
	case IDM_EMPTY_CLOSEALL:
		for (;;) {
			hRoot=TreeView_GetRoot(hTree);
			if (hRoot == NULL) break;
			if (ConfirmSave(hRoot) == IDCANCEL) break;
			TreeView_DeleteItem(hTree,hRoot);
		}
		ListView_DeleteAllItems(hList);
		break;
	case IDM_EMPTY_SAVEALL:
		for (hRoot=TreeView_GetRoot(hTree);hRoot;) {
			Save(hRoot);
			hRoot=TreeView_GetNextSibling(hTree,hRoot);
		}
		break;
	// 노드만 지우면 나머지는 TVN_DELETEITEM에서 뒷처리한다.
	case IDM_CAB_CLOSE:
		if (ConfirmSave(thi.hItem)==IDCANCEL) {
			break;
		}
		TreeView_DeleteItem(hTree,thi.hItem);
		ListView_DeleteAllItems(hList);
		break;
	case IDM_VOL_DELETE:
		hCabNode=GetCabinetNode(thi.hItem);
		pCab=(SCabinet *)TreeView_GetParam(hTree,hCabNode);
		pCab->bModified=TRUE;
		TreeView_DeleteItem(hTree,thi.hItem);
		break;
	case IDM_CAB_SAVE:
		Save(thi.hItem);
		break;
	case IDM_CAB_SAVEAS:
		SaveAs(thi.hItem);
		break;
	case IDM_CAB_ADDVOL:
		AddVolume(thi.hItem);
		break;
	case IDM_CAB_EDIT:
		EditCabinet(thi.hItem);
		break;
	case IDM_VOL_REMAKE:
		RemakeVolume(thi.hItem);
		break;
	case IDM_REC_DELETE:
		DeleteRecord(thi.hItem);
		break;
	}
	DestroyMenu(hMenu);
}

void NewCabinet(BOOL bDefault/*=FALSE*/)
{
	TVINSERTSTRUCT TI;
	SCabinet *pCab,tCab;
	HTREEITEM Node;

	memset(&tCab,0,sizeof(SCabinet));
	if (bDefault) {
		lstrcpy(tCab.Name,"새 캐비넷");
		lstrcpy(tCab.Desc,"디폴트로 만들어진 캐비넷");
	} else {
		if (DialogBoxParam(g_hInst,MAKEINTRESOURCE(IDD_DLGNEWCAB),
			hWndMain,NewCabinetProc,(LPARAM)&tCab)==IDCANCEL)
			return;
	}

	pCab=(SCabinet *)calloc(sizeof(SCabinet),1);
	lstrcpy(pCab->Magic,"FileCabinet");
	pCab->Ver=VERSION;
	lstrcpy(pCab->Name,tCab.Name);
	lstrcpy(pCab->Desc,tCab.Desc);
	lstrcpy(pCab->Path,"이름없음");
	pCab->Compress=tCab.Compress;
	pCab->Encrypt=tCab.Encrypt;
	lstrcpy(pCab->Password,tCab.Password);
	pCab->bModified=TRUE;
	// 아직 미구현이므로 무조건 선택하지 않은 것으로
	pCab->Compress=0;

	TI.hParent=TVI_ROOT;
	TI.hInsertAfter=TVI_LAST;
	TI.item.mask=TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	TI.item.iImage=0;
	TI.item.iSelectedImage=1;
	TI.item.pszText=pCab->Name;
	TI.item.lParam=(LPARAM)pCab;
	Node=TreeView_InsertItem(hTree, &TI);

	TreeView_SelectItem(hTree,Node);
}

void EditCabinet(HTREEITEM hCabItem)
{
	SCabinet *pCab;

	pCab=(SCabinet *)TreeView_GetParam(hTree,hCabItem);
	if (DialogBoxParam(g_hInst,MAKEINTRESOURCE(IDD_DLGNEWCAB),
		hWndMain,NewCabinetProc,(LPARAM)pCab)==IDCANCEL) {
		return;
	}
	TreeView_SetText(hTree,hCabItem,pCab->Name);
	pCab->bModified=TRUE;
}

// 새 캐비넷 또는 캐비넷 속성 편집. lParam으로 SCabinet 포인터가 전달된다.
BOOL CALLBACK NewCabinetProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	static SCabinet *pCab;

	switch (iMessage) {
	case WM_INITDIALOG:
		pCab=(SCabinet *)lParam;
		if (pCab->Name[0]==0) {
			SetWindowText(hDlg,"새 캐비넷 생성");
			SetDlgItemText(hDlg,IDC_EDCABNAME,"새 캐비넷");
			SetDlgItemText(hDlg,IDC_EDCABDESC,"간단한 설명");
		} else {
			SetWindowText(hDlg,"캐비넷 속성 편집");
			SetDlgItemText(hDlg,IDC_EDCABNAME,pCab->Name);
			SetDlgItemText(hDlg,IDC_EDCABDESC,pCab->Desc);
		}
		if (pCab->Compress) {
			 SendDlgItemMessage(hDlg,IDC_CHKCOMP,BM_SETCHECK,BST_CHECKED,0);
		}
		if (pCab->Encrypt) {
			 SendDlgItemMessage(hDlg,IDC_CHKENCRYPT,BM_SETCHECK,BST_CHECKED,0);
		}
		SetDlgItemText(hDlg,IDC_EDPASSWORD,pCab->Password);
		SendDlgItemMessage(hDlg,IDC_EDCABNAME,EM_LIMITTEXT,63,0);
		SendDlgItemMessage(hDlg,IDC_EDCABDESC,EM_LIMITTEXT,63,0);
		SendDlgItemMessage(hDlg,IDC_EDPASSWORD,EM_LIMITTEXT,31,0);
		MoveToParentCenter(hDlg);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			GetDlgItemText(hDlg,IDC_EDCABNAME,pCab->Name,64);
			GetDlgItemText(hDlg,IDC_EDCABDESC,pCab->Desc,64);
			pCab->Compress=SendDlgItemMessage(hDlg,IDC_CHKCOMP,BM_GETCHECK,0,0);
			pCab->Encrypt=SendDlgItemMessage(hDlg,IDC_CHKENCRYPT,BM_GETCHECK,0,0);
			GetDlgItemText(hDlg,IDC_EDPASSWORD,pCab->Password,64);
			EndDialog(hDlg,IDOK);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg,IDCANCEL);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

void AddVolume(HTREEITEM hCabItem)
{
	SVolume tVol;
	HTREEITEM NewNode;

	// 빌드 옵션 입력.
	memset(&tVol,0,sizeof(SVolume));
	tVol.Name[0]=0;
	tVol.bIncHid=TRUE;
	tVol.bMakePreview=TRUE;
	if (DialogBoxParam(g_hInst,MAKEINTRESOURCE(IDD_ADDVOLUME),
		hWndMain,AddVolumeProc,(LPARAM)&tVol)==IDCANCEL)
		return;

	NewNode=MakeVolume(&tVol,hCabItem,TVI_LAST);
	if (NewNode != NULL) {
		TreeView_SelectItem(hTree,NewNode);
	} else {
		SendMessage(hState, SB_SETTEXT, 0, (LPARAM)"볼륨 추가중 너의 변심으로 인해 취소했다.");
	}
}

BOOL CALLBACK AddVolumeProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	TCHAR szFolder[MAX_PATH];
	TCHAR ch,szRoot[10],szLabel[MAX_PATH],szDrive[128],szType[16];
	TCHAR szFileSystem[32];
	int idx;
	static int firstcd=0;
	static SVolume *pVol;
	LVCOLUMN COL;
	static HWND hDriveList;
	LVITEM LI;
	DWORD Serial;
	UINT Type;

	switch (iMessage) {
	case WM_INITDIALOG:
		pVol=(SVolume *)lParam;
		hDriveList=GetDlgItem(hDlg,IDC_DRIVES);
		ListView_SetExtendedListViewStyle(hDriveList, LVS_EX_FULLROWSELECT);
		COL.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
		COL.fmt=LVCFMT_LEFT;
		COL.cx=50;
		COL.pszText="드라이브";
		COL.iSubItem=0;
		ListView_InsertColumn(hDriveList,0,&COL);

		COL.pszText="타입";
		COL.fmt=LVCFMT_RIGHT;
		COL.cx=70;
		COL.iSubItem=1;
		ListView_InsertColumn(hDriveList,1,&COL);

		COL.pszText="레이블";
		COL.fmt=LVCFMT_LEFT;
		COL.cx=100;
		COL.iSubItem=2;
		ListView_InsertColumn(hDriveList,2,&COL);

		// 드라이브 목록 조사
		for (ch='C';ch<='Z';ch++) {
			wsprintf(szRoot,"%c:\\",ch);
			Type=GetDriveType(szRoot);
			if (Type == DRIVE_UNKNOWN || Type == DRIVE_NO_ROOT_DIR) {
				continue;
			}
			// 정보를 구할 수 없으면 0을 리턴한다. CD-ROM의 경우 미디어가 없으면 목록에 나오지 않음
			if (GetVolumeInformation(szRoot,szLabel,128,&Serial,NULL,NULL,szFileSystem,32)==FALSE) {
				if (Type == DRIVE_CDROM) {
					lstrcpy(szLabel,"미디어가 없음");
				}
			}
			LI.mask=LVIF_TEXT;
			LI.state=0;
			LI.stateMask=0;
			LI.iSubItem=0;
			LI.iItem=1000;
			wsprintf(szDrive,"%c",ch);
			LI.pszText=szDrive;
			idx=ListView_InsertItem(hDriveList,&LI);
			switch (Type) {
			case DRIVE_FIXED:
				ListView_SetItemText(hDriveList,idx,1,"고정식");
				break;
			case DRIVE_REMOVABLE:
				ListView_SetItemText(hDriveList,idx,1,"이동식");
				break;
			case DRIVE_CDROM:
				ListView_SetItemText(hDriveList,idx,1,"CD-ROM");
				// 처음 실행할 때 첫 번째 CD-ROM을 선택해 준다.
				if (firstcd==0) {
					firstcd=ch-'C';
				}
				break;
			case DRIVE_RAMDISK:
				ListView_SetItemText(hDriveList,idx,1,"RAM 드라이브");
				break;
			case DRIVE_REMOTE:
				ListView_SetItemText(hDriveList,idx,1,"네트워크");
				break;
			default:
				continue;
			}
			ListView_SetItemText(hDriveList,idx,2,szLabel);
		}

		// Name이 빈 문자열이면 새로운 볼륨 작성
		if (pVol->Name[0]==0) {
			SetDlgItemText(hDlg,IDC_EDTITLE,"볼륨의 제목");
			SetDlgItemText(hDlg,IDC_EDDESC,"볼륨에 대한 설명");
			ListView_SetItemState(hDriveList,firstcd,LVIS_FOCUSED | LVIS_SELECTED,
				LVIS_FOCUSED | LVIS_SELECTED);
			ListView_EnsureVisible(hDriveList,firstcd,FALSE);
		} else {
			SetDlgItemText(hDlg,IDC_EDFOLDER,pVol->Path);
			SetDlgItemText(hDlg,IDC_EDTITLE,pVol->Name);
			SetDlgItemText(hDlg,IDC_EDDESC,pVol->Desc);
			EnableWindow(GetDlgItem(hDlg,IDC_DRIVES),FALSE);
			EnableWindow(GetDlgItem(hDlg,IDC_BTNBROWSE),FALSE);
			EnableWindow(GetDlgItem(hDlg,IDC_EDFOLDER),FALSE);
		}
		if (pVol->bIncHid) {
			 SendDlgItemMessage(hDlg,IDC_CHKINCHID,BM_SETCHECK,BST_CHECKED,0);
		}
		if (pVol->bMakePreview) {
			 SendDlgItemMessage(hDlg,IDC_CHKMAKEPREVIEW,BM_SETCHECK,BST_CHECKED,0);
		}
		MoveToParentCenter(hDlg);
		return TRUE;
	case WM_NOTIFY:
		LPNMHDR hdr;
		LPNMLISTVIEW nlv;
		hdr=(LPNMHDR)lParam;
		nlv=(LPNMLISTVIEW)lParam;
		if (hdr->hwndFrom == hDriveList) {
			switch (hdr->code) {
			case LVN_ITEMCHANGED:
				if (nlv->uChanged == LVIF_STATE && nlv->uNewState == (LVIS_SELECTED | LVIS_FOCUSED)) {
					firstcd=nlv->iItem;
					ListView_GetItemText(hDriveList,nlv->iItem,0,szDrive,255);
					wsprintf(szRoot,"%c:\\",szDrive[0]);
					SetDlgItemText(hDlg,IDC_EDFOLDER,szRoot);
					ListView_GetItemText(hDriveList,nlv->iItem,1,szType,16);
					ListView_GetItemText(hDriveList,nlv->iItem,2,szLabel,MAX_PATH);
					wsprintf(szFolder,"%c:(%s)-%s",szDrive[0],szType,szLabel);
					SetDlgItemText(hDlg,IDC_EDTITLE,szFolder);
				}
				break;
			}
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_BTNBROWSE:
			GetDlgItemText(hDlg,IDC_EDFOLDER,szFolder,MAX_PATH);
			if (BrowseFolder(hDlg,"디렉토리 선택",szFolder,szFolder)==TRUE) {
				SetDlgItemText(hDlg,IDC_EDFOLDER,szFolder);
				SetDlgItemText(hDlg,IDC_EDTITLE,szFolder);
			}
			return TRUE;
		case IDOK:
			GetDlgItemText(hDlg,IDC_EDFOLDER,pVol->Path,MAX_PATH);
			GetDlgItemText(hDlg,IDC_EDTITLE,pVol->Name,64);
			if (lstrlen(pVol->Path) == 0) {
				MessageBox(hDlg,"대상 경로를 선택하십시오.","알림",MB_OK);
				return TRUE;
			}
			if (lstrlen(pVol->Name) == 0) {
				MessageBox(hDlg,"볼륨의 제목을 입력하십시오.","알림",MB_OK);
				return TRUE;
			}
			wsprintf(szRoot,"%c:\\",pVol->Path[0]);
			if (GetVolumeInformation(szRoot,szLabel,128,&Serial,NULL,NULL,szFileSystem,32)==FALSE) {
				MessageBox(hDlg,"현재 읽을 수 없는 드라이브야. 미디어가 있는지 확인한 후 다시 시도해.","알림",MB_OK);
				return TRUE;
			}
			lstrcpy(pVol->Label,szLabel);
			pVol->Serial=Serial;
			lstrcpy(pVol->FilsSystem,szFileSystem);
			pVol->bIncHid=IsDlgButtonChecked(hDlg,IDC_CHKINCHID);
			pVol->bZipDir=FALSE;//IsDlgButtonChecked(hDlg,IDC_CHKZIPDIR);
			pVol->bMakePreview=IsDlgButtonChecked(hDlg,IDC_CHKMAKEPREVIEW);
			GetDlgItemText(hDlg,IDC_EDDESC,pVol->Desc,64);
			EndDialog(hDlg,IDOK);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg,IDCANCEL);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

// 디렉토리에 속한 파일의 개수와 폴더 개수, 그리고 총 용량을 계산한다.
// 자신이 먼저 발견되고 자식들이 발견되므로 순회중에는 이 작업을 할 수 없다.
void BuildDirectoryInfo(SVolume *pVol)
{
	int i;
	int parent;
	SRecord *pRec;

	// 모든 레코드를 순회하되 0번 루트는 제외
	for (i=1;i<pVol->nRecord;i++) {
		pRec=&pVol->arRecord[i];
		if ((pRec->Attr & FA_DIRECTORY) == 0) {
			// 파일:자신보다 상위의 모든 부모를 찾아 용량과 파일 개수를 증가시킨다.
			for (parent=pRec->Parent;parent != 0;parent=pVol->arRecord[parent].Parent) {
				pVol->arRecord[parent].Size += pRec->Size;
				pVol->arRecord[parent].nFile++;
			}
		} else {
			// 디렉토리:자신보다 상위의 모든 부모를 찾아 폴더수만 증가시킨다.
			for (parent=pRec->Parent;parent != 0;parent=pVol->arRecord[parent].Parent) {
				pVol->arRecord[parent].nDir++;
			}
		}
	}
}

// pVolInfo의 정보대로 볼륨을 작성해서 hParent의 hAfter 다음에 삽입한다.
HTREEITEM MakeVolume(SVolume *pVolInfo,HTREEITEM hParent,HTREEITEM hAfter)
{
	SVolume *pVol;
	TVINSERTSTRUCT TI;
	TVITEM Tv;
	HTREEITEM Node;
	SYSTEMTIME st;
	SCabinet *pCab;

	SetCursor(LoadCursor(NULL,IDC_WAIT));

	// 인수로 전달된 볼륨 정보를 일단 대입한다.
	pVol=(SVolume *)malloc(sizeof(SVolume));
	*pVol=*pVolInfo;
	GetLocalTime(&st);
	pVol->Time=MyGetAbsSecond(st);

	// 레코드 초기 할당 및 루트 등록
	pVol->arRecord = (SRecord *)calloc(sizeof(SRecord)*1000,1);
	lstrcpyn(pVol->arRecord[0].Name,pVol->Name,NAMELIM);
	pVol->arRecord[0].Id=0;
	pVol->arRecord[0].Parent=-1;
	pVol->arRecord[0].Attr=FA_DIRECTORY;
	pVol->nRecord=1;

	// 하위 파일들 검색
	TotalFind=0;
	pTargetVol=pVol;
	// 작성중에 취소되었으면 지금까지 할당한 모든 메모리를 해제한다.
	if (FindFileRecursive(pVol->Path)==FALSE) {
		FreeVolume(pVol);
		return NULL;
	}
	BuildDirectoryInfo(pVol);
	SendMessage(hState, SB_SETTEXT, 0, (LPARAM)"볼륨 작성 완료");

	// 트리에 볼륨 노드 등록
	TI.hParent=hParent;
	TI.hInsertAfter=hAfter;
	TI.item.mask=TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	TI.item.iImage=2;
	TI.item.iSelectedImage=3;
	TI.item.pszText=pVol->Name;
	TI.item.lParam=(LPARAM)pVol;
	Node=TreeView_InsertItem(hTree, &TI);
	TreeView_Expand(hTree,TI.hParent,TVE_EXPAND);

	// 차일드가 있다는 표시만 한다.
	Tv.mask=TVIF_CHILDREN;
	Tv.hItem=Node;
	Tv.cChildren=1;
	TreeView_SetItem(hTree,&Tv);

	// 다시 그려야 + 표시가 보임
	InvalidateRect(hTree,NULL,TRUE);
	pCab=(SCabinet *)TreeView_GetParam(hTree,hParent);
	pCab->bModified=TRUE;
	return Node;
}

void RemakeVolume(HTREEITEM hVolItem)
{
	HTREEITEM hSib,NewNode;
	SVolume *pVol,tVol;

	pVol=(SVolume *)TreeView_GetParam(hTree,hVolItem);
	tVol=*pVol;
	if (DialogBoxParam(g_hInst,MAKEINTRESOURCE(IDD_ADDVOLUME),
		hWndMain,AddVolumeProc,(LPARAM)&tVol)==IDCANCEL)
		return;

	// 일련 번호 검사
	if (tVol.Serial != pVol->Serial) {
		if (MessageBox(hWndMain,"원본 미디어와 일련 번호가 다릅니다.\r\n"
			"그래도 다시 작성하시겠습니까?","확인",MB_YESNO)==IDNO) {
			return;
		}
	}

	// 원본 경로의 존재 여부 조사
	if (_access(tVol.Path,0) != 0) {
		MessageBox(hWndMain,"원본 경로를 찾을 수 없습니다.","확인",MB_OK);
		return;
	}

	// 옵션 변경 여부와 상관없이 다시 작성한다.
	hSib=TreeView_GetPrevSibling(hTree,hVolItem);
	if (hSib==NULL) {
		hSib=TVI_FIRST;
	}
	NewNode=MakeVolume(&tVol,TreeView_GetParent(hTree,hVolItem),hSib);
	TreeView_DeleteItem(hTree,hVolItem);
	TreeView_SelectItem(hTree,NewNode);
}

// 지정한 경로의 파일을 검색하여 전역 변수 pTargetVol에 추가한다.
// FALSE를 리턴하면 검색중에 중단했다는 뜻이다.
BOOL FindFileRecursive(TCHAR *Path)
{
	TCHAR SrchPath[MAX_PATH];
	TCHAR szFinded[MAX_PATH];
	WIN32_FIND_DATA wfd;
	HANDLE hSrch;
	BOOL nResult=TRUE;
	int ThisId;
	BOOL bSuccess=TRUE;

	lstrcpy(SrchPath, Path);
	if (SrchPath[lstrlen(SrchPath)-1] == '\\') {
		SrchPath[lstrlen(SrchPath)-1]=0;
	}
	lstrcat(SrchPath, "\\*.*");
	hSrch=FindFirstFile(SrchPath,&wfd);
	if (hSrch == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	// 현재 폴더의 ID를 저장해 놓는다. 반드시 지역 변수여야 한다.
	ThisId=pTargetVol->nRecord-1;

	while (nResult) {
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			if (MessageBox(hWndMain,"헥헥! 디스크 박박 뒤지는 중인데 힘들어 죽겄다. 그만 둘까?","질문",MB_YESNO)==IDYES) {
				bSuccess=FALSE;
				goto end;
			}
		}
		if (lstrlen(Path)==3) {
			wsprintf(szFinded,"%s%s",Path,wfd.cFileName);
		} else {
			wsprintf(szFinded,"%s\\%s",Path,wfd.cFileName);
		}
		// 숨겨진 파일은 옵션이 설정되어 있을 때만
		if ((wfd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)==0 || pTargetVol->bIncHid==TRUE) {
			if (pTargetVol->bZipDir==TRUE) {
				// todo : 확장자 ZIP에 대해 안쪽도 검사하도록 할 것
			}
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if (lstrcmp(wfd.cFileName,".") && lstrcmp(wfd.cFileName,"..") &&
					lstrcmpi(wfd.cFileName,"RECYCLER") && lstrcmpi(wfd.cFileName,"System Volume Information")) {
					AddFileToVolume(szFinded,wfd.dwFileAttributes,ThisId,pTargetVol);
					if (FindFileRecursive(szFinded)==FALSE) {
						bSuccess=FALSE;
						goto end;
					}
				}
			} else {
				AddFileToVolume(szFinded,wfd.dwFileAttributes,ThisId,pTargetVol);
			}
		}
		nResult=FindNextFile(hSrch,&wfd);
	}

end:
	FindClose(hSrch);
	return bSuccess;
}

// 파일을 불륨에 추가한다.
int AddFileToVolume(TCHAR *Path,DWORD Attr,int pid,SVolume *pVol)
{
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR fname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];
	TCHAR FileName[MAX_PATH];
	int Size;
	HANDLE hFile;
	SRecord *pRec;
	FILETIME CrTime,MoTime,AcTime,LoTime;
	SYSTEMTIME st;
	TCHAR StateText[MAX_PATH*2];
	DWORD SizeHigh;
	DWORD dwRead;

	// 부족시 메모리 재할당.
	Size=_msize(pVol->arRecord)/sizeof(SRecord);
	if (pVol->nRecord >= Size) {
		Size += 1000;
		pVol->arRecord = (SRecord *)realloc(pVol->arRecord,sizeof(SRecord)*Size);
		memset(&pVol->arRecord[Size-1000],0,sizeof(SRecord)*1000);
	}

	TotalFind++;
	wsprintf(StateText,"(작성중:취소는 ESC) %d번째 파일 %s 처리중",TotalFind,Path);
	SendMessage(hState, SB_SETTEXT, 0, (LPARAM)StateText);

	// 발견된 파일의 정보 기록
	pRec=&pVol->arRecord[pVol->nRecord];
	pRec->Id=pVol->nRecord;
	_splitpath(Path,drive,dir,fname,ext);
	wsprintf(FileName,"%s%s",fname,ext);
	// NAMELIM자가 넘으면 동적으로 할당한다.NAMELIM-1자까지만 Name에 기록한다.
	if (lstrlen(FileName) < NAMELIM) {
		lstrcpyn(pRec->Name,FileName,NAMELIM);
	} else {
		lstrcpy(pRec->stName.IsPtr,":::");
		pRec->stName.pName=(TCHAR *)malloc(lstrlen(FileName)+1);
		lstrcpy(pRec->stName.pName,FileName);
	}
	pRec->Parent=pid;
	pRec->Attr=Attr;

	// 미리보기는 일단 없음
	pRec->PvFormat=PV_NONE;
	pRec->PvPtr=NULL;
	pRec->PvLen=0;
	if (pVol->bMakePreview==TRUE) {
		// todo : 확장자에 따른 미리 보기 작성. 미리 보기 작성 규칙(크기, 대상 파일 등)은 전역 옵션으로 유지할 것
		if (lstrcmpi(ext,".txt")==0) {
			hFile=CreateFile(Path,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
			if (hFile != INVALID_HANDLE_VALUE) {
				pRec->PvFormat=PV_TEXT;
				pRec->PvLen=128;
				pRec->PvPtr=(BYTE *)calloc(pRec->PvLen,1);
				ReadFile(hFile,pRec->PvPtr,pRec->PvLen,&dwRead,0);
				CloseHandle(hFile);
			}
		}
	}

	// 디렉토리를 열려면 별도의 플래그를 지정해야 한다.
	hFile=CreateFile(Path,0,0,NULL,OPEN_EXISTING,FILE_FLAG_BACKUP_SEMANTICS,NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		pRec->Size=GetFileSize(hFile,&SizeHigh);
		pRec->Size |= (SizeHigh << 32);
		GetFileTime(hFile,&CrTime,&MoTime,&AcTime);

		if (lstrcmp(pVol->FilsSystem,"NTFS")==0) FileTimeToLocalFileTime(&MoTime,&LoTime); else LoTime=MoTime;
		FileTimeToSystemTime(&LoTime,&st);
		pRec->ModiTime=MyGetAbsSecond(st);
		/* 1.0에서는 변경 시간만 기록하기로 함
		if (lstrcmp(pVol->FilsSystem,"NTFS")==0) FileTimeToLocalFileTime(&CrTime,&LoTime); else LoTime=CrTime;
		FileTimeToSystemTime(&LoTime,&st);
		pRec->CreateTime=MyGetAbsSecond(st);
		if (lstrcmp(pVol->FilsSystem,"NTFS")==0) FileTimeToLocalFileTime(&AcTime,&LoTime); else LoTime=AcTime;
		FileTimeToSystemTime(&LoTime,&st);
		pRec->AccTime=MyGetAbsSecond(st);
		*/
		CloseHandle(hFile);
	}

	pVol->nRecord++;
	return 0;
}

// pVol의 pid를 부모로 가지는 항목들을 ParentNode에 추가한다.
void InsertChild(SVolume *pVol, HTREEITEM ParentNode, int pid)
{
	TVINSERTSTRUCT TI;
	HTREEITEM Node;
	int i, j;
	TVITEM Tv;
	BOOL bHaveChild;
	int id;

	for (i=0;i<pVol->nRecord;i++) {
		if (pVol->arRecord[i].Parent==pid) {
			// 디렉토리만 채운다.
			if ((pVol->arRecord[i].Attr & FA_DIRECTORY)==0) {
				continue;
			}
			TI.hParent=ParentNode;
			TI.hInsertAfter=TVI_SORT;
			TI.item.mask=TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
			TI.item.iImage=4;
			TI.item.iSelectedImage=5;
			if (lstrcmp(pVol->arRecord[i].stName.IsPtr,":::")!=0) {
				TI.item.pszText=pVol->arRecord[i].Name;
			} else {
				TI.item.pszText=pVol->arRecord[i].stName.pName;
			}
			// 편집되면 id가 배열 첨자와 다를 수 있으므로 레코드에서 읽어야 한다.
			id=pVol->arRecord[i].Id;
			TI.item.lParam=id;
			Node=TreeView_InsertItem(hTree, &TI);

			// 이 항목의 자식 항목이 존재하는지 조사한다. 단, 디렉토리만
			bHaveChild=FALSE;
			for (j=0;j<pVol->nRecord;j++) {
				if ((pVol->arRecord[j].Parent == id) && (pVol->arRecord[j].Attr & FA_DIRECTORY)) {
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

// pVol의 pid아래의 파일들을 리스트 뷰에 출력한다.
void ListFile(SVolume *pVol,int pid)
{
	LVITEM LI;
	int i,idx;
	TCHAR str[128];
	SYSTEMTIME st;
	__int64 Size;
	int id;
	SHFILEINFO si;

	ListView_DeleteAllItems(hList);
	for (i=0;i<pVol->nRecord;i++) {
		if (pVol->arRecord[i].Parent==pid) {
			LI.mask=LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
			LI.state=0;
			LI.stateMask=0;
			LI.iSubItem=0;
			if (lstrcmp(pVol->arRecord[i].stName.IsPtr,":::")!=0) {
				LI.pszText=pVol->arRecord[i].Name;
			} else {
				LI.pszText=pVol->arRecord[i].stName.pName;
			}
			id=pVol->arRecord[i].Id;
			LI.lParam=id;

			// 시스템 아이콘 사용
			if ((pVol->arRecord[i].Attr & FA_DIRECTORY)!=0) {
				SHGetFileInfo(LI.pszText,FILE_ATTRIBUTE_DIRECTORY,&si,sizeof(si),
					SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES);
			} else {
				SHGetFileInfo(LI.pszText,FILE_ATTRIBUTE_NORMAL,&si,sizeof(si),
					SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES);
			}
			LI.iImage=si.iIcon;

			LI.iItem=FindInsertIdx(LI,&pVol->arRecord[i]);
			idx=ListView_InsertItem(hList,&LI);

			// 크기 출력.
			Size=pVol->arRecord[i].Size;
			switch (SizeType) {
			case ST_BYTE:
				wsprintf(str,"%I64d",Size);
				break;
			case ST_AKMG:
				if (Size < 1024) {
					wsprintf(str,"%I64d",Size);
				} else if (Size < 1048576) {
					wsprintf(str,"%dK",Size/1024);
				} else if (Size < 1073741824) {
					wsprintf(str,"%dM",Size/1048576);
				} else {
					wsprintf(str,"%dG",Size/1073741824);
				}
				break;
			case ST_ApBKMG:
				if (Size < 1024) {
					wsprintf(str,"%I64d",Size);
				} else if (Size < 1048576) {
					sprintf(str,"%.1fK",(double)Size/1024);
				} else if (Size < 1073741824) {
					sprintf(str,"%.1fM",(double)Size/1048576);
				} else {
					sprintf(str,"%.1fG",(double)Size/1073741824);
				}
				break;
			}
			ListView_SetItemText(hList,idx,1,str);

			// 날짜는 수정한 날짜를 보여주고 년도는 두자리만. 
			// 디렉토리의 경우는 생성한 날짜를 보여주되 NTFS는 그렇지만 CDFS는 틀린 날짜가 조사되므로 다른 방법을 사용해야 할 것 같다. 
			// 일단 1.0에서는 무시하고 날짜 대신 소속 파일 개수와 디렉토리 개수를 출력한다.
			if ((pVol->arRecord[i].Attr & FA_DIRECTORY)) {
				wsprintf(str,"파일:%d개,폴더:%d개",pVol->arRecord[i].nFile,pVol->arRecord[i].nDir);
			} else {
				MyAbsSecondToSystem(pVol->arRecord[i].ModiTime,st);
				wsprintf(str,"%02d/%02d/%02d일 %02d:%02d:%02d",st.wYear%100,st.wMonth,st.wDay,
					st.wHour,st.wMinute,st.wSecond);
			}
			ListView_SetItemText(hList,idx,2,str);

			lstrcpy(str,"");
			if (pVol->arRecord[i].Attr & FA_ARCHIVE) lstrcat(str,"A");
			if (pVol->arRecord[i].Attr & FA_SYSTEM) lstrcat(str,"S");
			if (pVol->arRecord[i].Attr & FA_HIDDEN) lstrcat(str,"H");
			if (pVol->arRecord[i].Attr & FA_READONLY) lstrcat(str,"R");
			if (pVol->arRecord[i].Attr & FA_DIRECTORY) lstrcat(str,"D");
			ListView_SetItemText(hList,idx,3,str);
		}
	}
}

// 삽입될 위치를 구한다. 디렉토리가 파일보다 더 앞쪽이어야 한다.
int FindInsertIdx(LVITEM LI, SRecord *pRec)
{
	int idx,Count;
	TCHAR Text[128];
	TCHAR Attr[16];

	Count=ListView_GetItemCount(hList);
	idx=0;

	// 파일이면 일단 디렉토리를 건너뛴다. 속성에 D가 있으면 디렉토리이다.
	if ((pRec->Attr & FA_DIRECTORY)==0) {
		for (;idx<Count;idx++) {
			ListView_GetItemText(hList,idx,3,Attr,16);
			if (strchr(Attr,'D')==NULL) break;
		}
	}

	// 텍스트를 비교해서 자신보다 큰 항목 앞에 위치한다.
	for (;idx<Count;idx++) {
		// 디렉토리인 경우는 파일보다 무조건 앞에 있어야 한다.
		ListView_GetItemText(hList,idx,3,Attr,16);
		if ((pRec->Attr & FA_DIRECTORY)!=NULL && strchr(Attr,'D')==NULL) {
			return idx;
		}
		ListView_GetItemText(hList,idx,0,Text,128);
		if (lstrcmpi(LI.pszText,Text) < 0) return idx;
	}

	return idx;
}

HTREEITEM GetCabinetNode(HTREEITEM Node)
{
	HTREEITEM tNode,tNode2;

	for (tNode=Node;;) {
		tNode2=TreeView_GetParent(hTree,tNode);
		if (tNode2 == NULL) break;
		tNode=tNode2;
	}
	return tNode;
}

HTREEITEM FindVolNode(HTREEITEM Node)
{
	HTREEITEM tNode;

	// 위로 올라가면서 볼륨을 찾는다.
	for (tNode=Node;;tNode=TreeView_GetParent(hTree,tNode)) {
		if (tNode==NULL) {
			break;
		}
		if (TreeView_GetDepth(hTree,tNode) == 1) {
			return tNode;
		}
	}
	return NULL;
}

SVolume *FindVolume(HTREEITEM Node)
{
	HTREEITEM hVolNode;

	hVolNode=FindVolNode(Node);
	if (hVolNode == NULL) return NULL;
	return (SVolume *)TreeView_GetParam(hTree,hVolNode);
}

int ConfirmSave(HTREEITEM hCabItem)
{
	SCabinet *pCab=(SCabinet *)TreeView_GetParam(hTree,hCabItem);
	int result=IDNO;
	TCHAR Mes[MAX_PATH+64];

	if (pCab->bModified) {
		wsprintf(Mes,"%s 파일이 변경되었는데 그냥 갈려고 하네?. 저장할래?",pCab->Path);
		result=MessageBox(hWndMain,Mes,"어랍쇼",MB_YESNOCANCEL);
		if (result == IDCANCEL) {
			return IDCANCEL;
		}
		if (result == IDYES) {
			if (Save(hCabItem) == FALSE)
				return IDCANCEL;
			else 
				return IDYES;
		}
	}
	return result;
}

BOOL Save(HTREEITEM hCabItem)
{
	SCabinet *pCab=(SCabinet *)TreeView_GetParam(hTree,hCabItem);
	if (lstrcmp(pCab->Path,"이름없음")==0) {
		return SaveAs(hCabItem);
	} else {
		return SaveCabinet(hCabItem,pCab->Path);
	}
}

BOOL SaveAs(HTREEITEM hCabItem)
{
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH]="";

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.Flags = OFN_OVERWRITEPROMPT;
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner=hWndMain;
	OFN.lpstrFilter="캐비넷 파일(*.fcf)\0*.fcf\0모든 파일\0*.*\0";
	OFN.lpstrFile=lpstrFile;
	OFN.nMaxFile=256;
	OFN.lpstrDefExt=".fcf";
	if (GetSaveFileName(&OFN)==0) {
		return FALSE;
	}
	
	return SaveCabinet(hCabItem,lpstrFile);
}

// 캐비넷 저장
BOOL SaveCabinet(HTREEITEM hCabItem,TCHAR *Path)
{
	HANDLE hFile;
	DWORD dwWritten;
	int r;
	SVolume *pVol;
	SRecord *pRec;
	SCabinet *pCab;
	HTREEITEM hChild;
	TCHAR *pName;
	WORD len;

	// 변경되지 않았으면 저장하지 않는다. 단, 파일이 삭제되었거나 새 이름으로 저장인 경우는 변경 여부를 점검하지 않는다.
	pCab=(SCabinet *)TreeView_GetParam(hTree,hCabItem);
	if (_access(Path,0)==0) {
		if (pCab->bModified==FALSE) return TRUE;
	}

	hFile=CreateFile(Path,GENERIC_WRITE,0,NULL,
		CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return FALSE;
	}

	// 자식 노드의 개수가 곧 볼륨의 개수이다. 따로 관리하지 않고 저장 직전에 조사한다.
	pCab->nVolume=TreeView_GetChildCount(hTree,hCabItem);
	lstrcpy(pCab->Path,Path);
	pCab->bModified=FALSE;
	WriteFile(hFile,pCab,sizeof(SCabinet),&dwWritten,NULL);

	// 모든 볼륨에 대한 루프
	for (hChild=TreeView_GetChild(hTree,hCabItem);hChild!=NULL;) {
		pVol=(SVolume *)TreeView_GetParam(hTree,hChild);
		WriteFile(hFile,pVol,sizeof(SVolume),&dwWritten,NULL);
		for (r=0;r<pVol->nRecord;r++) {
			pRec=&pVol->arRecord[r];
			// 고정 길이 데이터부터 먼저 저장한다.
			WriteFile(hFile,pRec,offsetof(SRecord,Name),&dwWritten,NULL);

			// 파일명은 2바이트의 길이와 함께 저장한다.
			if (lstrcmp(pRec->stName.IsPtr,":::")==0) {
				pName=pRec->stName.pName;
			} else {
				pName=pRec->Name;
			}
			len=lstrlen(pName);
			WriteFile(hFile,&len,sizeof(WORD),&dwWritten,NULL);
			WriteFile(hFile,pName,len,&dwWritten,NULL);
			// 미리보기 정보가 있으면 따로 저장한다.
			if (pRec->PvFormat != PV_NONE) {
				WriteFile(hFile,&pRec->PvLen,sizeof(DWORD),&dwWritten,NULL);
				WriteFile(hFile,pRec->PvPtr,pRec->PvLen,&dwWritten,NULL);
			}
		}
		hChild=TreeView_GetNextSibling(hTree,hChild);
	}
	CloseHandle(hFile);
	SendMessage(hState, SB_SETTEXT, 0, (LPARAM)"저장했습니다.");
	return TRUE;
}

BOOL Open()
{
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH]="";

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner=hWndMain;
	OFN.lpstrFilter="캐비넷 파일(*.fcf)\0*.fcf\0모든 파일\0*.*\0";
	OFN.lpstrFile=lpstrFile;
	OFN.nMaxFile=256;
	if (GetOpenFileName(&OFN)==0) {
		return FALSE;
	}
	return OpenCabinet(lpstrFile);
}

BOOL OpenCabinet(TCHAR *Path)
{
	HANDLE hFile;
	DWORD dwRead;
	TVINSERTSTRUCT TI;
	SCabinet *pCab=NULL;
	int v,r;
	SVolume *pVol;
	SRecord *pRec;
	WORD len;
	TVITEM Tv;
	HTREEITEM CabNode,VolNode;
	int i;
	TCHAR Pass[32];
	BOOL bResult=TRUE;

	// 이미 열려 있는 캐비넷이면 오픈을 거부한다.
	for (CabNode=TreeView_GetRoot(hTree);CabNode;) {
		pCab=(SCabinet *)TreeView_GetParam(hTree,CabNode);
		if (lstrcmpi(pCab->Path,Path)==0) {
			MessageBox(hWndMain,"이 캐비넷은 조기에 이미 열려 있잖아. 왜 그래~~?","바보야",MB_OK);
			return FALSE;
		}
		CabNode=TreeView_GetNextSibling(hTree,CabNode);
	}

	hFile=CreateFile(Path,GENERIC_READ,0,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return FALSE;
	}

	// 루트 노드만 일단 생성
	pCab=(SCabinet *)calloc(sizeof(SCabinet),1);
	ReadFile(hFile,pCab,sizeof(SCabinet),&dwRead,NULL);
	if (lstrcmp(pCab->Magic,"FileCabinet") != 0) {
		MessageBox(hWndMain,"캐비넷 파일이 아니네.","에러",MB_OK);
		bResult=FALSE;
		goto end;
	}
	if (pCab->Ver!=VERSION) {
		MessageBox(hWndMain,"버전이 틀리므로 읽을 수 없어.","에러",MB_OK);
		bResult=FALSE;
		goto end;
	}
	if (pCab->Encrypt==1) {
		if (DialogBoxParam(g_hInst,MAKEINTRESOURCE(IDD_DLGPASS),
			hWndMain,PasswordProc,(LPARAM)&Pass)==IDCANCEL) {
			bResult=FALSE;
			goto end;
		}
		if (lstrcmp(Pass,pCab->Password) != 0) {
			MessageBox(hWndMain,"암호가 틀리므로 캐비넷을 열 수 없다.","알림",MB_OK);
			bResult=FALSE;
			goto end;
		}
	}
	pCab->bModified=FALSE;

	// 루트 노드 삽입
	TI.hParent=TVI_ROOT;
	TI.hInsertAfter=TVI_LAST;
	TI.item.mask=TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	TI.item.iImage=0;
	TI.item.iSelectedImage=1;
	TI.item.pszText=pCab->Name;
	TI.item.lParam=(LPARAM)pCab;
	CabNode=TreeView_InsertItem(hTree, &TI);

	for (v=0;v<pCab->nVolume;v++) {
		pVol=(SVolume *)calloc(sizeof(SVolume),1);
		ReadFile(hFile,pVol,sizeof(SVolume),&dwRead,NULL);

		// 볼륨 노드는 일단 트리에 추가한다.
		TI.hParent=CabNode;
		TI.hInsertAfter=TVI_LAST;
		TI.item.mask=TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
		TI.item.iImage=2;
		TI.item.iSelectedImage=3;
		TI.item.pszText=pVol->Name;
		TI.item.lParam=(LPARAM)pVol;
		VolNode=TreeView_InsertItem(hTree, &TI);
		
		pVol->arRecord=(SRecord *)malloc(sizeof(SRecord)*pVol->nRecord);
		for (r=0;r<pVol->nRecord;r++) {
			pRec=&pVol->arRecord[r];
			ReadFile(hFile,pRec,offsetof(SRecord,Name),&dwRead,NULL);

			// NAMELIM-1자까지는 Name에 읽고 그 이상은 동적으로 할당한다.
			ReadFile(hFile,&len,sizeof(WORD),&dwRead,NULL);
			if (len < NAMELIM) {
				ReadFile(hFile,pRec->Name,len,&dwRead,NULL);
				pRec->Name[len]=0;
			} else {
				pRec->stName.pName=(TCHAR *)calloc(len+1,1);
				lstrcpy(pRec->stName.IsPtr,":::");
				ReadFile(hFile,pRec->stName.pName,len,&dwRead,NULL);
			}
			// 미리 보기 정보도 읽어들인다.
			if (pRec->PvFormat != PV_NONE) {
				ReadFile(hFile,&pRec->PvLen,sizeof(DWORD),&dwRead,NULL);
				pRec->PvPtr=(BYTE *)calloc(pRec->PvLen,1);
				ReadFile(hFile,pRec->PvPtr,pRec->PvLen,&dwRead,NULL);
			}
		}

		// 디렉토리가 하나라도 있으면 + 표시 나오도록 한다. 파일만 있으면 표시할 필요없다.
		for (i=1;i<pVol->nRecord;i++) {
			if (pVol->arRecord[i].Attr & FA_DIRECTORY) {
				Tv.mask=TVIF_CHILDREN;
				Tv.hItem=VolNode;
				Tv.cChildren=1;
				TreeView_SetItem(hTree,&Tv);
				break;
			}
		}
	}

	TreeView_Expand(hTree,CabNode,TVE_EXPAND);
end:
	if (bResult==FALSE) free(pCab);
	CloseHandle(hFile);
	return bResult;
}

LRESULT CALLBACK SrchEditSubProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage) {
	case WM_KEYDOWN:
		if (wParam==VK_RETURN) {
			StartSearch();
			return 0;
		}
	case WM_CHAR:
		if (wParam=='\r') {
			return 0;
		}
	}
	return CallWindowProc(OldEditProc,hWnd,iMessage,wParam,lParam);
}

void StartSearch()
{
	HTREEITEM hItem,hCabNode,hRoot;
	int Range;
	TCHAR szStatus[64];
	int parent;

	Range=SendMessage(hSrchCombo,CB_GETCURSEL,0,0);
	if (GetWindowTextLength(hSrchEdit)==0) {
		return;
	}

	TotalFind=0;
	hItem=TreeView_GetSelection(hTree);
	ListView_DeleteAllItems(hSrchList);
	switch (Range) {
	case 0:
		for (hRoot=TreeView_GetRoot(hTree);hRoot;) {
			if (SearchCabinet(hRoot)==FALSE) break;
			hRoot=TreeView_GetNextSibling(hTree,hRoot);
		}
		break;
	case 1:
		// 선택이 없으면 첫 번째 캐비넷을 대상으로 한다. 그것도 없으면 할 수 없고
		if (hItem == NULL) hItem=TreeView_GetRoot(hTree);
		if (hItem != NULL) {
			hCabNode=GetCabinetNode(hItem);
			SearchCabinet(hCabNode);
		}
		break;
	case 2:
		// 캐비넷에 대해서는 이 옵션을 쓸 수 없음. 폴더가 선택된 경우 상위의 볼륨을 먼저 찾음
		if (hItem!=NULL && TreeView_GetDepth(hTree,hItem) != 0) {
			hItem=FindVolNode(hItem);
			SearchVolume(hItem);
		} else {
			MessageBox(hWndMain,"검색할 볼륨을 선택해 주십시오.","알림",MB_OK);
		}
		break;
	case 3:
		if (hItem!=NULL && TreeView_GetDepth(hTree,hItem) >= 2) {
			parent=TreeView_GetParam(hTree,hItem);
			SearchVolume(FindVolNode(hItem),parent);
		} else {
			MessageBox(hWndMain,"검색할 폴더를 선택해 주십시오.","알림",MB_OK);
		}
		break;
	}

	if (TotalFind==0) {
		lstrcpy(szStatus,"찾는 파일이 없습니다.");
	} else {
		wsprintf(szStatus,"총 %d 개가 검색되었습니다",TotalFind);
	}
	SendMessage(hState, SB_SETTEXT, 0, (LPARAM)szStatus);
}

BOOL SearchCabinet(HTREEITEM hCabNode)
{
	HTREEITEM hChild;

	hChild=TreeView_GetChild(hTree,hCabNode);
	for (;hChild;hChild=TreeView_GetNextSibling(hTree,hChild)) {
		if (SearchVolume(hChild)==FALSE) {
			return FALSE;
		}
	}
	return TRUE;
}

// 볼륨을 검색한다. parent가 주어지면 이 아래의 자식만 검색한다.
// 중간에 취소할 때는 FALSE를 리턴한다.
BOOL SearchVolume(HTREEITEM hVolNode,int parent/*=-1*/)
{
	int i,idx;
	HTREEITEM hCabNode;
	SCabinet *pCab;
	SVolume *pVol;
	SRecord *pRec;
	TCHAR SrchText[128];
	TCHAR Name[MAX_PATH];
	TCHAR FullPath[MAX_PATH*2];
	LVITEM LI;
	BOOL bUseWild,bMatch;
	int pid;

	GetWindowText(hSrchEdit,SrchText,128);
	strupr(SrchText);
	bUseWild=(strpbrk(SrchText,"*?")!=0);
	hCabNode=TreeView_GetParent(hTree,hVolNode);
	pCab=(SCabinet *)TreeView_GetParam(hTree,hCabNode);
	pVol=(SVolume *)TreeView_GetParam(hTree,hVolNode);
	// 0번 레코드는 볼륨 루트이므로 검색에서 제외
	for (i=1;i<pVol->nRecord;i++) {
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			if (MessageBox(hWndMain,"열심히 검색중인데 따분하지? 그만 집어치울까?","질문",MB_YESNO)==IDYES) {
				return FALSE;
			}
		}
		pRec=&pVol->arRecord[i];
		// 폴더가 지정되어 있는 경우 이 폴더의 후손만 검사한다. 선조중에 parent가 있는지 모두 검사해야 손자도 검색된다.
		if (parent != -1) {
			for (pid=pRec->Parent;pid!=0;) {
				if (pid == parent) break;
				// pid의 부모 찾기. pid의 첨자를 찾고 이 첨자 레코드의 Parent를 읽는다.
				pid=pVol->arRecord[GetIndexFromId(pVol,pid)].Parent;
			}
			// 루트 레코드에 도달할 때까지 parent가 없으면 후손이 아님
			if (pid == 0) continue;
		}
		if (pRec->stName.IsPtr[0]==':') {
			lstrcpy(Name,pRec->stName.pName);
		} else {
			lstrcpy(Name,pRec->Name);
		}
		strupr(Name);

		// 검색식에 *나 ?가 있으면 와일드 카드로 검색, 아니면 부분 문자열 검색
		if (bUseWild) {
			bMatch=IsMatch(pRec->Name,SrchText);
		} else {
			bMatch=(strstr(Name,SrchText)!=NULL);
		}
		if (bMatch) {
			LI.mask=LVIF_TEXT | LVIF_PARAM;
			LI.state=0;
			LI.stateMask=0;
			LI.iSubItem=0;
			LI.iItem=1000;
			LI.pszText=pCab->Name;
			LI.lParam=(LPARAM)hVolNode;
			idx=ListView_InsertItem(hSrchList,&LI);

			ListView_SetItemText(hSrchList,idx,1,pVol->Name);
			GetRecordFullPath(pVol,pRec->Id,FullPath);
			ListView_SetItemText(hSrchList,idx,2,FullPath);
			TotalFind++;
			wsprintf(FullPath,"(검색중:취소는 ESC) %d개 검색",TotalFind);
			SendMessage(hState, SB_SETTEXT, 0, (LPARAM)FullPath);
			// 검색 결과를 리스트 뷰에 즉시 보여 주되 결과가 많을 때는 느려지므로 매 100개마다 업데이트
			if (TotalFind < 100 || TotalFind%100==0) {
				UpdateWindow(hSrchList);
			}
		}
	}
	return TRUE;
}

// 볼륨내의 Id로 전체 경로를 구한다. 부모의 경로를 계속 앞쪽에 덧붙인다.
void GetRecordFullPath(SVolume *pVol,int Id,TCHAR *FullPath)
{
	TCHAR *pName;
	TCHAR pTemp[MAX_PATH*2];
	SRecord *pRec;

	lstrcpy(FullPath,"");
	for (;;) {
		pRec=&pVol->arRecord[GetIndexFromId(pVol,Id)];
		if (pRec->stName.IsPtr[0]==':') {
			pName=pRec->stName.pName;
		} else {
			pName=pRec->Name;
		}
		if (FullPath[0]==0) {
			lstrcpy(FullPath,pName);
		} else {
			lstrcpy(pTemp,FullPath);
			wsprintf(FullPath,"%s\\%s",pName,pTemp);
		}
		Id=pRec->Parent;
		if (Id == 0) break;
	}
}

// 볼륨 노드에서 경로에 해당하는 노드를 찾아 선택한다.
void GotoItemInTree(HTREEITEM hVolNode,TCHAR *FullPath)
{
	TCHAR *p,*p2,Text[MAX_PATH],Text2[MAX_PATH];
	HTREEITEM hChild,hChild2;
	SVolume *pVol;
	LVFINDINFO lfi;
	int listidx;

	pVol=(SVolume *)TreeView_GetParam(hTree,hVolNode);

	hChild=hVolNode;
	for (p=FullPath;*p;) {
		// \구분자를 기준으로 한 다음 노드 텍스트를 구한다.
		for (p2=Text;*p !='\\' && *p != 0;) {
			*p2++=*p++;
		}
		*p2=0;
		if (*p) p++;

		// 트리의 노드는 아직 차일드가 등록되지 않은 상태이므로 일단 노드를 확장한다. -> TVN_ITEMEXPANDING에서 노드가 읽혀진다.
		TreeView_Expand(hTree,hChild,TVE_EXPAND);
		// 차일드가 없으면 디렉토리 깊이까지는 검색이 완료된 것이다.
		hChild2=TreeView_GetChild(hTree,hChild);
		if (hChild2==NULL) break;
		// 텍스트와 일치하는 노드를 찾는다.
		for (;;) {
			TreeView_GetText(hTree,hChild2,Text2,MAX_PATH);
			if (lstrcmp(Text,Text2)==0) break;
			hChild2=TreeView_GetNextSibling(hTree,hChild2);
			if (hChild2==NULL) break;
		}
		if (hChild2==NULL) break;
		hChild=hChild2;
	}

	TreeView_SelectItem(hTree,hChild);
	ListView_SetItemState(hList,-1,0,LVIS_FOCUSED | LVIS_SELECTED);
	// 트리에서 차일드를 못찾은 경우 리스트 뷰에서 파일을 찾는다.
	if (hChild2==NULL) {
		lfi.flags=LVFI_STRING;
		lfi.psz=Text;
		lfi.vkDirection=VK_DOWN;
		listidx=ListView_FindItem(hList,-1,&lfi);
		ListView_SetItemState(hList,listidx,LVIS_FOCUSED | LVIS_SELECTED,
			LVIS_FOCUSED | LVIS_SELECTED);
		ListView_EnsureVisible(hList,listidx,FALSE);
	}
}

void CreateToolBar()
{
	TBBUTTON ToolBtn[]={
		{0,TM_NEW,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,0},
		{1,TM_OPEN,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,1},
		{2,TM_SAVE,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,2},
		{3,TM_CLOSE,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,3},
		{4,0,0,TBSTYLE_SEP,0,0},
		{4,TM_ADDVOL,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,4},
		{5,TM_DELVOL,TBSTATE_ENABLED,TBSTYLE_BUTTON,0,0,0,5},
	};
	TCHAR *szToolText="새캐비넷\0열기\0저장\0닫기\0볼륨추가\0볼륨삭제\0";
	
	if (hToolBar) {
		DestroyWindow(hToolBar);
	}

	hToolBar=CreateToolbarEx(hWndMain, WS_CHILD | WS_VISIBLE
		| TBSTYLE_TOOLTIPS | TBSTYLE_FLAT ,
		99, 6, g_hInst, IDB_TOOLBAR, ToolBtn, 7,
		24,22,24,22,sizeof(TBBUTTON));

	SendMessage(hToolBar,TB_ADDSTRING,NULL,(LPARAM)szToolText);
	SendMessage(hToolBar,TB_AUTOSIZE,0,0);
}

// 해당 노드의 모든 자식을 제거하고 설명 문자열만 남긴다.
void DeleteRecord(HTREEITEM hRecNode)
{
	int i,id;
	SVolume *pVol;
	int st=-1,ed;
	HTREEITEM hChild,hChild2;
	TVITEM Tv;
	TCHAR Cap[MAX_PATH];
	HTREEITEM hCabNode;
	SCabinet *pCab;
	SRecord *pRec;

	// 설명을 입력받는다.
	TreeView_GetText(hTree,hRecNode,Cap,MAX_PATH);
	lstrcat(Cap,":삭제됨");
	if (DialogBoxParam(g_hInst,MAKEINTRESOURCE(IDD_DLGREMARK),
		hWndMain,RemarkProc,(LPARAM)Cap)==IDCANCEL) {
		return;
	}

	pVol=FindVolume(hRecNode);
	id=TreeView_GetParam(hTree,hRecNode);
	DeleteDirNode(pVol,id);

	// 삭제할 범위를 선정한다. 할당된 메모리도 해제한다.
	for (i=1;i<pVol->nRecord;i++) {
		if (pVol->arRecord[i].Size == -1) {
			if (st==-1) st=i;
			ed=i;
			if (lstrcmp(pVol->arRecord[i].stName.IsPtr,":::")==0) {
				free(pVol->arRecord[i].stName.pName);
			}
		}
	}

	// 배열 이동 및 크기도 변경
	memmove(&pVol->arRecord[st],&pVol->arRecord[ed+1],(pVol->nRecord-ed-1)*sizeof(SRecord));
	pVol->nRecord-=(ed-st+1);

	// 트리의 노드도 제거한다. 리스트 뷰에 현재 출력되어 있는 항목도
	hChild=TreeView_GetChild(hTree,hRecNode);
	for (;hChild;) {
		hChild2=TreeView_GetNextSibling(hTree,hChild);
		TreeView_DeleteItem(hTree,hChild);
		hChild=hChild2;
	}
	ListView_DeleteAllItems(hList);

	// + 표시는 직접 제거해야 한다. 
	Tv.mask=TVIF_CHILDREN;
	Tv.hItem=hRecNode;
	Tv.cChildren=0;
	TreeView_SetItem(hTree,&Tv);

	// 캡션도 수정 - 트리 및 볼륨 모두에 대해
	TreeView_SetText(hTree,hRecNode,Cap);
	pRec=&pVol->arRecord[GetIndexFromId(pVol,id)];
	// 편집된 길이를 예측하기 어려우므로 무조건 동적할당한다.
	if (lstrcmp(pRec->stName.IsPtr,":::")==0) {
		pRec->stName.pName=(TCHAR *)realloc(pRec->stName.pName,lstrlen(Cap)+1);
		lstrcpy(pRec->stName.pName,Cap);
	} else {
		lstrcpy(pRec->stName.IsPtr,":::");
		pRec->stName.pName=(TCHAR *)malloc(lstrlen(Cap)+1);
		lstrcpy(pRec->stName.pName,Cap);
	}
	pRec->Attr |= FA_REMARK;

	hCabNode=GetCabinetNode(hRecNode);
	pCab=(SCabinet *)TreeView_GetParam(hTree,hCabNode);
	pCab->bModified=TRUE;
}

// pVol에서 id의 자식들을 모두 제거 표시한다.
void DeleteDirNode(SVolume *pVol,int id)
{
	int i;

	for (i=1;i<pVol->nRecord;i++) {
		if (pVol->arRecord[i].Parent == id) {
			if (pVol->arRecord[i].Attr & FA_DIRECTORY) {
				DeleteDirNode(pVol,i);
			}
			// 크기를 -1로 설정하여 삭제될 레코드임을 기록한다.
			// 속도 문제로 인해 일일이 지우지 않고 표시후 일괄 삭제한다.
			pVol->arRecord[i].Size=-1;
		}
	}
}

BOOL CALLBACK RemarkProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	static TCHAR *Cap;

	switch (iMessage) {
	case WM_INITDIALOG:
		Cap=(TCHAR *)lParam;
		SetDlgItemText(hDlg,IDC_EDREMARK,Cap);
		MoveToParentCenter(hDlg);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			GetDlgItemText(hDlg,IDC_EDREMARK,Cap,MAX_PATH);
			EndDialog(hDlg,IDOK);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg,IDCANCEL);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

// Id로부터 배열상의 첨자를 찾는다. 레코드의 일부가 지워질 경우 Id와 첨자가 불일치할 수 있으므로
// 첨자를 찾아서 사용해야 한다. todo : 다음에 이분 검색으로 바꿀 것
int GetIndexFromId(SVolume *pVol, int Id)
{
	int i;

	for (i=0;i<pVol->nRecord;i++) {
		if (pVol->arRecord[i].Id == Id) {
			return i;
		}
	}
	assert(!"배열상에 존재하지 않는 무효한 Id를 참조하고 있음");
	return 0;
}

void FreeVolume(SVolume *pVol)
{
	int i;
	SRecord *pRec;

	for (i=0;i<pVol->nRecord;i++) {
		pRec=&pVol->arRecord[i];
		if (lstrcmp(pRec->stName.IsPtr,":::")==0) {
			free(pRec->stName.pName);
		}
		if (pRec->PvFormat != PV_NONE) {
			free(pRec->PvPtr);
		}
	}
	free(pVol->arRecord);
	free(pVol);
}

BOOL CALLBACK PasswordProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	static TCHAR *Pass;

	switch (iMessage) {
	case WM_INITDIALOG:
		Pass=(TCHAR *)lParam;
		MoveToParentCenter(hDlg);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			GetDlgItemText(hDlg,IDC_EDPASSWORD,Pass,31);
			EndDialog(hDlg,IDOK);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg,IDCANCEL);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

BOOL CALLBACK AboutProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	RECT brt,crt;
	POINT pt;
	HWND hBtn;
	int x,y;

	switch (iMessage) {
	case WM_INITDIALOG:
		MoveToParentCenter(hDlg);
		SetTimer(hDlg,1,50,NULL);
		return TRUE;
	case WM_TIMER:
		hBtn=GetDlgItem(hDlg,IDNOOK);
		if (GetFocus()==hBtn) {
			SetFocus(GetDlgItem(hDlg,IDOK));
		}
		GetCursorPos(&pt);
		GetWindowRect(hBtn,&brt);
		// 마우스가 위로 올라오지 못하도록. 난수로 고른 좌표가 설사 겹치더라도 다음 타이머에서 처리하므로 큰 문제없음
		// 다음에는 좀 더 부드럽게 도망다니도록 해 볼 것 -> 생각보다 복잡할 것 같다.
		if (PtInRect(&brt,pt)) {
			GetClientRect(hDlg,&crt);
			x=rand()%(crt.right-(brt.right-brt.left));
			y=rand()%(crt.bottom-(brt.bottom-brt.top));
			SetWindowPos(hBtn,NULL,x,y,0,0,SWP_NOSIZE | SWP_NOZORDER);
			// IDOK와 겹치다 보면 가끔 깨지는 수가 있어 다시 그림
			InvalidateRect(hBtn,NULL,TRUE);
			UpdateWindow(hBtn);
		}
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			KillTimer(hDlg,1);
			EndDialog(hDlg,IDOK);
			return TRUE;
		}
		break;
	}
	return FALSE;
}
