#include <windows.h>
#include <wininet.h>
#include <commctrl.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
BOOL CALLBACK FtpDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("FTPDemo");

HWND hList, hLabel1, hLabel2;				// 파일 목록 리스트 뷰
HIMAGELIST hImgSm;							// 이미지
HINTERNET hInternet, hFtp;					// 인터넷 핸들
TCHAR g_Server[50]="ftp.microsoft.com";		// 접속 서버
TCHAR g_User[20]="anonymous";				// 사용자 ID
TCHAR g_Pass[20]="aaa@bbb.co.kr";			// 패스워드

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
	WndClass.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);

	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

// FTP에 연결한다.
BOOL MyFtpConnect()
{
	hInternet=InternetOpen("FTPTEST", INTERNET_OPEN_TYPE_PRECONFIG,
		NULL, NULL, 0);
	if (hInternet == NULL) {
		return FALSE;
	}

	hFtp=InternetConnect(hInternet,g_Server,INTERNET_DEFAULT_FTP_PORT,
		g_User,g_Pass,INTERNET_SERVICE_FTP,0,0);
	if (hFtp==NULL) {
		InternetCloseHandle(hInternet);
		return FALSE;
	}
	return TRUE;
}

// FTP 연결을 끊는다.
void MyFtpUnConnect()
{
	InternetCloseHandle(hFtp);
	InternetCloseHandle(hInternet);
	hFtp=NULL;
	hInternet=NULL;
}

