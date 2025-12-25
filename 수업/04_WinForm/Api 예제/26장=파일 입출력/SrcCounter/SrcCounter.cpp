// 소스 줄 수를 세는 유틸리티. 폴더 백업 기능을 추가로 가지고 있다.
#include <windows.h>
#include <io.h>
#include <commctrl.h>
#include <direct.h>
#include "Util.h"
#include "SHReg.h"
#include "resource.h"

typedef int (*FIFCALLBACK)(TCHAR *, LPVOID);

// 함수 원형 선언
BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);
BOOL CALLBACK BackUpDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);
BOOL CALLBACK RegDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);
void StartCount(BOOL bCount);
int LineCount(TCHAR *path);
void SelectPrjType(void);
void Initialize(void);
void OnDestroy(void);
void OnSize(LPARAM lParam);
void OnDropFiles(WPARAM wParam, LPARAM lParam);
int CALLBACK MyCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
void SetEditor();
void OnBtnFind();
void FindInFiles(TCHAR *Path, TCHAR (*arExt)[10], BOOL bDeep, FIFCALLBACK pCallBack, LPVOID pCustom);
int OnFindFile(TCHAR *Path, LPVOID pCustom);
int OnSearchFile(TCHAR *Path, LPVOID pCustom);

// 전역 변수들
HINSTANCE g_hInst;
LPSTR lpszClass="SrcCounter";
TCHAR arExt[100][10];		// 검색 대상 확장자들
TCHAR ExFile[100][32];		// 백업 제외 파일
TCHAR ExFolder[100][32];		// 백업 제외 폴더
HWND hMainDlg, hBackDlg;	// 대화상자 핸들
HWND hList;					// 리스트 컨트롤의 핸들
int nTotalFile;				// 발견된 총 파일
int nTotalLine;				// 총 줄 수
int nTotalSize;				// 총 크기
TCHAR szTmp[MAX_PATH];		// 임시 버퍼
TCHAR szTmp2[MAX_PATH];		// 임시 버퍼 2
BOOL gAsc=TRUE;				// 정렬 순서
int lParamIdx;				// 검색된 파일의 일련번호. 정렬에 사용된다.

// 레지스트리 키
#define KEY "Software\\SangHyungSoft\\SrcCounter\\"

// 메인 함수에서는 대화상자를 띄우기만 한다.
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
		  ,LPSTR lpszCmdParam,int nCmdShow)
{
	g_hInst=hInstance;
	
	DialogBox(g_hInst,MAKEINTRESOURCE(IDD_DLGMAIN),NULL,MainDlgProc);
	return 0;
}

// 메인 대화상자 프로시저
BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage){
	// 콤보 박스에 항목을 등록하고 레지스트리에서 최후 검색 폴더를 읽어온다.
	case WM_INITDIALOG:
		hMainDlg=hDlg;
		Initialize();
		return TRUE;
	// 명령 버튼과 콤보 박스의 통지 메시지 처리
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		// 찾아보기
		case IDC_BTN_BROWSE:
			if (BrowseFolder(hMainDlg,NULL,NULL,szTmp) == TRUE) {
				SetDlgItemText(hDlg,IDC_PRJFOLDER,szTmp);
			}
			return TRUE;
		// 폴더 등록 대화상자 호출
		case IDC_REGISTER:
			DialogBox(g_hInst,MAKEINTRESOURCE(IDD_DLGREGFOLDER),hMainDlg,(DLGPROC)RegDlgProc);
			return TRUE;
		// 폴더 백업 대화상자 호출
		case IDC_BACKUP:
			DialogBox(g_hInst,MAKEINTRESOURCE(IDD_DLGBACK),hMainDlg,(DLGPROC)BackUpDlgProc);
			return TRUE;
		// 줄 수 세기
		case IDC_BTN_COUNT:
			StartCount(TRUE);
			return TRUE;
		case IDC_BTN_LIST:
			StartCount(FALSE);
			return TRUE;
		// 편집기 선택
		case IDC_BTNSETEDIT:
			SetEditor();
			return TRUE;
		// 프로젝트 유형 변경
		case IDC_CBPRJTYPE:
			switch (HIWORD(wParam)) {
			case CBN_SELCHANGE:
				SelectPrjType();
				break;
			}
			return TRUE;
		case IDCANCEL:
			OnDestroy();
			EndDialog(hMainDlg,0);
			return TRUE;
		case IDC_BTNFIND:
			OnBtnFind();
			return TRUE;
		}
		break;
	case WM_NOTIFY:
		LPNMHDR hdr;
		LPNMLISTVIEW nlv;
		hdr=(LPNMHDR)lParam;
		nlv=(LPNMLISTVIEW)lParam;
		LPNMITEMACTIVATE nia;
		TCHAR fname[128];
		TCHAR path[MAX_PATH];

		if (hdr->hwndFrom == hList) {
			switch (hdr->code) {
			// 헤더를 클릭하면 소트한다.
			case LVN_COLUMNCLICK:
				ListView_SortItems(hList, MyCompare, nlv->iSubItem);
				gAsc=!gAsc;
				break;
			// 리스트의 항목을 더블 클릭하면 등록된 편집기로 파일을 열어준다.
			case NM_DBLCLK:
				nia = (LPNMITEMACTIVATE)lParam;
				if (nia->iItem != -1) {
					ListView_GetItemText(hList,nia->iItem,0,fname,128);
					GetDlgItemText(hDlg,IDC_PRJFOLDER,path,MAX_PATH);
					GetDlgItemText(hMainDlg,IDC_EDEDITOR,szTmp,MAX_PATH);
					wsprintf(szTmp2, "%s \"%s\\%s\"",szTmp,path,fname);
					WinExec(szTmp2,SW_SHOWNORMAL);
				}
				return TRUE;
			}
		}
		break;
	case WM_SIZE:
		OnSize(lParam);
		break;
	case WM_DROPFILES:
		OnDropFiles(wParam, lParam);
		break;
	}
	return FALSE;
}

