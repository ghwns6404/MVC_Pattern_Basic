// FMOD 라이브러리를 사용한 Mp3 플레이어. 2005.1.13 김상형
#include <windows.h>
#include <commctrl.h>
#include <shlwapi.h>
#include "ShReg.h"
#include "GrpBtn.h"
#include "GrpScrl.h"
#include "fmod.h"
#include "Util.h"
#include "resource.h"

#define KEY "Software\\MiyoungSoft\\LpaMp3\\"
#define MpNum (ListView_GetItemCount(hList))
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("LpaMp3");

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClassEx;
	g_hInst=hInstance;
	HWND hBrood;
	ATOM hAtom;
	
	// 인수가 전달된 경우 이미 떠 있는 인스턴스가 있으면 넘기고 자신은 죽는다.
	// 두 번 실행을 굳이 금지하지는 않지만 쉘 오픈시는 이전 인스턴스에게 우선권을 주기로 한다.
	if (__argc > 1) {
		hBrood=FindWindow("LpaMp3",NULL);
		if (hBrood) {
			hAtom=GlobalAddAtom(__argv[1]);
			SendMessage(hBrood,WM_USER+1,(WPARAM)hAtom,0);
			GlobalDeleteAtom(hAtom);
			SetForegroundWindow(hBrood);
			return 0;
		}
	}
	
	WndClassEx.cbSize=sizeof(WNDCLASSEX);
	WndClassEx.cbClsExtra=0;
	WndClassEx.cbWndExtra=0;
	WndClassEx.hbrBackground=(HBRUSH)(COLOR_BTNFACE+1);
	WndClassEx.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClassEx.hIcon=LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
	WndClassEx.hIconSm=(HICON)LoadImage(hInstance,MAKEINTRESOURCE(IDI_ICON1),
		IMAGE_ICON,GetSystemMetrics(SM_CXSMICON),
		GetSystemMetrics(SM_CXSMICON),LR_DEFAULTCOLOR);
	WndClassEx.hInstance=hInstance;
	WndClassEx.lpfnWndProc=WndProc;
	WndClassEx.lpszClassName=lpszClass;
	WndClassEx.lpszMenuName=NULL;
	WndClassEx.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx(&WndClassEx);

	hWnd=CreateWindowEx(0,lpszClass,lpszClass,WS_POPUP | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT,CW_USEDEFAULT,250,50,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	
	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

// 메시지 핸들러
LRESULT OnCreate(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnDestroy(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnPaint(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnCommand(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnContextMenu(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnTimer(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnDropFiles(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnKeyDown(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnLButtonDown(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnHScroll(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnNotify(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnWindowPosChanging(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnUser1(HWND hWnd,WPARAM wParam,LPARAM lParam);

// 일반 함수
void OpenFiles();
void AddFileToList(TCHAR *Path);
void DrawSpectrum(HDC hdc);
void StopStream();
void PlayStream();
LPCTSTR GetMpName(int idx);
void MakeHistory();
void UpdateHistory();
void ReloadHistory();
int OnFindFile(TCHAR *Path,DWORD Attr,LPVOID Param);

// 전역 변수
HWND hOpen,hPlay,hStop,hPrev,hNext,hClose,hMin,hMenu,hPList;
HWND hProg,hVol;
HWND hCombo,hList;
int NowMp;
FSOUND_STREAM *stream=NULL;
int channel;
enum tag_Status { S_STOP, S_PAUSE, S_PLAY } Status;
float spectrum[512];
int Volume;
BOOL aTop;
enum tag_Repeat { R_SEQ, R_ONE, R_ALL, R_RANDOM } Repeat;
BOOL bSpectrum;
HFONT hGulim;

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
	case WM_CONTEXTMENU:
		return OnContextMenu(hWnd,wParam,lParam);
	case WM_TIMER:
		return OnTimer(hWnd,wParam,lParam);
	case WM_DROPFILES:
		return OnDropFiles(hWnd,wParam,lParam);
	case WM_KEYDOWN:
		return OnKeyDown(hWnd,wParam,lParam);
	case WM_LBUTTONDOWN:
		return OnLButtonDown(hWnd,wParam,lParam);
	case WM_HSCROLL:
		return OnHScroll(hWnd,wParam,lParam);
	case WM_NOTIFY:
		return OnNotify(hWnd,wParam,lParam);
	case WM_WINDOWPOSCHANGING:
		return OnWindowPosChanging(hWnd,wParam,lParam);
	case WM_USER+1:
		return OnUser1(hWnd,wParam,lParam);
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

LRESULT OnCreate(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	RECT wrt;
	BOOL bPList;
	LVCOLUMN COL;
	int i,Num;
	TCHAR Date[128],Value[64];

	InitCommonControls();
	hWndMain=hWnd;
	DragAcceptFiles(hWnd,TRUE);

	// 버튼 컨트롤 생성
	hOpen=CreateWindow("GrpBtn",NULL,WS_CHILD | WS_VISIBLE,2,30,16,16,hWnd,(HMENU)1,g_hInst,NULL);
	SendMessage(hOpen,GBM_SETIMAGE,MAKEWPARAM(IDB_OPENNORMAL,IDB_OPENHOT),MAKEWPARAM(IDB_OPENDOWN,IDB_OPENNORMAL));
	hPlay=CreateWindow("GrpBtn",NULL,WS_CHILD | WS_VISIBLE,20,30,24,16,hWnd,(HMENU)2,g_hInst,NULL);
	SendMessage(hPlay,GBM_SETIMAGE,MAKEWPARAM(IDB_PLAYNORMAL,IDB_PLAYHOT),MAKEWPARAM(IDB_PLAYDOWN,IDB_PLAYNORMAL));
	hStop=CreateWindow("GrpBtn",NULL,WS_CHILD | WS_VISIBLE,46,30,16,16,hWnd,(HMENU)3,g_hInst,NULL);
	SendMessage(hStop,GBM_SETIMAGE,MAKEWPARAM(IDB_STOPNORMAL,IDB_STOPHOT),MAKEWPARAM(IDB_STOPDOWN,IDB_STOPNORMAL));
	hPrev=CreateWindow("GrpBtn",NULL,WS_CHILD | WS_VISIBLE,64,30,16,16,hWnd,(HMENU)4,g_hInst,NULL);
	SendMessage(hPrev,GBM_SETIMAGE,MAKEWPARAM(IDB_PREVNORMAL,IDB_PREVHOT),MAKEWPARAM(IDB_PREVDOWN,IDB_PREVNORMAL));
	hNext=CreateWindow("GrpBtn",NULL,WS_CHILD | WS_VISIBLE,82,30,16,16,hWnd,(HMENU)5,g_hInst,NULL);
	SendMessage(hNext,GBM_SETIMAGE,MAKEWPARAM(IDB_NEXTNORMAL,IDB_NEXTHOT),MAKEWPARAM(IDB_NEXTDOWN,IDB_NEXTNORMAL));
	hMenu=CreateWindow("GrpBtn",NULL,WS_CHILD | WS_VISIBLE,106,30,32,16,hWnd,(HMENU)6,g_hInst,NULL);
	SendMessage(hMenu,GBM_SETIMAGE,MAKEWPARAM(IDB_MENUNORMAL,IDB_MENUHOT),MAKEWPARAM(IDB_MENUDOWN,IDB_MENUNORMAL));
	hPList=CreateWindow("GrpBtn",NULL,WS_CHILD | WS_VISIBLE | GBS_CHECK,142,30,16,16,hWnd,(HMENU)7,g_hInst,NULL);
	SendMessage(hPList,GBM_SETIMAGE,MAKEWPARAM(IDB_PLISTNORMAL,IDB_PLISTNORMAL),MAKEWPARAM(IDB_PLISTDOWN,IDB_PLISTNORMAL));

	// 닫기, 최소화 버튼
	hClose=CreateWindow("GrpBtn",NULL,WS_CHILD | WS_VISIBLE,235,3,10,10,hWnd,(HMENU)101,g_hInst,NULL);
	SendMessage(hClose,GBM_SETIMAGE,MAKEWPARAM(IDB_CLOSENORMAL,IDB_CLOSEHOT),MAKEWPARAM(IDB_CLOSEDOWN,IDB_CLOSENORMAL));
	hMin=CreateWindow("GrpBtn",NULL,WS_CHILD | WS_VISIBLE,223,3,10,10,hWnd,(HMENU)102,g_hInst,NULL);
	SendMessage(hMin,GBM_SETIMAGE,MAKEWPARAM(IDB_MINNORMAL,IDB_MINHOT),MAKEWPARAM(IDB_MINDOWN,IDB_MINNORMAL));

	// 진행 및 버튼 컨트롤 생성
	hProg=CreateWindow("GrpScrl",NULL,WS_CHILD | WS_VISIBLE | WS_DISABLED | GSS_HORZ,2,15,198,14,hWnd,(HMENU)201,g_hInst,NULL);
	SendMessage(hProg,GSM_SETRANGE,0,1000);
	hVol=CreateWindow("GrpScrl",NULL,WS_CHILD | WS_VISIBLE | GSS_HORZ,202,15,42,14,hWnd,(HMENU)202,g_hInst,NULL);
	SendMessage(hVol,GSM_SETRANGE,0,255);
	SendMessage(hVol,GSM_SETTHUMBCOLOR,(WPARAM)RGB(0,0,255),0);
	SendMessage(hVol,GSM_SETTSIZE,10,0);
	SendMessage(hVol,GSM_SETTGAP,3,0);

	// 콤보, 리스트 뷰 생성
	hGulim=CreateFont(12,0,0,0,0,0,0,0,HANGEUL_CHARSET,3,2,1,
		VARIABLE_PITCH | FF_MODERN,"굴림");
	hCombo=CreateWindow("combobox",NULL,WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
		60,55,187,200,hWnd,(HMENU)203,g_hInst,NULL);
	SendMessage(hCombo,WM_SETFONT,(WPARAM)hGulim,FALSE);
	hList=CreateWindow(WC_LISTVIEW,NULL,WS_CHILD | WS_VISIBLE | WS_BORDER |
		LVS_REPORT | LVS_SHOWSELALWAYS | LVS_NOCOLUMNHEADER ,
		2,80,244,215,hWnd,(HMENU)204,g_hInst,NULL);
	ListView_SetExtendedListViewStyle(hList, LVS_EX_FULLROWSELECT);
	COL.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	COL.fmt=LVCFMT_LEFT;
	COL.cx=225;
	COL.pszText="파일명";
	COL.iSubItem=0;
	ListView_InsertColumn(hList,0,&COL);

	// 레지스트리에서 설정 정보 읽어 옴
	Volume=SHRegReadInt(SHCU,KEY"Option","Volume",200);
	SendMessage(hVol,GSM_SETPOS,Volume,0);
	aTop=SHRegReadInt(SHCU,KEY"Option","aTop",FALSE);
	if (aTop==TRUE) {
		SetWindowPos(hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
	Repeat=(tag_Repeat)SHRegReadInt(SHCU,KEY"Option","Repeat",R_ALL);
	bSpectrum=SHRegReadInt(SHCU,KEY"Option","bSpectrum",TRUE);
	wrt.left=SHRegReadInt(SHCU,KEY"Option","left",100);
	wrt.top=SHRegReadInt(SHCU,KEY"Option","top",100);
	SetWindowPos(hWnd,NULL,wrt.left,wrt.top,0,0,SWP_NOSIZE | SWP_NOZORDER);
	bPList=SHRegReadInt(SHCU,KEY"Option","bPList",FALSE);
	if (bPList==TRUE) {
		SendMessage(hPList,GBM_SETSTATE,GB_DOWN,0);
		SetWindowPos(hWnd,NULL,0,0,250,300,SWP_NOMOVE | SWP_NOZORDER);
	}

	// 히스토리 목록 읽음
	Num=SHRegReadInt(SHCU,KEY"History","Num",0);
	for (i=0;i<Num;i++) {
		wsprintf(Value,"%d",i);
		SHRegReadString(SHCU,KEY"History",Value,"",Date,MAX_PATH);
		SendMessage(hCombo,CB_ADDSTRING,0,(LPARAM)Date);
	}
	if (Num!=0) {
		SendMessage(hCombo,CB_SETCURSEL,0,0);
		ReloadHistory();
	}

	channel=-1;
	Status=S_STOP;
	SetTimer(hWnd,0,1000,NULL);
	SetTimer(hWnd,1,100,NULL);

	// 전달된 인수가 있으면 이 파일을 연주한다. 확장자만 연결해 두면 더블 클릭해서 연주 가능
	if (__argc > 1) {
		ListView_DeleteAllItems(hList);
		AddFileToList(__argv[1]);
		PlayStream();
	}
	return 0;
}

LRESULT OnDestroy(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	RECT wrt;
	int i,Count;
	TCHAR Date[128],Value[64];

	// 설정 저장
	GetWindowRect(hWnd,&wrt);
	SHRegWriteInt(SHCU,KEY"Option","left",wrt.left);
	SHRegWriteInt(SHCU,KEY"Option","top",wrt.top);
	SHRegWriteInt(SHCU,KEY"Option","aTop",aTop);
	SHRegWriteInt(SHCU,KEY"Option","Repeat",Repeat);
	SHRegWriteInt(SHCU,KEY"Option","Volume",Volume);
	SHRegWriteInt(SHCU,KEY"Option","bSpectrum",bSpectrum);
	SHRegWriteInt(SHCU,KEY"Option","bPList",SendMessage(hPList,GBM_GETSTATE,0,0)==GB_DOWN);

	// 히스토리 목록 저장
	Count=SendMessage(hCombo,CB_GETCOUNT,0,0);
	SHRegWriteInt(SHCU,KEY"History","Num",Count);
	for (i=0;i<Count;i++) {
		wsprintf(Value,"%d",i);
		SendMessage(hCombo,CB_GETLBTEXT,i,(LPARAM)Date);
		SHRegWriteString(SHCU,KEY"History",Value,(TCHAR *)Date);
	}

	DeleteObject(hGulim);
	KillTimer(hWnd,0);
	KillTimer(hWnd,1);
	StopStream();
	PostQuitMessage(0);
	return 0;
}

LRESULT OnPaint(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR SongName[MAX_PATH];
	HFONT OldFont;
	RECT crt;
	HPEN hPen,OldPen;

	hdc=BeginPaint(hWnd, &ps);
	GetClientRect(hWnd,&crt);
	SelectObject(hdc,GetStockObject(WHITE_PEN));
	MoveToEx(hdc,0,0,NULL);LineTo(hdc,crt.right,0);
	MoveToEx(hdc,0,0,NULL);LineTo(hdc,0,crt.bottom);
	hPen=CreatePen(PS_SOLID,1,RGB(92,92,92));
	OldPen=(HPEN)SelectObject(hdc,hPen);
	MoveToEx(hdc,0,crt.bottom-1,NULL);LineTo(hdc,crt.right,crt.bottom-1);
	MoveToEx(hdc,crt.right-1,0,NULL);LineTo(hdc,crt.right-1,crt.bottom-1);
	DeleteObject(SelectObject(hdc,OldPen));

	OldFont=(HFONT)SelectObject(hdc,hGulim);
	if (Status == S_STOP) {
		lstrcpy(SongName,"");
	} else {
		_splitpath(GetMpName(NowMp),NULL,NULL,SongName,NULL);
	}
	SetBkMode(hdc,TRANSPARENT);
	TextOut(hdc,2,2,SongName,lstrlen(SongName));
	TextOut(hdc,2,59,"노래 모음",9);
	SelectObject(hdc,OldFont);
	DrawSpectrum(hdc);
	EndPaint(hWnd, &ps);
	return 0;
}

LRESULT OnCommand(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	int i,*pi;
	static int arLarge[8]={55,60,70,80,100,140,200,300};
	static int arSmall[8]={295,290,280,270,250,210,150,50};
	static int arSleep[8]={60,50,40,30,20,10,10,10};

	switch (LOWORD(wParam)) {
	case 1:			// 파일 열기
		OpenFiles();
		break;
	case 2:			// 연주,잠시 중지
		switch (Status) {
		case S_STOP:
			PlayStream();
			break;
		case S_PAUSE:
			FSOUND_SetPaused(channel,FALSE);
			Status=S_PLAY;
			break;
		case S_PLAY:
			FSOUND_SetPaused(channel,TRUE);
			Status=S_PAUSE;
			break;
		}
		break;
	case 3:			// 중지
		StopStream();
		break;
	case 4:			// 이전곡
		if (Repeat == R_RANDOM) {
			NowMp=rand() % MpNum;
		} else {
			NowMp=(NowMp != 0) ? NowMp-1:MpNum-1;
		}
		PlayStream();
		break;
	case 5:			// 다음곡
		if (Repeat == R_RANDOM) {
			NowMp=rand() % MpNum;
		} else {
			NowMp=(NowMp < MpNum-1) ? NowMp+1:0;
		}
		PlayStream();
		break;
	case 6:			// 메뉴
		SendMessage(hWnd,WM_CONTEXTMENU,0,0);
		break;
	case 7:			// 연주 목록 토글
		if (SendMessage(hPList,GBM_GETSTATE,0,0)==GB_DOWN) {
			pi=arLarge;
		} else {
			pi=arSmall;
		}
		for (i=0;i<8;i++,pi++) {
			SetWindowPos(hWnd,NULL,0,0,250,*pi,SWP_NOMOVE | SWP_NOZORDER);
			UpdateWindow(hWnd);
			Sleep(arSleep[i]);
		}
		break;
	case 101:		// 닫기
		DestroyWindow(hWnd);
		break;
	case 102:		// 최소화
		CloseWindow(hWnd);
		break;
	case 203:		// 콤보 박스
		switch (HIWORD(wParam)) {
		case CBN_SELCHANGE:
			ReloadHistory();
			PlayStream();
			break;
		}
		break;
	case IDM_POPUP_ATOP:
		aTop=!aTop;
		SetWindowPos(hWnd,aTop ? HWND_TOPMOST:HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		break;
	case IDM_POPUP_REPSEQ:
		Repeat=R_SEQ;
		break;
	case IDM_POPUP_REPONE:
		Repeat=R_ONE;
		break;
	case IDM_POPUP_REPALL:
		Repeat=R_ALL;
		break;
	case IDM_POPUP_RANDOM:
		Repeat=R_RANDOM;
		break;
	case IDM_POPUP_SPECTRUM:
		bSpectrum=!bSpectrum;
		break;
	case IDM_POPUP_ABOUT:
		MessageBox(hWnd,"LpaMp3 Ver 1.0\r\n\r\n"
			"이 프로그램은 최초 실행 후 100년동안만 사용할 수 있는 셰어웨어입니다.\r\n"
			"FMOD 라이브러리를 사용했으며 비주얼 C++ 6.0으로 컴파일했습니다.\r\n"
			"소스는 http://www.winapi.co.kr에서 다운로드 받을 수 있습니다.",
			"프로그램 소개",MB_OK);
		break;
	}
	return 0;
}

LRESULT OnContextMenu(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	HMENU hMenu, hPopup;
	POINT pt;

	hMenu=LoadMenu(g_hInst,MAKEINTRESOURCE(IDR_MENU1));
	hPopup=GetSubMenu(hMenu,0);

	CheckMenuItem(hPopup,IDM_POPUP_ATOP,MF_BYCOMMAND | (aTop ? MF_CHECKED:MF_UNCHECKED));
	CheckMenuItem(hPopup,IDM_POPUP_REPSEQ,MF_BYCOMMAND | (Repeat==R_SEQ ? MF_CHECKED:MF_UNCHECKED));
	CheckMenuItem(hPopup,IDM_POPUP_REPONE,MF_BYCOMMAND | (Repeat==R_ONE ? MF_CHECKED:MF_UNCHECKED));
	CheckMenuItem(hPopup,IDM_POPUP_REPALL,MF_BYCOMMAND | (Repeat==R_ALL ? MF_CHECKED:MF_UNCHECKED));
	CheckMenuItem(hPopup,IDM_POPUP_RANDOM,MF_BYCOMMAND | (Repeat==R_RANDOM ? MF_CHECKED:MF_UNCHECKED));
	CheckMenuItem(hPopup,IDM_POPUP_SPECTRUM,MF_BYCOMMAND | (bSpectrum ? MF_CHECKED:MF_UNCHECKED));

	GetCursorPos(&pt);
	TrackPopupMenu(hPopup,TPM_LEFTALIGN,pt.x,pt.y,0,hWnd,NULL);
	DestroyMenu(hMenu);
	return 0;
}

LRESULT OnTimer(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	int Len,Pos;
	float *spec;
	HDC hdc;

	if (Status != S_PLAY) {
		return 0;
	}
	
	switch (wParam) {
	case 0:			// 프로그래스 점검 타이머
		Len=FSOUND_Stream_GetLength(stream);
		Pos=FSOUND_Stream_GetPosition(stream);
		SendMessage(hProg,GSM_SETPOS,MulDiv(Pos,1000,Len),0);
		if (Len==Pos) {
			if (Repeat == R_RANDOM) {
				NowMp=rand() % MpNum;
			} else {
				if (Repeat != R_ONE) {
					if (NowMp == MpNum-1) {
						if (Repeat==R_ALL) {
							NowMp=0;
						} else {	// R_SEQ
							StopStream();
							break;
						}
					} else {
						NowMp++;
					}
				}
			}
			PlayStream();
		}
		break;
	case 1:			// 스팩트럼 출력 타이머
		spec=FSOUND_DSP_GetSpectrum();
		memcpy(spectrum,spec,sizeof(float)*512);
		hdc=GetDC(hWnd);
		DrawSpectrum(hdc);
		ReleaseDC(hWnd,hdc);
		break;
	}
	return 0;
}

LRESULT OnDropFiles(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	int nDrop,i;
	TCHAR Path[MAX_PATH];
	POINT pt,cpt;
	HMENU hMenu, hPopup;
	WORD Cmd;
	BOOL bRemake;

	GetCursorPos(&pt);
	cpt=pt;
	ScreenToClient(hWnd,&cpt);

	// 메인 창에 드롭할 때는 Ctrl키의 상태에 따라, 목록에 드롭할 때는 팝업 메뉴로 질문
	// 파일이 하나도 없을 때는 질문할 필요도 없음
	if (MpNum == 0) {
		bRemake=TRUE;
	} else {
		if (cpt.y < 80) {
			bRemake=((GetKeyState(VK_CONTROL) & 0x8000) == 0);
		} else {
			hMenu=LoadMenu(g_hInst,MAKEINTRESOURCE(IDR_MENU1));
			hPopup=GetSubMenu(hMenu,1);
			Cmd=(WORD)TrackPopupMenu(hPopup,TPM_LEFTALIGN | TPM_RETURNCMD,pt.x,pt.y,0,hWnd,NULL);
			DestroyMenu(hMenu);
			if (Cmd==0) return 0;
			bRemake=(Cmd==IDM_POPUP_REMAKE);
		}
	}

	// 그냥 드롭은 바로 연주, Ctrl+Drop은 목록에 추가만
	if (bRemake) {
		ListView_DeleteAllItems(hList);
		NowMp=0;
	}
	nDrop=DragQueryFile((HDROP)wParam,-1,NULL,0);
	for (i=0;i<nDrop;i++) {
		DragQueryFile((HDROP)wParam,i,Path,MAX_PATH);
		if (GetFileAttributes(Path) & FILE_ATTRIBUTE_DIRECTORY) {
			FindInFiles(Path,"*.mp3;*.wav",FIF_DEEP,OnFindFile,0);
		} else {
			if (IsMatch(Path,"*.mp3;*.wav")) {
				AddFileToList(Path);
			}
		}
	}
	if (bRemake) {
		PlayStream();
		MakeHistory();
	} else {
		UpdateHistory();
	}
	return 0;
}

LRESULT OnKeyDown(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	switch (wParam) {
	case VK_UP:
		Volume=min(255,Volume+10);
		FSOUND_SetVolume(channel,Volume);
		SendMessage(hVol,GSM_SETPOS,Volume,0);
		break;
	case VK_DOWN:
		Volume=max(0,Volume-10);
		FSOUND_SetVolume(channel,Volume);
		SendMessage(hVol,GSM_SETPOS,Volume,0);
		break;
	case VK_LEFT:
		SendMessage(hWnd,WM_COMMAND,MAKEWPARAM(4,0),0);
		break;
	case VK_RIGHT:
		SendMessage(hWnd,WM_COMMAND,MAKEWPARAM(5,0),0);
		break;
	}
	return 0;
}

LRESULT OnLButtonDown(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	return DefWindowProc(hWnd,WM_NCLBUTTONDOWN,HTCAPTION,lParam);
}

LRESULT OnHScroll(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	int Len,Pos;

	if (lParam == (LPARAM)hProg) {
		Pos=SendMessage(hProg,GSM_GETPOS,0,0);
		Len=FSOUND_Stream_GetLength(stream);
		FSOUND_Stream_SetPosition(stream,MulDiv(Len,Pos,1000));
	}
	if (lParam == (LPARAM)hVol) {
		Volume=SendMessage(hVol,GSM_GETPOS,0,0);
		FSOUND_SetVolume(channel,Volume);
	}
	return 0;
}

LRESULT OnNotify(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	LPNMHDR hdr;
	LPNMITEMACTIVATE nia;
	LPNMLISTVIEW nlv;
	LVITEM LI;

	hdr=(LPNMHDR)lParam;
	if (hdr->hwndFrom == hList) {
		switch (hdr->code) {
		case NM_DBLCLK:
			nia=(LPNMITEMACTIVATE)lParam;
			if (nia->iItem != -1) {
				NowMp=nia->iItem;
				PlayStream();
			}
			break;
		case LVN_DELETEITEM:
			nlv=(LPNMLISTVIEW)lParam;
			LI.mask=LVIF_PARAM;
			LI.iItem=nlv->iItem;
			ListView_GetItem(hList,&LI);
			free((LPVOID)LI.lParam);
			break;
		}
	}
	return 0;
}

LRESULT OnWindowPosChanging(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	int t;
	RECT rt;
	LPWINDOWPOS wp=(LPWINDOWPOS)lParam;
	const int gap=15;

	SystemParametersInfo(SPI_GETWORKAREA,0,&rt,0);
	t=wp->x;
	if (t < rt.left+gap && t > rt.left-gap) t=rt.left;
	if (t+wp->cx > rt.right-gap && t+wp->cx < rt.right+gap)
		t=rt.right-wp->cx;
	wp->x=t;

	t=wp->y;
	if (t < rt.top+gap && t > rt.top-gap) t=rt.top;
	if (t+wp->cy > rt.bottom-gap && t+wp->cy < rt.bottom+gap)
		t=rt.bottom-wp->cy;
	wp->y=t;
	return 0;
}

LRESULT OnUser1(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	TCHAR Path[MAX_PATH];

	if (wParam) {
		GlobalGetAtomName((ATOM)wParam,Path,MAX_PATH);
		NowMp=0;
		ListView_DeleteAllItems(hList);
		AddFileToList(Path);
		PlayStream();
	}
	if (IsIconic(hWnd)) {
		ShowWindow(hWnd,SW_RESTORE);
	}
	return 0;
}

///////////////////////////////////////////////////////////////////

void OpenFiles()
{
	OPENFILENAME OFN;
	TCHAR lpstrFile[30000]="";
	TCHAR *p;
	TCHAR Name[MAX_PATH];
	TCHAR Dir[MAX_PATH];
	TCHAR szTmp[MAX_PATH];
	int i=1;

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner=hWndMain;
	OFN.lpstrFilter="소리 파일들\0*.mp3;*.wav\0모든 파일(*.*)\0*.*\0";
	OFN.lpstrFile=lpstrFile;
	OFN.nMaxFile=10000;
	OFN.Flags=OFN_EXPLORER | OFN_ALLOWMULTISELECT;
	if (GetOpenFileName(&OFN)!=0) {
		ListView_DeleteAllItems(hList);
		NowMp=0;
		p=lpstrFile;
		lstrcpy(Name, p);
		if (*(p+lstrlen(Name)+1)==0) {
			AddFileToList(Name);
		} else {
			lstrcpy(Dir,Name);
			for (;;) {
				p=p+lstrlen(Name)+1;
				if (*p==0)
					break;
				lstrcpy(Name,p);
				wsprintf(szTmp,"%s\\%s",Dir,Name);
				AddFileToList(szTmp);
			}
		}
		PlayStream();
		MakeHistory();
	} else {
		if (CommDlgExtendedError()==FNERR_BUFFERTOOSMALL) {
			MessageBox(hWndMain,"이렇게 많은 파일을 한꺼번에 선택하면 안됩니다.","에러",MB_OK);
		}
	}
}

int OnFindFile(TCHAR *Path,DWORD Attr,LPVOID Param)
{
	if ((Attr & FILE_ATTRIBUTE_DIRECTORY) == 0) {
		AddFileToList(Path);
	}
	return 0;
}

void AddFileToList(TCHAR *Path)
{
	LVITEM LI;
	TCHAR Name[MAX_PATH];
	TCHAR *Param;

	// 텍스트는 파일명만 파라미터에는 풀 패스를 저장한다.
	LI.mask=LVIF_TEXT | LVIF_PARAM;
	LI.iSubItem=0;
	LI.iItem=100000;
	_splitpath(Path,NULL,NULL,Name,NULL);
	LI.pszText=Name;
	Param=(TCHAR *)malloc(lstrlen(Path)+1);
	lstrcpy(Param,Path);
	LI.lParam=(LPARAM)Param;
	ListView_InsertItem(hList,&LI);
}

// 스팩트럼 그림 - 앞부분의 주파수만 대충 그렸음.
void DrawSpectrum(HDC hdc)
{
	int i,x;
	RECT rt;
	HPEN hPen,OldPen;

	SetRect(&rt,202,30,244,46);
	FillRect(hdc,&rt,GetSysColorBrush(COLOR_BTNFACE));
	if (bSpectrum) {
		hPen=CreatePen(PS_SOLID,1,RGB(0,0,255));
		OldPen=(HPEN)SelectObject(hdc,hPen);
		for (x=202,i=0;x<244;x++,i++) {
			MoveToEx(hdc,x,46,NULL);
			LineTo(hdc,x,int(46-min(spectrum[i]*16*1,16)));
		}
		DeleteObject(SelectObject(hdc,OldPen));
	}
}

// 재생중이면 중지한다. 그렇지 않으면 두 음악이 동시에 나온다.
void StopStream()
{
	if (stream) {
		FSOUND_Stream_Stop(stream);
		FSOUND_Stream_Close(stream);
		stream=NULL;
		Status=S_STOP;
		SendMessage(hProg,GSM_SETPOS,0,0);
		EnableWindow(hProg,FALSE);
		InvalidateRect(hWndMain,NULL,TRUE);
	}
}

void PlayStream()
{
	TCHAR Mes[512];
	static BOOL bFirst=TRUE;

	if (MpNum == 0) {
		return;
	}
	if (bFirst) {
		FSOUND_Init(44100,32,0);
		FSOUND_DSP_SetActive(FSOUND_DSP_GetFFTUnit(),TRUE);
		bFirst=FALSE;
	}

	StopStream();
	lstrcpy(Mes,GetMpName(NowMp));
	stream=FSOUND_Stream_Open(Mes,FSOUND_NORMAL, 0, 0);
	if (stream!=NULL) {
		channel=FSOUND_Stream_Play(FSOUND_FREE, stream);
		// 전역값대로 볼륨 설정. 새로 생성되는 채널은 항상 최대 볼륨이므로 강제로 맞춰줘야 함
		FSOUND_SetVolume(channel,Volume);

		Status=S_PLAY;
		SendMessage(hProg,GSM_SETPOS,0,0);
		EnableWindow(hProg,TRUE);
		InvalidateRect(hWndMain,NULL,TRUE);
	} else {
		wsprintf(Mes,"%s 파일을 열 수 없습니다.",GetMpName(NowMp));
		MessageBox(hWndMain,Mes,"에러",MB_OK);
	}
}

LPCTSTR GetMpName(int idx)
{
	LVITEM LI;

	LI.mask=LVIF_PARAM;
	LI.iItem=idx;
	ListView_GetItem(hList,&LI);
	return (LPCTSTR)LI.lParam;
}

void MakeHistory()
{
	SYSTEMTIME st;
	TCHAR Date[128];
	int Count;
	TCHAR Key[128];

	// 새 히스토리를 작성한다.
	GetLocalTime(&st);
	wsprintf(Date,"%02d/%02d일 %02d:%02d:%02d에 들은 노래",
		st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
	SendMessage(hCombo,CB_INSERTSTRING,0,(LPARAM)Date);
	SendMessage(hCombo,CB_SETCURSEL,0,0);

	// 10개를 넘으면 11번째 건 삭제한다.
	Count=SendMessage(hCombo,CB_GETCOUNT,0,0);
	if (Count==11) {
		SendMessage(hCombo,CB_GETLBTEXT,10,(LPARAM)Date);
		SendMessage(hCombo,CB_DELETESTRING,10,0);
		wsprintf(Key,"%sHistory\\%s",KEY,Date);
		SHDeleteKey(SHCU,Key);
	}
	UpdateHistory();
}

// 히스토리를 레지스트리에 저장한다. - 시작, 종료 속도를 신속하게 하기 위해.
void UpdateHistory()
{
	int Count;
	int i,Sel;
	LVITEM LI;
	TCHAR Date[128];
	TCHAR Key[128],Value[64];

	Sel=SendMessage(hCombo,CB_GETCURSEL,0,0);
	SendMessage(hCombo,CB_GETLBTEXT,Sel,(LPARAM)Date);
	Count=ListView_GetItemCount(hList);
	wsprintf(Key,"%sHistory\\%s",KEY,Date);
	SHRegWriteInt(SHCU,Key,"Num",Count);
	for (i=0;i<Count;i++) {
		LI.mask=LVIF_PARAM;
		LI.iItem=i;
		ListView_GetItem(hList,&LI);
		wsprintf(Value,"%d",i);
		SHRegWriteString(SHCU,Key,Value,(TCHAR *)LI.lParam);
	}
}

void ReloadHistory()
{
	TCHAR Date[128];
	int i,Num,Sel;
	TCHAR Key[128],Value[64];
	TCHAR Path[MAX_PATH];

	NowMp=0;
	ListView_DeleteAllItems(hList);
	Sel=SendMessage(hCombo,CB_GETCURSEL,0,0);
	SendMessage(hCombo,CB_GETLBTEXT,Sel,(LPARAM)Date);
	wsprintf(Key,"%sHistory\\%s",KEY,Date);
	Num=SHRegReadInt(SHCU,Key,"Num",0);
	for (i=0;i<Num;i++) {
		wsprintf(Value,"%d",i);
		SHRegReadString(SHCU,Key,Value,"",Path,MAX_PATH);
		AddFileToList(Path);
	}
}