// 파일을 리스트 뷰에 출력한다.
void AddFileToList(WIN32_FIND_DATA wfd)
{
	LVITEM LI;
	int idx;
	TCHAR szTmp[50];
	SYSTEMTIME st;

	LI.mask=LVIF_TEXT | LVIF_IMAGE;

	if (lstrcmp(wfd.cFileName,"한단계 위로")!=0) {
		if (wfd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			LI.iImage=0;
		else
			LI.iImage=1;
	} else {
		LI.iImage=2;
	}
	LI.iSubItem=0;
	LI.iItem=10000;
	LI.pszText=wfd.cFileName;
	idx=ListView_InsertItem(hList,&LI);

	if (lstrcmp(wfd.cFileName,"한단계 위로")!=0) {
		itoa(wfd.nFileSizeLow,szTmp,10);
		ListView_SetItemText(hList,idx,1,szTmp);
		FileTimeToSystemTime(&wfd.ftLastWriteTime,&st);
		wsprintf(szTmp,"%d년 %d월 %d일 %d시 %d분",st.wYear, st.wMonth, st.wDay, 
			st.wHour, st.wMinute);
		ListView_SetItemText(hList,idx,2,szTmp);
	}
}

// FTP 서버의 현재 디렉토리에서 파일을 열거한다.
void MyFtpEnumFiles()
{
	WIN32_FIND_DATA wfd;
	BOOL bResult=TRUE;
	HINTERNET hFind;

	if (hFtp==NULL)
		return;

	// 모든 항목을 지우고 한단계 위로 항목을 디폴트로 보여준다.
	ListView_DeleteAllItems(hList);
	lstrcpy(wfd.cFileName,"한단계 위로");
	AddFileToList(wfd);

	// 파일 목록 출력
	hFind=FtpFindFirstFile(hFtp,"*.*",&wfd,0,0);
	if (hFind!=NULL) {
		AddFileToList(wfd);
		while (bResult) {
			bResult=InternetFindNextFile(hFind,&wfd);
			if (bResult==TRUE) {
				AddFileToList(wfd);
			}
		}
		InternetCloseHandle(hFind);
	}
}

// FTP의 현재 디렉토리에서 파일을 다운로드 받는다.
BOOL MyFtpDownLoad(TCHAR *Path)
{
	TCHAR NewFile[MAX_PATH];

	wsprintf(NewFile,"c:\\%s",Path);
	return FtpGetFile(hFtp,Path,NewFile,FALSE,FILE_ATTRIBUTE_NORMAL,
		FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD,0);
}

// 파일 데이터 다운로드
BOOL MyFtpDownLoad2(TCHAR *Path)
{
	HINTERNET hRemote;
	HANDLE hLocal;
	DWORD dwRead,dwWritten,Prog;
	BOOL Result;
	TCHAR NewFile[MAX_PATH];
	TCHAR buf[1000];
	TCHAR str[255];

	// 서버의 파일을 오픈한다.
	hRemote=FtpOpenFile(hFtp,Path,GENERIC_READ,FTP_TRANSFER_TYPE_BINARY,0);
	if (hRemote == NULL)
		return FALSE;

	// 로컬 파일을 만든다.
	wsprintf(NewFile,"c:\\%s",Path);
	hLocal=CreateFile(NewFile,GENERIC_WRITE,0,NULL,
		CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	// 1000바이트씩 받으며 진행 상태를 출력한다.
	Prog=0;
	for (;;) {
		Result=InternetReadFile(hRemote,buf,1000,&dwRead);
		if ((Result==TRUE) && (dwRead==0))
			break;
		WriteFile(hLocal,buf,dwRead,&dwWritten,NULL);
		Prog+=dwWritten;
		wsprintf(str,"%d바이트 받았습니다",Prog);
		SetWindowText(hLabel2,str);
	}

	CloseHandle(hLocal);
	InternetCloseHandle(hRemote);
	return TRUE;
}

// Path 파일을 FTP의 현재 디렉토리로 업로드한다.
BOOL MyFtpUpLoad(TCHAR *Path)
{
	TCHAR ext[_MAX_EXT];
   	TCHAR Remote[MAX_PATH];

	_splitpath(Path,NULL,NULL,Remote,ext);
	lstrcat(Remote,ext);
	return FtpPutFile(hFtp,Path,Remote,
		FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD,0);
}

// 파일 데이터 업로드
BOOL MyFtpUpLoad2(TCHAR *Path)
{
	HINTERNET hRemote;
	HANDLE hLocal;
	TCHAR ext[_MAX_EXT];
   	TCHAR Remote[MAX_PATH];
	DWORD dwRead,dwWritten,Prog;
	BOOL Result;
	TCHAR buf[1000];
	TCHAR str[255];

	// 서버에 파일을 만든다.
	_splitpath(Path,NULL,NULL,Remote,ext);
	lstrcat(Remote,ext);
	hRemote=FtpOpenFile(hFtp,Remote,GENERIC_WRITE,FTP_TRANSFER_TYPE_BINARY,0);
	if (hRemote == NULL)
		return FALSE;

	// 로컬의 파일을 연다
	hLocal=CreateFile(Path,GENERIC_READ,0,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	// 1000바이트씩 올리며 진행 상태를 출력한다.
	Prog=0;
	for (;;) {
		Result=ReadFile(hLocal,buf,1000,&dwRead,NULL);
		if ((Result==TRUE) && (dwRead==0))
			break;
		InternetWriteFile(hRemote,buf,1000,&dwWritten);
		Prog+=dwWritten;
		wsprintf(str,"%d바이트 보냈습니다",Prog);
		SetWindowText(hLabel2,str);
	}

	CloseHandle(hLocal);
	InternetCloseHandle(hRemote);
	return TRUE;
}

// 항목 더블클릭 처리
void OnDblClick(int iItem)
{
	LVITEM LI;
	TCHAR Path[MAX_PATH];
	BOOL Result;

	if ((hFtp == NULL) || (iItem == -1))
		return;

	// 한 단계 위로 이동
	if (iItem == 0) {
		if (FtpSetCurrentDirectory(hFtp,"..")==TRUE)
			MyFtpEnumFiles();
		return;
	}

	LI.iItem=iItem;
	LI.iSubItem=0;
	LI.pszText=Path;
	LI.cchTextMax=255;
	LI.mask=LVIF_IMAGE | LVIF_TEXT;
	ListView_GetItem(hList,&LI);

	// 디렉토리 변경
	if (LI.iImage == 0) {
		if (FtpSetCurrentDirectory(hFtp,Path)==TRUE) {
			MyFtpEnumFiles();
		} else {
			MessageBox(hWndMain,"디렉토리 변경 실패","알림",0);
		}
	// 파일 다운 로드 - Shift키를 누르고 있으면 1000바이트씩 받는다.
	} else {
		if ((GetKeyState(VK_SHIFT) & 0x8000) == 0x8000) {
			Result=MyFtpDownLoad2(Path);
		} else {
			Result=MyFtpDownLoad(Path);
		}
		if (Result==TRUE) {
			MessageBox(hWndMain,"다운로드 받았습니다","알림",0);
		} else {
			MessageBox(hWndMain,"다운로드 실패했습니다.","알림",0);
		}
	}
}

// 파일 업로드
void OnUpLoad()
{
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH]="";

	if (hFtp == NULL) {
		MessageBox(hWndMain,"먼저 FTP 서버에 연결하십시요","알림",MB_OK);
		return;
	}

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner=hWndMain;
	OFN.lpstrFilter="Every File(*.*)\0*.*\0";
	OFN.lpstrFile=lpstrFile;
	OFN.nMaxFile=MAX_PATH;
	OFN.lpstrInitialDir="c:\\";
	if (GetOpenFileName(&OFN)!=0) {
		if (MyFtpUpLoad(OFN.lpstrFile)==TRUE) {
			MessageBox(hWndMain,"업로드했습니다.","알림",0);
			MyFtpEnumFiles();
		} else {
			MessageBox(hWndMain,"업로드에 실패했습니다.","알림",0);
		}
	}
}

// 윈도우 프로시저
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR szCap[255];
	LV_COLUMN COL;

	switch (iMessage) {
	case WM_CREATE:
		hLabel1=CreateWindow("static","현재상태:연결 해제",WS_CHILD | WS_VISIBLE,
			10,10,500,20,hWnd,(HMENU)1,g_hInst,NULL);
		hLabel2=CreateWindow("static","다운로드 상태",WS_CHILD | WS_VISIBLE,
			10,30,300,20,hWnd,(HMENU)2,g_hInst,NULL);
		hList=CreateWindow(WC_LISTVIEW,NULL,WS_CHILD | WS_VISIBLE | WS_BORDER |
			LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL,
			10,55,600,400,hWnd,(HMENU)3,g_hInst,NULL);
		hImgSm=ImageList_LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BIT16), 16, 1, 
			RGB(255,255,255));
		ListView_SetImageList(hList, hImgSm, LVSIL_SMALL);
		COL.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT |LVCF_SUBITEM;
		COL.fmt=LVCFMT_LEFT;
		COL.cx=200;
		COL.pszText="파일";
		COL.iSubItem=0;
		ListView_InsertColumn(hList,0,&COL);

		COL.pszText="크기";
		COL.iSubItem=1;
		ListView_InsertColumn(hList,1,&COL);

		COL.cx=150;
		COL.pszText="날짜";
		COL.iSubItem=2;
		ListView_InsertColumn(hList,2,&COL);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDM_CONNECT:
			if (hFtp != NULL) {
				MessageBox(hWnd,"기존 연결을 먼저 끊으십시요.","알림",MB_OK);
				break;
			}
			if (DialogBox(g_hInst,MAKEINTRESOURCE(IDD_DLGFTP),hWnd,FtpDlgProc)==IDCANCEL)
				break;
			wsprintf(szCap,"현재상태:%s 서버에 연결하고 있습니다.",g_Server);
			SetWindowText(hLabel1,szCap);
			if (MyFtpConnect()==FALSE) {
				MessageBox(hWnd,"연결에 실패했습니다.","알림",MB_OK);
				break;
			}
			MyFtpEnumFiles();
			wsprintf(szCap,"현재상태:%s 서버에 연결됨",g_Server);
			SetWindowText(hLabel1,szCap);
			break;
		case IDM_DISCONNECT:
			MyFtpUnConnect();
			ListView_DeleteAllItems(hList);
			SetWindowText(hLabel1,"현재상태:연결 해제");
			break;
		case IDM_UPLOAD:
			OnUpLoad();
			break;
		case IDM_EXIT:
			PostQuitMessage(0);
			break;
		}
		return 0;
	case WM_NOTIFY:
		LPNMHDR hdr;
		LPNMITEMACTIVATE nia;
		hdr=(LPNMHDR)lParam;
		nia=(LPNMITEMACTIVATE)lParam;
		if (hdr->hwndFrom == hList) {
			switch (hdr->code) {
			// 선택된 항목을 에디트에 보여준다.
			case NM_DBLCLK:
				OnDblClick(nia->iItem);
				break;
			}
		}
		break;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		if (hFtp != NULL)
			MyFtpUnConnect();
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

// FTP 연결 설정을 변경한다.
BOOL CALLBACK FtpDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam) 
{
	switch (iMessage) {
	case WM_INITDIALOG:
		SetDlgItemText(hDlg,IDC_EDSERVER,g_Server);
		SetDlgItemText(hDlg,IDC_EDUSER,g_User);
		SetDlgItemText(hDlg,IDC_EDPASS,g_Pass);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			GetDlgItemText(hDlg,IDC_EDSERVER,g_Server,50);
			GetDlgItemText(hDlg,IDC_EDUSER,g_User,20);
			GetDlgItemText(hDlg,IDC_EDPASS,g_Pass,20);
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