// 리스트 뷰의 컬럼을 초기화한다. 1=소스 줄 수, 2=문자열 검색용
void InitListColumn(int n)
{
	LVCOLUMN COL;
	ListView_DeleteAllItems(hList);
	int i,count;

	// 모든 컬럼을 지운다.
	count=Header_GetItemCount(ListView_GetHeader(hList));
	for (i=0;i<count;i++) {
		ListView_DeleteColumn(hList,0);
	}

	if (n == 1) {
		COL.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT |LVCF_SUBITEM;
		COL.fmt=LVCFMT_LEFT;
		COL.cx=300;
		COL.pszText="파일";
		COL.iSubItem=0;
		ListView_InsertColumn(hList,0,&COL);

		COL.fmt=LVCFMT_RIGHT;
		COL.cx=120;
		COL.pszText="수정날짜";
		COL.iSubItem=1;
		ListView_InsertColumn(hList,1,&COL);

		COL.fmt=LVCFMT_RIGHT;
		COL.cx=80;
		COL.pszText="크기(byte)";
		COL.iSubItem=2;
		ListView_InsertColumn(hList,2,&COL);

		COL.fmt=LVCFMT_RIGHT;
		COL.cx=60;
		COL.pszText="줄";
		COL.iSubItem=3;
		ListView_InsertColumn(hList,3,&COL);
	} else {
		COL.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT |LVCF_SUBITEM;
		COL.fmt=LVCFMT_LEFT;
		COL.cx=250;
		COL.pszText="파일";
		COL.iSubItem=0;
		ListView_InsertColumn(hList,0,&COL);

		COL.fmt=LVCFMT_RIGHT;
		COL.cx=80;
		COL.pszText="위치";
		COL.iSubItem=1;
		ListView_InsertColumn(hList,1,&COL);

		COL.fmt=LVCFMT_LEFT;
		COL.cx=400;
		COL.pszText="텍스트";
		COL.iSubItem=2;
		ListView_InsertColumn(hList,2,&COL);
	}
}

// 초기화
void Initialize()
{
	InitCommonControls();

	// 대화 상자의 아이콘을 등록한다.
	SendMessage(hMainDlg,WM_SETICON,(WPARAM)ICON_BIG,
		(LPARAM)LoadIcon(g_hInst,MAKEINTRESOURCE(IDI_ICON1)));

	// 리스트 컨트롤의 핸들을 구해 전역 변수에 저장해 놓는다.
	hList=GetDlgItem(hMainDlg,IDC_LIST1);

	// 콤보 박스의 항목 초기화
	SendDlgItemMessage(hMainDlg,IDC_CBPRJTYPE,CB_ADDSTRING,0,(LPARAM)"모두 해제");
	SendDlgItemMessage(hMainDlg,IDC_CBPRJTYPE,CB_ADDSTRING,0,(LPARAM)"C/C++");
	SendDlgItemMessage(hMainDlg,IDC_CBPRJTYPE,CB_ADDSTRING,0,(LPARAM)"Pascal");
	SendDlgItemMessage(hMainDlg,IDC_CBPRJTYPE,CB_ADDSTRING,0,(LPARAM)"Assembly");
	SendDlgItemMessage(hMainDlg,IDC_CBPRJTYPE,CB_ADDSTRING,0,(LPARAM)"Internet");
	SendDlgItemMessage(hMainDlg,IDC_CBPRJTYPE,CB_SETCURSEL,1,0);

	// 최후 검색 폴더와 확장자를 읽어온다.
	SHRegReadString(HKEY_CURRENT_USER,KEY"Option","Folder","폴더를 입력하십시요",szTmp,256);
	SetDlgItemText(hMainDlg,IDC_PRJFOLDER, szTmp);
	SHRegReadString(HKEY_CURRENT_USER,KEY"Option","Extension","cpp;c;h;hpp",szTmp,256);
	SetDlgItemText(hMainDlg,IDC_EXTENSION, szTmp);
	SHRegReadString(HKEY_CURRENT_USER,KEY"Option","Search","",szTmp,256);
	SetDlgItemText(hMainDlg,IDC_EDFIND, szTmp);

	// 리스트 뷰에 컬럼 생성
	InitListColumn(1);

	// 한줄 모두 선택되도록 할 것
	ListView_SetExtendedListViewStyle(hList,LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);

	// 등록된 폴더 목록을 읽어와 콤보 박스에 채운다.
	int Count,i;
	TCHAR szKey[20];
	Count=SHRegReadInt(HKEY_CURRENT_USER,KEY"RegFolder","RegNum",0);

	for (i=0;i<Count;i++) {
		wsprintf(szKey,"Reg%d",i);
		SHRegReadString(HKEY_CURRENT_USER,KEY"RegFolder",szKey,"",szTmp,MAX_PATH);
		SendDlgItemMessage(hMainDlg,IDC_PRJFOLDER,CB_ADDSTRING,0,(LPARAM)szTmp);
	}

	// 소스 편집기 설정을 읽어온다.
	SHRegReadString(HKEY_CURRENT_USER,KEY"Option","Editor","NotePad.EXE",szTmp,MAX_PATH);
	SetDlgItemText(hMainDlg,IDC_EDEDITOR,szTmp);

	// 최후 실행 위치로 이동한다.
	RECT rt;
	rt.left=SHRegReadInt(HKEY_CURRENT_USER,KEY"Position","Left",100);
	rt.top=SHRegReadInt(HKEY_CURRENT_USER,KEY"Position","Top",100);
	rt.right=SHRegReadInt(HKEY_CURRENT_USER,KEY"Position","Right",730);
	rt.bottom=SHRegReadInt(HKEY_CURRENT_USER,KEY"Position","Bottom",500);
	MoveWindow(hMainDlg, rt.left, rt.top, rt.right-rt.left,rt.bottom-rt.top, TRUE);

	// 파일 드롭 허가
	DragAcceptFiles(hMainDlg,TRUE);
}

// 프로그램을 종료할 때 정보를 저장한다.
void OnDestroy(void)
{
	RECT rt;

	// 최후로 조사한 폴더, 확장자를 레지스트리에 기록한다.
	GetDlgItemText(hMainDlg,IDC_PRJFOLDER,szTmp,256);
	SHRegWriteString(HKEY_CURRENT_USER,KEY"Option","Folder",szTmp);
	GetDlgItemText(hMainDlg,IDC_EXTENSION,szTmp,256);
	SHRegWriteString(HKEY_CURRENT_USER,KEY"Option","Extension",szTmp);
	GetDlgItemText(hMainDlg,IDC_EDFIND,szTmp,256);
	SHRegWriteString(HKEY_CURRENT_USER,KEY"Option","Search",szTmp);

	// 최후 실행 위치 저장
	GetWindowRect(hMainDlg, &rt);
	SHRegWriteInt(HKEY_CURRENT_USER,KEY"Position","Left",rt.left);
	SHRegWriteInt(HKEY_CURRENT_USER,KEY"Position","Top",rt.top);
	SHRegWriteInt(HKEY_CURRENT_USER,KEY"Position","Right",rt.right);
	SHRegWriteInt(HKEY_CURRENT_USER,KEY"Position","Bottom",rt.bottom);
}

// 리스트 컨트롤과 검색 결과 스태틱의 위치를 재배치한다.
void OnSize(LPARAM lParam)
{
	int sx,sy;
	sx=LOWORD(lParam);
	sy=HIWORD(lParam);

	MoveWindow(hList,10,130,sx-20,sy-160,TRUE);
	MoveWindow(GetDlgItem(hMainDlg,IDC_RESULT),10,sy-20,sx-20,20,TRUE);
}

// 확장자들을 배열에 분리해 포함시킨다.
int InitExtension()
{
	int cExt=0;
	TCHAR *p, *e;

	// 검색 대상 확장자 선택
	cExt=0;
	GetDlgItemText(hMainDlg,IDC_EXTENSION,szTmp,MAX_PATH);
	p=szTmp;
	while (p < szTmp+lstrlen(szTmp)) {
		e=arExt[cExt];
		*e++='.';		// 확장자앞에는 항상 .이 있어야 한다.
		while (*p!=';' && *p!=0) 
			*e++=*p++;
		*e=0;
		cExt++;
		p++;
	}
	arExt[cExt][0]=0;

	return cExt;
}

// 지금 조사 버튼
void StartCount(BOOL bCount)
{
	TCHAR Mes[256];
	TCHAR path[MAX_PATH];
	int cExt;

	cExt=InitExtension();

	// 확장자가 하나도 선택되지 않았을 경우의 에러 처리
	if (cExt==0) {
		MessageBox(hMainDlg,"최소한 하나 이상의 확장자를 선택하셔야 합니다","바보야",MB_OK);
		return;
	}

	// 검색 대상 폴더가 유효한지 점검한다.
	GetDlgItemText(hMainDlg, IDC_PRJFOLDER,path,MAX_PATH);
	if (_access(path,0)!=0) {
		MessageBox(hMainDlg,"경로명이 틀렸습니다","바보야",MB_OK);
		return;
	}

	// 변수를 초기화하고 리스트 박스를 비운 후 파일을 검색한다.
	nTotalFile=0;
	nTotalLine=0;
	nTotalSize=0;

	InitListColumn(1);
	lParamIdx=0;

	SetCursor(LoadCursor(NULL,IDC_WAIT));
	FindInFiles(path,arExt,TRUE,OnFindFile,(TCHAR *)bCount);
	SetCursor(LoadCursor(NULL,IDC_ARROW));

	// 검색 결과를 리스트 박스에 출력한다.
	wsprintf(Mes,"검색 결과 : 총 %d 개 파일 %d줄. 총 크기=%d바이트",nTotalFile,nTotalLine,nTotalSize);
	SetDlgItemText(hMainDlg,IDC_RESULT,Mes);
}

// 문자열 검색
void OnBtnFind()
{
	TCHAR path[MAX_PATH];
	int cExt;
	TCHAR sSrch[255];

	GetDlgItemText(hMainDlg, IDC_EDFIND, sSrch, 255);
	if (lstrlen(sSrch) == 0) {
		MessageBox(hMainDlg,"검색할 단어를 입력해 주십시오","알림",MB_OK);
		return;
	}

	cExt=InitExtension();

	// 확장자가 하나도 선택되지 않았을 경우의 에러 처리
	if (cExt==0) {
		MessageBox(hMainDlg,"최소한 하나 이상의 확장자를 선택하셔야 합니다","바보야",MB_OK);
		return;
	}

	// 검색 대상 폴더가 유효한지 점검한다.
	GetDlgItemText(hMainDlg, IDC_PRJFOLDER,path,MAX_PATH);
	if (_access(path,0)!=0) {
		MessageBox(hMainDlg,"경로명이 틀렸습니다","바보야",MB_OK);
		return;
	}

	InitListColumn(2);
	lParamIdx=0;

	SetCursor(LoadCursor(NULL,IDC_WAIT));
	nTotalFile = 0;
	FindInFiles(path,arExt,TRUE,OnSearchFile,(TCHAR *)sSrch);

	wsprintf(szTmp,"총 %d 개의 문자열이 검색되었습니다.", nTotalFile);
	SetDlgItemText(hMainDlg,IDC_RESULT,szTmp);
	SetCursor(LoadCursor(NULL,IDC_ARROW));

	// 검색 결과를 리스트 박스에 출력한다.
//	wsprintf(Mes,"검색 결과 : 총 %d 개 파일 %d줄. 총 크기=%d바이트",nTotalFile,nTotalLine,nTotalSize);
//	SetDlgItemText(hMainDlg,IDC_RESULT,Mes);
}

// szBuf 버퍼에서 ptr 포인터가 가리키는 지점이 몇번째 줄인지 조사한다.
int GetLineNumAndText(TCHAR *szBuf, TCHAR *ptr, TCHAR *Text)
{
	TCHAR *p=szBuf;
	TCHAR *t=Text;
	int line=1;

	while (p != ptr) {
		if (*p == '\n') {
			line++;
		}
		p++;
	}

	// 버퍼 선두 또는 행 처럼으로 이동한다.
	while ((p != szBuf-1) && (*p != '\r') && (*p != '\n')) p--;
	p++;

	// 줄 끝까지 또는 1024바이트까지 문자열 복사
	while ((*p != 0) && (*p != '\r') && (*p != '\n') && (t-Text < 1024)) {
		if (*p == '\t') {
			*t++=' ';
			*t++=' ';
			*t++=' ';
			*t=' ';
		} else {
			*t=*p;
		}
		t++;
		p++;
	}
	*t=0;

	return line;
}

int OnSearchFile(TCHAR *Path, LPVOID pCustom)
{
	HANDLE hFile;
	DWORD size, dwRead;
	TCHAR *buf, *pbuf;
	TCHAR *sSrch=(TCHAR *)pCustom;
	TCHAR *p;
	int line;
	TCHAR Text[1024];
	int nDirOffset;
	LVITEM LI;
	int idx;

	// 검색디렉토리 경로의 길이를 구해 놓는다. 이 길이는 검색된 파일명을 출력할 때 폴더 경로를
	// 제외시키기 위해 사용된다.예를 들어 c:\Src\file.cpp가 검색되었다면 file.cpp만 출력한다.
	GetDlgItemText(hMainDlg, IDC_PRJFOLDER,szTmp,MAX_PATH);
	nDirOffset=lstrlen(szTmp)+1;

	hFile=CreateFile(Path,GENERIC_READ,0,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile==INVALID_HANDLE_VALUE) {
		return -1;
	}

	// 검색중인 파일 보여 주기
	wsprintf(szTmp,"검색 중 : %s", Path);
	SetDlgItemText(hMainDlg,IDC_RESULT,szTmp);

	size=GetFileSize(hFile,NULL);
	buf=(TCHAR *)malloc(size+1);
	ReadFile(hFile,buf,size,&dwRead,NULL);
	buf[size]=0;

	for (pbuf=buf;;) {
		p=strstr(pbuf,sSrch);
		if (p == NULL)
			break;
		nTotalFile++;
		line=GetLineNumAndText(buf,p,Text);
		pbuf=p+lstrlen(sSrch);

		// 파일명 출력. 단 검색 폴더 이후부터만 출력한다. 
		LI.mask=LVIF_TEXT | LVIF_PARAM;
		LI.state=0;
		LI.stateMask=0;
		LI.iSubItem=0;
		LI.iItem=ListView_GetItemCount(hList);
		LI.pszText=Path+nDirOffset;
		LI.lParam=(LPARAM)lParamIdx++;		// 일련 번호를 부여한다.
		idx=ListView_InsertItem(hList,&LI);

		// 검색된 줄 수 출력
		itoa(line,szTmp,10);
		ListView_SetItemText(hList,idx,1,szTmp);

		// 샘플 텍스트 출력
		ListView_SetItemText(hList,idx,2,Text);

		// 즉시 출력
		UpdateWindow(hList);
	}

	free(buf);
	CloseHandle(hFile);
	return 0;
}

// 찾은 파일에 대한 콜백 함수. 파일명을 출력하고 줄 수를 세어 준다.
int OnFindFile(TCHAR *Path, LPVOID pCustom)
{
	int cLine;
	int idx;
	LVITEM LI;
	TCHAR sLine[20];
	SYSTEMTIME st;
	FILETIME lf;
	BOOL bCount=(BOOL)pCustom;
	HANDLE hFile;
	FILETIME LastWrite;
	DWORD FileSize;
	int nDirOffset;

	// 검색디렉토리 경로의 길이를 구해 놓는다. 이 길이는 검색된 파일명을 출력할 때 폴더 경로를
	// 제외시키기 위해 사용된다.예를 들어 c:\Src\file.cpp가 검색되었다면 file.cpp만 출력한다.
	GetDlgItemText(hMainDlg, IDC_PRJFOLDER,szTmp,MAX_PATH);
	nDirOffset=lstrlen(szTmp)+1;

	hFile=CreateFile(Path,GENERIC_READ,0,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile==INVALID_HANDLE_VALUE) {
		return -1;
	}

	// 파일명 출력. 단 검색 폴더 이후부터만 출력한다. 
	LI.mask=LVIF_TEXT | LVIF_PARAM;
	LI.state=0;
	LI.stateMask=0;
	LI.iSubItem=0;
	LI.iItem=ListView_GetItemCount(hList);
	LI.pszText=Path+nDirOffset;
	LI.lParam=(LPARAM)lParamIdx++;		// 일련 번호를 부여한다.
	idx=ListView_InsertItem(hList,&LI);

	// 최후 수정 날짜 출력
	GetFileTime(hFile,NULL,NULL,&LastWrite);
	FileTimeToLocalFileTime(&LastWrite, &lf);
	FileTimeToSystemTime(&lf, &st);
	wsprintf(szTmp, "%4d-%02d-%02d일 %02d:%02d",st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);
	ListView_SetItemText(hList,idx,1,szTmp);

	// 파일 크기 출력
	FileSize=GetFileSize(hFile,NULL);
	itoa(FileSize,sLine,10);
	ListView_SetItemText(hList,idx,2,sLine);
	CloseHandle(hFile);

	// 검색된 파일의 줄 수를 센다.
	if (bCount == TRUE) {
		cLine=LineCount(Path);
		itoa(cLine,sLine,10);
	} else {
		lstrcpy(sLine,"?");
	}
	ListView_SetItemText(hList,idx,3,sLine);

	nTotalFile++;
	nTotalLine+=cLine;
	nTotalSize+=FileSize;

	return 0;
}

// Path 폴더에서 arExt 배열에 있는 확장자의 파일들을 검색하여 콜백 함수를 호출해 준다.
// 파일 검색을 위한 일반적인 용도로 사용하기 위해 재사용성을 고려하여 작성하였다.
void FindInFiles(TCHAR *Path, TCHAR (*arExt)[10], BOOL bDeep, FIFCALLBACK pCallBack, LPVOID pCustom)
{
	TCHAR SrchPath[MAX_PATH];
	TCHAR szFinded[MAX_PATH];
	WIN32_FIND_DATA wfd;
	HANDLE hSrch;
	BOOL nResult=TRUE;
	TCHAR ext[_MAX_EXT];
	int e;

	// 경로명에 *.*를 붙여 일단 모든 파일을 다 찾도록 한다.
	lstrcpy(SrchPath, Path);
	lstrcat(SrchPath, "\\*.*");
	hSrch=FindFirstFile(SrchPath,&wfd);
	if (hSrch == INVALID_HANDLE_VALUE)
		return;

	// 더 이상 발견되는 파일이 없을 때까지
	while (nResult) {
		wsprintf(szFinded,"%s\\%s",Path,wfd.cFileName);
		// 서브 디렉토리일 경우 디렉토리 안까지 검색한다.
		// 단 현재 디렉토리(.)와 부모 디렉토리(..)은 검색에서 제외한다.
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && wfd.cFileName[0]!='.') {
			FindInFiles(szFinded,arExt,bDeep,pCallBack,pCustom);
		}

		// 발견된 파일의 확장자를 검색 대상 확장자와 비교해 본다.
		e=0;
		_splitpath(wfd.cFileName,NULL,NULL,NULL,ext);
		while (arExt[e][0]!=0) {
			if (stricmp(ext,arExt[e])==0) {
				pCallBack(szFinded,pCustom);
			}
			e++;
		}
		nResult=FindNextFile(hSrch,&wfd);
	}
	FindClose(hSrch);
}

// 지정 경로에 있는 확장자의 줄 수를 센다.
void SourceCount(TCHAR *path,BOOL bCount)
{

/*	WIN32_FIND_DATA wfd;
	HANDLE hSrch;
	BOOL nResult=TRUE;
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[MAX_PATH];
	TCHAR fname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];
	TCHAR newpath[MAX_PATH];
	int cLine;
	int e, idx;
	LVITEM LI;
	TCHAR sLine[20];
	SYSTEMTIME st;
	FILETIME lf;
	int nDirOffset;

	// 검색디렉토리 경로의 길이를 구해 놓는다. 이 길이는 검색된 파일명을 출력할 때 폴더 경로를
	// 제외시키기 위해 사용된다.예를 들어 c:\Src\file.cpp가 검색되었다면 file.cpp만 출력한다.
	GetDlgItemText(hMainDlg, IDC_PRJFOLDER,szTmp,MAX_PATH);
	nDirOffset=lstrlen(szTmp)+1;

	hSrch=FindFirstFile(path,&wfd);
	// 더 이상 발견되는 파일이 없을 때까지
	while (nResult) {
		// 서브 디렉토리일 경우 디렉토리 안까지 검색한다.
		// 단 현재 디렉토리(.)와 부모 디렉토리(..)은 검색에서 제외한다.
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY &&
			wfd.cFileName[0]!='.') {
			_splitpath(path,drive,dir,fname,ext);
			wsprintf(newpath,"%s%s%s\\*.*",drive,dir,wfd.cFileName);
			// 재귀 호출
			SourceCount(newpath,bCount);
		}
		_splitpath(wfd.cFileName,drive,dir,fname,ext);
		e=0;

		// 발견된 파일의 확장자를 검색 대상 확장자와 비교해 본다.
		while (arExt[e][0]!=0) {
			if (stricmp(ext,arExt[e])==0) {
				// 검색 폴더 뒤의 "\*.*"를 떼 내고 파일명을 뒤에 붙인다.
				lstrcpy(newpath,path);
				newpath[lstrlen(newpath)-3]=0;
				strcat(newpath,wfd.cFileName);

				// 파일명 출력. 단 검색 폴더 이후부터만 출력한다. 
				LI.mask=LVIF_TEXT | LVIF_PARAM;
				LI.state=0;
				LI.stateMask=0;
				LI.iSubItem=0;
				LI.iItem=ListView_GetItemCount(hList);
				LI.pszText=newpath+nDirOffset;
				LI.lParam=(LPARAM)lParamIdx++;		// 일련 번호를 부여한다.
				idx=ListView_InsertItem(hList,&LI);

				// 최후 수정 날짜 출력
				FileTimeToLocalFileTime(&wfd.ftLastWriteTime, &lf);
				FileTimeToSystemTime(&lf, &st);
				wsprintf(szTmp, "%4d-%02d-%02d일 %02d:%02d",st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);
				ListView_SetItemText(hList,idx,1,szTmp);

				// 파일 크기 출력
				itoa(wfd.nFileSizeLow,sLine,10);
				ListView_SetItemText(hList,idx,2,sLine);

				// 검색된 파일의 줄 수를 센다.
				if (bCount == TRUE) {
					cLine=LineCount(newpath);
					itoa(cLine,sLine,10);
				} else {
					lstrcpy(sLine,"?");
				}
				ListView_SetItemText(hList,idx,3,sLine);

				nTotalFile++;
				nTotalLine+=cLine;
				nTotalSize+=wfd.nFileSizeLow;
			}
			e++;
		}
		nResult=FindNextFile(hSrch,&wfd);
	}
	FindClose(hSrch);*/
}

// 지정한 파일의 줄 수를 계산한다.
int LineCount(TCHAR *path)
{
	HANDLE hFile;
	DWORD Size;
	DWORD dwRead;
	int Count=0;
	TCHAR *buf;
	int i;

	hFile=CreateFile(path,GENERIC_READ,0,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	Size=GetFileSize(hFile,NULL);
	if (Size==0xFFFFFFFF || Size==0)  {
		CloseHandle(hFile);
		return 0;
	}
	buf=(TCHAR *)malloc(Size);
	ReadFile(hFile,buf,Size,&dwRead,NULL);
	
	// Enter코드의 개수를 센다.
	for (i=0;i<(int)dwRead;i++) {
		if (buf[i]=='\n') Count++;
	}
	free(buf);
	CloseHandle(hFile);
	return Count+1;
}

// 콤보 박스에서 선택한 프로젝트 유형에 맞는 확장자를 선택한다.
void SelectPrjType(void)
{
	int Type;
	Type=SendDlgItemMessage(hMainDlg,IDC_CBPRJTYPE,CB_GETCURSEL,0,0);

	// 프로젝트 타입에 따라 확장자를 입력한다.
	switch (Type) {
	case 0:
		SetDlgItemText(hMainDlg,IDC_EXTENSION, "");
		break;
	case 1:
		SetDlgItemText(hMainDlg,IDC_EXTENSION, "cpp;c;h;hpp");
		break;
	case 2:
		SetDlgItemText(hMainDlg,IDC_EXTENSION, "pas");
		break;
	case 3:
		SetDlgItemText(hMainDlg,IDC_EXTENSION, "asm");
		break;
	case 4:
		SetDlgItemText(hMainDlg,IDC_EXTENSION, "html;htm;asp;php");
		break;
	}
}

// 소트를 위한 정렬 함수
int CALLBACK MyCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	int Result;
	int idx1, idx2;
	TCHAR str1[255],str2[255];
	LVFINDINFO fi;

	// lParam1으로부터 첫번째 항목의 인덱스를 찾음
	fi.flags=LVFI_PARAM;
	fi.lParam=lParam1;
	fi.vkDirection=VK_DOWN;
	idx1=ListView_FindItem(hList,-1,&fi);

	// lParam2로부터 두번째 항목의 인덱스를 찾음
	fi.lParam=lParam2;
	idx2=ListView_FindItem(hList,-1,&fi);

	// 두 항목의 비교 텍스트를 구한다.
	ListView_GetItemText(hList, idx1, lParamSort, str1, 255);
	ListView_GetItemText(hList, idx2, lParamSort, str2, 255);

	// 파일명, 날짜는 문자열로 비교하고 크기와 줄수는 정수로 비교한다.
	if (lParamSort <= 1 ) {
		Result=lstrcmp(str1,str2);
	} else {
		Result=atoi(str1)-atoi(str2);
	}

	if (gAsc==TRUE) {
		return Result;
	} else {
		return -Result;
	}
}

// 소스 편집기를 등록한다.
void SetEditor()
{
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH]="";

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner=hMainDlg;
	OFN.lpstrFilter="실행 파일(*.exe)\0*.exe\0";
	OFN.lpstrFile=lpstrFile;
	OFN.nMaxFile=256;
	if (GetOpenFileName(&OFN)!=0) {
		SetDlgItemText(hMainDlg,IDC_EDEDITOR,OFN.lpstrFile);
		SHRegWriteString(HKEY_CURRENT_USER,KEY"Option","Editor",OFN.lpstrFile);
	}
}

//////////////////////////////////////////////////////////////////////////////////
// 폴더 백업 대화상자 루틴

// Src 폴더의 모든 파일과 서브 디렉토리를 Dest로 복사한다. 
// XCopy(Dir1, Dir2)형태로 사용한다.
BOOL XBackup(TCHAR *Src, TCHAR *Dest)
{
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	BOOL bResult=TRUE;
	TCHAR fname[MAX_PATH];
	TCHAR WildCard[MAX_PATH];
	TCHAR SrcFile[MAX_PATH];
	TCHAR DestFile[MAX_PATH];
	int e;
	BOOL bExclude;
	TCHAR ext[_MAX_EXT];

	lstrcpy(WildCard,Src);
	strcat(WildCard, "\\*.*");
	CreateDirectory(Dest,NULL);
	hSrch=FindFirstFile(WildCard,&wfd);
	if (hSrch == INVALID_HANDLE_VALUE) 
		return FALSE;
	while (bResult) {
		wsprintf(fname,"%s\\%s",Src,wfd.cFileName);
		// 서브 디렉토리가 발견되면 서브 디렉토리를 복사한다.
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (wfd.cFileName[0]!='.') {
				// 백업 제외 폴더인지 조사한다.
				e=0;
				bExclude=FALSE;
				while (ExFolder[e][0] != 0) {
					if (stricmp(ExFolder[e],wfd.cFileName) == 0) {
						bExclude=TRUE;
						break;
					}
					e++;
				}

				// 백업 제외 폴더가 아니면 재귀 호출을 한다.
				if (bExclude == FALSE) {
					wsprintf(SrcFile,"%s\\%s",Src,wfd.cFileName);
					wsprintf(DestFile,"%s\\%s",Dest,wfd.cFileName);
					XBackup(SrcFile, DestFile);
				}
			}
		} else {
			// 백업 제외 파일인지 조사한다.
			_splitpath(wfd.cFileName,NULL,NULL,NULL,ext);
			e=0;
			bExclude=FALSE;
			while (ExFile[e][0] != 0) {
				if (stricmp(ExFile[e],ext) == 0) {
					bExclude=TRUE;
					break;
				}
				e++;
			}

			// 백업 제외 파일이 아니면 백업을 한다.
			if (bExclude == FALSE) {
				wsprintf(DestFile,"%s\\%s",Dest,wfd.cFileName);
				wsprintf(szTmp,"파일 복사중 : %s",fname);
				SetDlgItemText(hBackDlg,IDC_STATUS,szTmp);
				CopyFile(fname,DestFile,FALSE);
			}
		}
		bResult=FindNextFile(hSrch,&wfd);
	}
	FindClose(hSrch);
	return TRUE;
}

// 백업 대화상자 프로시저
BOOL CALLBACK BackUpDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	TCHAR szSrc[MAX_PATH], szDest[MAX_PATH];
	TCHAR *p, *e;
	int cExt;

	switch (iMessage) {
	case WM_INITDIALOG:
		hBackDlg=hDlg;
		// 메인 대화상자의 대상 폴더가 백업 소스 폴더가 된다.
		GetDlgItemText(hMainDlg, IDC_PRJFOLDER,szTmp,MAX_PATH);
		SetDlgItemText(hDlg,IDC_EDSRC,szTmp);

		// 제외 폴더, 제외 파일, 백업 폴더 초기화
		SHRegReadString(HKEY_CURRENT_USER,KEY"Option","ExDir","Debug;Release", szTmp, 256);
		SetDlgItemText(hDlg,IDC_EDEXFOLDER,szTmp);
		SHRegReadString(HKEY_CURRENT_USER,KEY"Option","ExFile",
			"pch;pdb;ilk;obj;sbr;bsc;res;ncb;aps;plg;exe", szTmp, 256);
		SetDlgItemText(hDlg,IDC_EDEXFILE,szTmp);
		SHRegReadString(HKEY_CURRENT_USER,KEY"Option","BackDir","C:\\Temp", szTmp, 256);
		SetDlgItemText(hDlg,IDC_EDDEST,szTmp);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDCANCEL:
			// 제외 폴더, 제외 파일, 백업 폴더 저장
			GetDlgItemText(hDlg,IDC_EDEXFOLDER,szTmp,256);
			SHRegWriteString(HKEY_CURRENT_USER,KEY"Option","ExDir",szTmp);
			GetDlgItemText(hDlg,IDC_EDEXFILE,szTmp,256);
			SHRegWriteString(HKEY_CURRENT_USER,KEY"Option","ExFile",szTmp);
			GetDlgItemText(hDlg,IDC_EDDEST,szTmp,256);
			SHRegWriteString(HKEY_CURRENT_USER,KEY"Option","BackDir",szTmp);

			EndDialog(hDlg,0);
			return TRUE;
		case IDC_BTNBROWSE:
			if (BrowseFolder(hMainDlg,NULL,NULL,szTmp)==TRUE) {
				SetDlgItemText(hDlg,IDC_EDDEST,szTmp);
			}
			return TRUE;
		case IDOK:
			// 백업 제외 폴더 목록을 배열에 작성한다.
			cExt=0;
			GetDlgItemText(hDlg,IDC_EDEXFOLDER,szTmp,MAX_PATH);
			p=szTmp;
			while (p < szTmp+lstrlen(szTmp)) {
				e=ExFolder[cExt];
				while (*p!=';' && *p!=0) 
					*e++=*p++;
				*e=0;
				cExt++;
				p++;
			}
			ExFolder[cExt][0]=0;

			// 백업 제외 파일 목록을 배열에 작성한다.
			cExt=0;
			GetDlgItemText(hDlg,IDC_EDEXFILE,szTmp,MAX_PATH);
			p=szTmp;
			while (p < szTmp+lstrlen(szTmp)) {
				e=ExFile[cExt];
				*e++='.';		// 확장자앞에는 항상 .이 있어야 한다.
				while (*p!=';' && *p!=0) 
					*e++=*p++;
				*e=0;
				cExt++;
				p++;
			}
			ExFile[cExt][0]=0;

			// 복사 Src 폴더와 Dest 폴더를 구한다.
			SetCursor(LoadCursor(NULL,IDC_WAIT));
			GetDlgItemText(hDlg,IDC_EDSRC,szSrc,MAX_PATH);
			GetDlgItemText(hDlg,IDC_EDDEST,szDest,MAX_PATH);
			if ((_access(szSrc,0)!=0) || (_access(szDest,0)!=0)) {
				MessageBox(hDlg,"폴더 지정이 잘못되었습니다","에러",MB_OK);
				return TRUE;
			}

			// 새로 만든 폴더에 Src를 복사한다.
			p=strrchr(szSrc,'\\');
			strcat(szDest,p);
			XBackup(szSrc,szDest);
			SetCursor(LoadCursor(NULL,IDC_ARROW));
			SetDlgItemText(hBackDlg,IDC_STATUS,"백업 완료");
			return TRUE;
		}
		break;
	}
	return FALSE;
}

// Path가 디렉토리 경로인지 아닌지 조사한다.
BOOL IsDirectory(TCHAR *Path)
{
	DWORD Attr;

	Attr=GetFileAttributes(Path);
	if ((Attr & FILE_ATTRIBUTE_DIRECTORY) != 0) {
		return TRUE;
	} else {
		return FALSE;
	}
}

// 대상 폴더를 드래그받는다.
void OnDropFiles(WPARAM wParam, LPARAM lParam)
{
	int count;
	TCHAR Path[MAX_PATH];
	HDROP hDropInfo=(HDROP)wParam;

	count=DragQueryFile(hDropInfo,0xffffffff,NULL,0);
	if (count != 1) {
		MessageBox(hMainDlg,"폴더 하나만 드래그할 수 있습니다","알림",MB_OK);
		return;
	}

	DragQueryFile(hDropInfo,0, Path, MAX_PATH);
	if (IsDirectory(Path) == FALSE) {
		MessageBox(hMainDlg,"XSL 파일 또는 원본 디렉토리만 드래그할 수 있습니다.","알림",MB_OK);
		return;
	}

	SetDlgItemText(hMainDlg,IDC_PRJFOLDER,Path);
}


//////////////////////////////////////////////////////////////////////////////////
// 폴더 등록 대화상자 루틴

BOOL CALLBACK RegDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	int Count,i;
	TCHAR szKey[20];

    switch (iMessage) {
    case WM_INITDIALOG:
		// 등록된 폴더 목록을 읽어와 콤보 박스에 채운다.
		Count=SHRegReadInt(HKEY_CURRENT_USER,KEY"RegFolder","RegNum",0);

		for (i=0;i<Count;i++) {
			wsprintf(szKey,"Reg%d",i);
			SHRegReadString(HKEY_CURRENT_USER,KEY"RegFolder",szKey,"",szTmp,MAX_PATH);
			SendDlgItemMessage(hDlg,IDC_LISTFOLDER,LB_ADDSTRING,0,(LPARAM)szTmp);
		}
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDCANCEL:
            EndDialog(hDlg,0);
            return TRUE;
		// 폴더 추가 - 이미 등록되어 있는 폴더는 추가할 수 없다.
		case IDC_BTNADD:
			if (BrowseFolder(hDlg,NULL,NULL,szTmp) == TRUE) {
				i=SendDlgItemMessage(hDlg,IDC_LISTFOLDER,LB_FINDSTRINGEXACT,(WPARAM)-1,(LPARAM)szTmp);
				if (i == LB_ERR) {
					SendDlgItemMessage(hDlg,IDC_LISTFOLDER,LB_ADDSTRING,0,(LPARAM)szTmp);
				} else {
					wsprintf(szTmp2, "%s 폴더는 이미 등록되어 있습니다",szTmp);
					MessageBox(hDlg, szTmp2, "알림", MB_OK);
				}
			}
			return TRUE;
		// 폴더 삭제
		case IDC_BTNDEL:
			i=SendDlgItemMessage(hDlg,IDC_LISTFOLDER,LB_GETCURSEL,0,0);
			if (i!=LB_ERR) {
				SendDlgItemMessage(hDlg,IDC_LISTFOLDER,LB_DELETESTRING,i,0);
			}
			return TRUE;
		// 등록 폴더를 레지스트리에 기록한다. 메인 윈도우의 콤보 박스도 갱신한다.
        case IDOK:
			SendDlgItemMessage(hMainDlg,IDC_PRJFOLDER,CB_RESETCONTENT,0,0);
			Count=SendDlgItemMessage(hDlg,IDC_LISTFOLDER,LB_GETCOUNT,0,0);
			SHRegWriteInt(HKEY_CURRENT_USER,KEY"RegFolder","RegNum",Count);
			for (i=0;i<Count;i++) {
				SendDlgItemMessage(hDlg,IDC_LISTFOLDER,LB_GETTEXT,i,(LPARAM)szTmp);
				wsprintf(szKey,"Reg%d",i);
				SHRegWriteString(HKEY_CURRENT_USER,KEY"RegFolder",szKey,szTmp);
				SendDlgItemMessage(hMainDlg,IDC_PRJFOLDER,CB_ADDSTRING,0,(LPARAM)szTmp);
			}
			EndDialog(hDlg,0);
			return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

