// 타자 연습 프로그램
// todo
// - 화면 해상도가 충분하면 화면 폭을 늘릴 수 있도록 할 것(950정도)

#include <windows.h>
#include <stdio.h>
#include <imm.h>
#include <mmsystem.h>
#include <io.h>
#include "resource.h"
#include "JohabUtil.h"
#include "ShReg.h"

#define KEY "Software\\MiyoungSoft\\LpaTaja\\"
#define random(n) (rand()%n)
#define MAXEXAM 1024			// 최대 문제 개수
#define MAXLINE 256				// 한 문제의 최대 길이(장문은 한 줄의 최대 길이)
#define SHORTNUM 10				// 단문 문제 출제 개수

#include "ExamText.h"
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("LpaTaja");
BOOL arUsed[2][3][MAXEXAM];
LOGFONT LogFont;
HFONT hFont;
TCHAR FontFace[32];
int FontHeight;
int FontWidth;
BOOL bBold;
enum tag_Lang { HAN, ENG, CODE } Lang;
enum tag_Mode { M_NONE, M_SHORT, M_LONG, M_FILE } Mode;
enum tag_Status { S_WAIT, S_EXACT, S_WRONG };
struct tag_Grid {
	int Len;					// 문자 길이(한글=2,영문=1),0이면 끝;
	TCHAR Code[2];				// 문자 코드.
	int x;						// x좌표
	tag_Status Status;			// 현재 상태. 미입력,맞음,틀림
};
BOOL bComp;
int off;
struct tag_Exam {
	TCHAR Text[MAXLINE];		// ""이면 마지막 문제
	int TypeCount;				// 총 타수
	int CharCount;				// 문자 수
	int WrongCount;				// 틀린 문자 수
	DWORD st,ed;				// 시작 시간, 끝 시간
	int Tpm;					// 분당 타이핑수
};
tag_Exam arExam[1024];
int NowExam;
struct tag_Slot {
	int x,y;					// x가 -1이면 마지막 슬롯임
	tag_Grid Grid[MAXLINE];		// 글자별 출력 위치및 상태 정보
	TCHAR buf[MAXLINE];			// 입력받은 문자열
};
tag_Slot arSlot[3];
int NowSlot;
BOOL bTypeSound,bWrongSound;
DWORD tst,ted,pst;
SYSTEMTIME pst2;
int TotalSentence;
int TotalTpm;

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
LRESULT OnCreate(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnDestroy(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnPaint(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnCommand(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnInitMenu(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnChar(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnImeComposition(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OnImeChar(HWND hWnd, WPARAM wParam, LPARAM lParam);
LRESULT OnKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);

void SetFont(TCHAR *szFace,int Height,BOOL bBold);
void InitBuf(TCHAR *buf,const TCHAR *Text);
BOOL NextExam();
void StartExam();
void OnTextChange();
void BuildGrid(tag_Grid *pGrid,TCHAR *pText);
void CompareGrid(tag_Grid *pGrid,TCHAR *pInput);
void MySetImeMode(HWND hEdit, BOOL bHan);
int GetCharCount(TCHAR *str);
void Insert(int nPos, TCHAR *str);
void Delete(int nPos, int nCount);
void SetCaret();
int GetPrevOff(int nPos);
DWORD MyGetAbsSecond(SYSTEMTIME st);
void MyAbsSecondToSystem(DWORD Abs, SYSTEMTIME &st);

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;
	
	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=NULL;
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(g_hInst,MAKEINTRESOURCE(IDI_ICON1));
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style=0;
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPED | 
		WS_CAPTION | WS_SYSMENU | WS_BORDER | WS_MINIMIZEBOX,
		CW_USEDEFAULT,CW_USEDEFAULT,800,355,
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
	case WM_INITMENU:
		return OnInitMenu(hWnd,wParam,lParam);
	case WM_CHAR:
		return OnChar(hWnd,wParam,lParam);
	case WM_IME_COMPOSITION:
		return OnImeComposition(hWnd,wParam,lParam);
	case WM_IME_CHAR:
		return OnImeChar(hWnd,wParam,lParam);
	case WM_KEYDOWN:
		return OnKeyDown(hWnd,wParam,lParam);
	case WM_IME_STARTCOMPOSITION:
		return 0;
	case WM_SETFOCUS:
		if (Mode != M_NONE) SetCaret();
		return 0;
	case WM_KILLFOCUS:
		DestroyCaret();
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

///////////////////////////////////////////////////////////////////
// 메시지 핸들러

LRESULT OnCreate(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	RECT wrt;

	hWndMain=hWnd;
	Mode=M_NONE;
	srand(GetTickCount());
	Lang=(tag_Lang)SHRegReadInt(SHCU,KEY"Option","Lang",HAN);
	FontHeight=SHRegReadInt(SHCU,KEY"Option","FontHeight",20);
	SHRegReadString(SHCU,KEY"Option","FontFace","굴림체",FontFace,32);
	bBold=SHRegReadInt(SHCU,KEY"Option","bBold",FALSE);
	SetFont(FontFace,FontHeight,bBold);
	bTypeSound=SHRegReadInt(SHCU,KEY"Option","bTypeSound",FALSE);
	bWrongSound=SHRegReadInt(SHCU,KEY"Option","bWrongSound",TRUE);
	wrt.left=SHRegReadInt(SHCU,KEY"Option","left",100);
	wrt.top=SHRegReadInt(SHCU,KEY"Option","top",100);
	SetWindowPos(hWnd,NULL,wrt.left,wrt.top,0,0,SWP_NOSIZE | SWP_NOZORDER);
	pst=GetTickCount();
	GetLocalTime(&pst2);
	return 0;
}

LRESULT OnDestroy(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	RECT wrt;
	TCHAR AppName[32],szValue[32];
	DWORD elapse;
	int Sec,Min,Hour;

	GetWindowRect(hWnd,&wrt);
	SHRegWriteInt(SHCU,KEY"Option","left",wrt.left);
	SHRegWriteInt(SHCU,KEY"Option","top",wrt.top);
	SHRegWriteInt(SHCU,KEY"Option","Lang",Lang);
	SHRegWriteInt(SHCU,KEY"Option","FontHeight",FontHeight);
	SHRegWriteString(SHCU,KEY"Option","FontFace",FontFace);
	SHRegWriteInt(SHCU,KEY"Option","bBold",bBold);
	SHRegWriteInt(SHCU,KEY"Option","bTypeSound",bTypeSound);
	SHRegWriteInt(SHCU,KEY"Option","bWrongSound",bWrongSound);

	elapse=GetTickCount()-pst;
	Sec=elapse/1000;
	Hour=Sec/3600;
	Min=(Sec % 3600)/60;
	wsprintf(AppName,"%d년 %d월 %d일 %d시 %d분",pst2.wYear,pst2.wMonth,
		pst2.wDay,pst2.wHour,pst2.wMinute);
	wsprintf(szValue,"%d시간 %d분 %d초",Hour,Min,Sec%60);
	WritePrivateProfileString(AppName,"총연습시간",szValue,"LpaTaja.ini");
	wsprintf(szValue,"%d",TotalSentence);
	WritePrivateProfileString(AppName,"총연습문장",szValue,"LpaTaja.ini");
	if (TotalSentence == 0) {
		lstrcpy(szValue,"0");
	} else {
		wsprintf(szValue,"%d",TotalTpm/TotalSentence);
	}
	WritePrivateProfileString(AppName,"평균타수",szValue,"LpaTaja.ini");
	DeleteObject(hFont);
	PostQuitMessage(0);
	return 0;
}

LRESULT OnPaint(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HFONT OldFont;
	int nSlot,i,j,idx;
	int Max,Sum,Average;
	int Len;
	TCHAR str[128];
	RECT crt,rt;
	HDC hMemDC;
	HBITMAP hBackBit,hOldBitmap;
	TCHAR *Mes="프로그래머를 위한 간단한 타자 연습 프로그램입니다.\r\n"
		"언어 메뉴에서 연습할 언어를 선택한 후 연습 메뉴에서 단문 또는 장문을 선택하십시오.\r\n"
		"옵션에서 글꼴 및 소리 사용 여부를 조정할 수 있습니다.";

	hdc=BeginPaint(hWnd, &ps);
	hMemDC=CreateCompatibleDC(hdc);
	GetClientRect(hWnd,&crt);
	hBackBit=CreateCompatibleBitmap(hdc,crt.right,crt.bottom);
	hOldBitmap=(HBITMAP)SelectObject(hMemDC,hBackBit);

	GetClientRect(hWnd,&rt);
	FillRect(hMemDC,&rt,(HBRUSH)GetStockObject(DKGRAY_BRUSH));
	InflateRect(&rt,-10,-10);
	rt.bottom=200;
	FillRect(hMemDC,&rt,(HBRUSH)GetStockObject(LTGRAY_BRUSH));
	rt.top=210;
	rt.bottom=300;
	FillRect(hMemDC,&rt,(HBRUSH)GetSysColorBrush(COLOR_WINDOW));
	SetBkMode(hMemDC,TRANSPARENT);
	if (Mode != M_NONE) {
		OldFont=(HFONT)SelectObject(hMemDC,hFont);
		for (nSlot=0;nSlot<3;nSlot++) {
			if (arSlot[nSlot].x == -1) break;
			for (i=0,idx=0;;i++) {
				// 원본 문자 출력. 탭은 무시하면 됨.
				if (arSlot[nSlot].Grid[i].Len==0) break;
				if (arSlot[nSlot].Grid[i].Code[0]!='\t') {
					switch (arSlot[nSlot].Grid[i].Status) {
					case S_WAIT:
						SetTextColor(hMemDC,RGB(0,0,0));
						break;
					case S_EXACT:
						SetTextColor(hMemDC,RGB(0,0,255));
						break;
					case S_WRONG:
						SetTextColor(hMemDC,RGB(255,0,0));
						break;
					}
					TextOut(hMemDC,arSlot[nSlot].x+arSlot[nSlot].Grid[i].x,
						arSlot[nSlot].y,arSlot[nSlot].Grid[i].Code,arSlot[nSlot].Grid[i].Len);
				}

				// 입력된 문자 출력
				if (arSlot[nSlot].buf[idx]) {
					Len=(IsDBCSLeadByte(arSlot[nSlot].buf[idx]) ? 2:1);
					if (arSlot[nSlot].buf[idx]!='\t') {
						SetTextColor(hMemDC,RGB(0,0,0));
						TextOut(hMemDC,arSlot[nSlot].x+arSlot[nSlot].Grid[i].x,
							arSlot[nSlot].y+FontHeight,arSlot[nSlot].buf+idx,Len);
					}
					idx+=Len;
				}
			}
		}
		SelectObject(hMemDC,OldFont);

		// 직전 문제의 성적 출력
		if (NowExam != 0) {
			i=NowExam-1;
			wsprintf(str,"타수(분당) : %d타",arExam[i].Tpm);
			TextOut(hMemDC,15,220,str,lstrlen(str));
			wsprintf(str,"정확도 : %d%%",(arExam[i].CharCount-arExam[i].WrongCount)*
				100/arExam[i].CharCount);
			TextOut(hMemDC,15,240,str,lstrlen(str));
			sprintf(str,"시간 : %.2f초",(arExam[i].ed-arExam[i].st)/1000.0);
			TextOut(hMemDC,15,260,str,lstrlen(str));
			for (j=0,Max=0,Sum=0;j<NowExam;j++) {
				Max=max(arExam[j].Tpm,Max);
				Sum+=arExam[j].Tpm;
			}
			Average=Sum/NowExam;
			wsprintf(str,"최대 타수(분당) : %d타",Max);
			TextOut(hMemDC,300,220,str,lstrlen(str));
			wsprintf(str,"평균 타수(분당) : %d타",Average);
			TextOut(hMemDC,300,240,str,lstrlen(str));
			sprintf(str,"경과 시간 : %.2f초",(ted-tst)/1000.0);
			TextOut(hMemDC,300,260,str,lstrlen(str));
		}
	} else {
		SetRect(&rt,20,20,700,180);
		DrawText(hMemDC,Mes,-1,&rt,DT_WORDBREAK);
	}

	BitBlt(hdc,0,0,crt.right,crt.bottom,hMemDC,0,0,SRCCOPY);
	SelectObject(hMemDC,hOldBitmap);
	DeleteObject(hBackBit);
	DeleteDC(hMemDC);
	EndPaint(hWnd, &ps);
	return 0;
}

LRESULT OnCommand(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	TCHAR Path[MAX_PATH],Cmd[MAX_PATH];
	STARTUPINFO si={0,};
	PROCESS_INFORMATION pi;

	switch (LOWORD(wParam)) {
	case IDM_LANG_HAN:
		Lang=HAN;
		StartExam();
		break;
	case IDM_LANG_ENG:
		Lang=ENG;
		StartExam();
		break;
	case IDM_LANG_CODE:
		Lang=CODE;
		StartExam();
		break;
	case IDM_LANG_VIEWLOG:
		GetWindowsDirectory(Path,MAX_PATH);
		wsprintf(Cmd,"NotePad %s\\LpaTaja.ini",Path);
		if (_access(Path,0)==0) {
			CreateProcess(NULL,Cmd,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
		break;
	case IDM_EXE_SHORT:
		Mode=M_SHORT;
		StartExam();
		break;
	case IDM_EXE_LONG:
		Mode=M_LONG;
		StartExam();
		break;
	case IDM_EXE_FILE:
		MessageBox(hWnd,"이 기능은 아직 구현되지 않았습니다.","알림",MB_OK);
		break;
	case IDM_OPT_GULIM:
		lstrcpy(FontFace,"굴림체");
		SetFont(FontFace,FontHeight,bBold);
		break;
	case IDM_OPT_BATANG:
		lstrcpy(FontFace,"바탕체");
		SetFont(FontFace,FontHeight,bBold);
		break;
	case IDM_OPT_GUNGSEO:
		lstrcpy(FontFace,"궁서체");
		SetFont(FontFace,FontHeight,bBold);
		break;
	case IDM_OPT_SMALL:
		FontHeight=16;
		SetFont(FontFace,FontHeight,bBold);
		break;
	case IDM_OPT_MEDIUM:
		FontHeight=20;
		SetFont(FontFace,FontHeight,bBold);
		break;
	case IDM_OPT_LARGE:
		FontHeight=24;
		SetFont(FontFace,FontHeight,bBold);
		break;
	case IDM_OPT_BOLD:
		bBold=!bBold;
		SetFont(FontFace,FontHeight,bBold);
		break;
	case IDM_OPT_TYPESOUND:
		bTypeSound=!bTypeSound;
		break;
	case IDM_OPT_WRONGSOUND:
		bWrongSound=!bWrongSound;
		break;
	}
	return 0;
}

LRESULT OnInitMenu(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	CheckMenuItem((HMENU)wParam,IDM_LANG_HAN,MF_BYCOMMAND | 
		(Lang == HAN ? MF_CHECKED:MF_UNCHECKED));
	CheckMenuItem((HMENU)wParam,IDM_LANG_ENG,MF_BYCOMMAND | 
		(Lang == ENG ? MF_CHECKED:MF_UNCHECKED));
	CheckMenuItem((HMENU)wParam,IDM_LANG_CODE,MF_BYCOMMAND | 
		(Lang == CODE ? MF_CHECKED:MF_UNCHECKED));
	CheckMenuItem((HMENU)wParam,IDM_OPT_GULIM,MF_BYCOMMAND | 
		(lstrcmp(FontFace,"굴림체")==0 ? MF_CHECKED:MF_UNCHECKED));
	CheckMenuItem((HMENU)wParam,IDM_OPT_BATANG,MF_BYCOMMAND | 
		(lstrcmp(FontFace,"바탕체")==0 ? MF_CHECKED:MF_UNCHECKED));
	CheckMenuItem((HMENU)wParam,IDM_OPT_GUNGSEO,MF_BYCOMMAND | 
		(lstrcmp(FontFace,"궁서체")==0 ? MF_CHECKED:MF_UNCHECKED));
	CheckMenuItem((HMENU)wParam,IDM_OPT_SMALL,MF_BYCOMMAND | 
		(FontHeight==16 ? MF_CHECKED:MF_UNCHECKED));
	CheckMenuItem((HMENU)wParam,IDM_OPT_MEDIUM,MF_BYCOMMAND | 
		(FontHeight==20 ? MF_CHECKED:MF_UNCHECKED));
	CheckMenuItem((HMENU)wParam,IDM_OPT_LARGE,MF_BYCOMMAND | 
		(FontHeight==24 ? MF_CHECKED:MF_UNCHECKED));
	CheckMenuItem((HMENU)wParam,IDM_OPT_BOLD,MF_BYCOMMAND | 
		(bBold ? MF_CHECKED:MF_UNCHECKED));
	CheckMenuItem((HMENU)wParam,IDM_OPT_TYPESOUND,MF_BYCOMMAND | 
		(bTypeSound ? MF_CHECKED:MF_UNCHECKED));
	CheckMenuItem((HMENU)wParam,IDM_OPT_WRONGSOUND,MF_BYCOMMAND | 
		(bWrongSound ? MF_CHECKED:MF_UNCHECKED));
	return 0;
}

LRESULT OnChar(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	TCHAR szChar[3];

	if (Mode == M_NONE) return 0;
	if (bTypeSound) {
		PlaySound(MAKEINTRESOURCE(IDR_TYPING),g_hInst,SND_RESOURCE|SND_ASYNC);
	}
	// 첫 문자가 입력될 때 시작 시간 기록.
	if (lstrlen(arSlot[NowSlot].buf) == 0) {
		arExam[NowExam].st=GetTickCount();
		if (NowExam==0) tst=GetTickCount();
	}
	ted=GetTickCount();
	// 개행 코드를 제외한 제어 코드는 입력받지 않는다.
	if ((wParam < ' ' && wParam != '\r') || wParam==127)
		return 0;

	// 개행 코드는 공백으로 바꾼다.
	if (wParam == '\r') wParam=' ';
	wsprintf(szChar,"%c",wParam);
	Insert(off,szChar);
	off+=lstrlen(szChar);
	bComp=FALSE;
	InvalidateRect(hWnd,NULL,TRUE);
	SetCaret();
	OnTextChange();
	return 0;
}

LRESULT OnImeComposition(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	HIMC hImc;
	TCHAR *szComp;
	int len;

	if (Mode == M_NONE) return 0;
	if (bTypeSound) {
		PlaySound(MAKEINTRESOURCE(IDR_TYPING),g_hInst,SND_RESOURCE|SND_ASYNC);
	}
	if (lstrlen(arSlot[NowSlot].buf) == 0) {
		arExam[NowExam].st=GetTickCount();
		if (NowExam==0) tst=GetTickCount();
	}
	ted=GetTickCount();
	hImc=ImmGetContext(hWnd);
	if (lParam & GCS_COMPSTR) {
		len=ImmGetCompositionString(hImc,GCS_COMPSTR,NULL,0);
		szComp=(TCHAR *)malloc(len+1);
		ImmGetCompositionString(hImc,GCS_COMPSTR,szComp,len);
		szComp[len]=0;
		if (bComp) {
			off-=2;
			Delete(off,2);
		}
		if (len == 0) {
			bComp=FALSE;
		} else {
			bComp=TRUE;
		}

		Insert(off,szComp);
		off+=len;
		ImmReleaseContext(hWnd,hImc);
		free(szComp);
		InvalidateRect(hWnd,NULL,TRUE);
		SetCaret();
	}

	return DefWindowProc(hWnd,WM_IME_COMPOSITION,wParam,lParam);
}

LRESULT OnImeChar(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	TCHAR szChar[3];

	if (Mode == M_NONE) return 0;
	if (IsDBCSLeadByte((BYTE)(wParam >> 8))) {
		szChar[0]=HIBYTE(LOWORD(wParam));
		szChar[1]=LOBYTE(LOWORD(wParam));
		szChar[2]=0;
	} else {
		szChar[0]=(BYTE)wParam;
		szChar[1]=0;
	}
	if (bComp) {
		off-=2;
		Delete(off,2);
	}
	Insert(off,szChar);
	off+=lstrlen(szChar);
	bComp=FALSE;
	InvalidateRect(hWnd,NULL,TRUE);
	SetCaret();
	OnTextChange();
	return 0;
}

LRESULT OnKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	int toff;

	if (Mode == M_NONE) return 0;
	switch (wParam) {
	case VK_BACK:
		if (off != 0) {
			// 앞쪽의 탭 문자는 지울 수 없다.
			toff=GetPrevOff(off);
			if (arSlot[NowSlot].buf[toff] == '\t') return 0;
			off=toff;
			if (IsDBCSLeadByte(arSlot[NowSlot].buf[off])) {
				Delete(off, 2);
			} else {
				Delete(off, 1);
			}
			InvalidateRect(hWnd,NULL,TRUE);
			SetCaret();
			OnTextChange();
		}
	}
	return 0;
}

///////////////////////////////////////////////////////////////////
// 여기서부터 일반 함수들

// 글꼴을 변경한다.
void SetFont(TCHAR *szFace,int Height,BOOL bBold)
{
	HDC hdc;
	HFONT OldFont;
	TEXTMETRIC tm;
	int nSlot,i,x;
	SIZE sz;

	hdc=GetDC(hWndMain);
	LogFont.lfHeight=Height;;
	LogFont.lfWidth=0;
	LogFont.lfEscapement=0;
	LogFont.lfOrientation=0;
	LogFont.lfWeight=(bBold ? FW_BOLD:FW_NORMAL);
	LogFont.lfItalic=0;
	LogFont.lfUnderline=0;
	LogFont.lfStrikeOut=0;
	LogFont.lfCharSet=HANGEUL_CHARSET;
	LogFont.lfOutPrecision=3;
	LogFont.lfClipPrecision=2;
	LogFont.lfQuality=1;
	if (lstrcmp(szFace,"굴림체")==0) {
		LogFont.lfPitchAndFamily=FF_MODERN | FIXED_PITCH;
	} else {
		LogFont.lfPitchAndFamily=FF_ROMAN | FIXED_PITCH;
	}
	lstrcpy(LogFont.lfFaceName,szFace);

	if (hFont) {
		DeleteObject(hFont);
	}
	hFont=CreateFontIndirect(&LogFont);
	OldFont=(HFONT)SelectObject(hdc,hFont);
	GetTextMetrics(hdc,&tm);
	FontWidth=tm.tmAveCharWidth;

	// 연습 중이면 그리드의 좌표를 다시 갱신한다.
	if (Mode != M_NONE) {
		for (nSlot=0;nSlot<3;nSlot++) {
			if (arSlot[nSlot].x == -1) break;
			for (i=0,x=0;;i++) {
				if (arSlot[nSlot].Grid[i].Len==0) break;
				arSlot[nSlot].Grid[i].x=x;
				GetTextExtentPoint32(hdc,arSlot[nSlot].Grid[i].Code,arSlot[nSlot].Grid[i].Len,&sz);
				x+=sz.cx;
			}
		}
		InvalidateRect(hWndMain,NULL,TRUE);
		SetCaret();
	}

	SelectObject(hdc,OldFont);
	ReleaseDC(hWndMain,hdc);
}

// 원본 텍스트 앞의 탭 문자를 복사한다.
void InitBuf(TCHAR *buf,const TCHAR *Text)
{
	const TCHAR *p=Text;
	TCHAR *b=buf;

	while (*p=='\t') {
		*b++=*p++;
	}
	*b=0;
}

// 다음 문제로 넘어간다. 다 출제했으면 TRUE 리턴
BOOL NextExam()
{
	int i;

	NowExam++;
	if (Mode == M_LONG) {
		NowSlot++;
		if (NowSlot == 3) {
			NowSlot=0;
			for (i=0;i<3;i++) {
				if (lstrlen(arExam[NowExam+i].Text) != 0) {
					arSlot[i].x=15;
					arSlot[i].y=i*60+15;
					InitBuf(arSlot[i].buf,arExam[NowExam+i].Text);
					BuildGrid(arSlot[i].Grid,arExam[NowExam+i].Text);
				} else {
					arSlot[i].x=-1;
				}
			}
		}
		if (arSlot[NowSlot].x == -1) {
			return TRUE;
		}
	} else {
		if (lstrlen(arExam[NowExam].Text) == 0) {
			return TRUE;
		}
		NowSlot=0;
		arSlot[0].x=15;
		arSlot[0].y=80;
		InitBuf(arSlot[0].buf,arExam[NowExam].Text);
		BuildGrid(arSlot[0].Grid,arExam[NowExam].Text);
		arSlot[1].x=-1;
	}
	for (off=0;arExam[NowExam].Text[off]=='\t';off++);
	SetCaret();
	InvalidateRect(hWndMain,NULL,TRUE);
	return FALSE;
}

// 필요한 양만큼 문제가 남았는지 점검하고 부족할 경우 arUsed를 리셋한다.
void CheckFreeUsed(int num)
{
	int i,n;
	TCHAR *(*ar)[MAXEXAM];

	ar=(Mode==M_SHORT ? arShort:arLong);
	for (i=0,n=0;;i++) {
		if (lstrlen(ar[Lang][i])==0) break;
		if (arUsed[Mode-1][Lang][i]==FALSE) n++;
	}

	if (n<num) {
		memset(arUsed[Mode-1][Lang],0,sizeof(BOOL)*MAXEXAM);
	}
}

// 모드와 언어에 따라 문제 출제를 시작한다.
void StartExam()
{
	int i,idx;
	TCHAR *p,*p2;

	// 문제를 골라 arExam에 작성한다. 미입력되었거나 이미 사용한 문제는 제외
	if (Mode == M_LONG) {
		CheckFreeUsed(1);
		do {
			idx=random(MAXEXAM);
		} while (lstrlen(arLong[Lang][idx])==0 || arUsed[Mode-1][Lang][idx]==TRUE);
		arUsed[Mode-1][Lang][idx]=TRUE;

		// 장문을 arExam에 잘라 넣는다.
		p=arLong[Lang][idx];
		for (i=0;*p;i++) {
			for (p2=p;*p2!='\r' && *p2;p2++) {;}
			lstrcpyn(arExam[i].Text,p,p2-p+1);
			for (p=p2;*p=='\r' || *p=='\n';p++) {;}
		}
	} else {
		CheckFreeUsed(SHORTNUM);
		for (i=0;i<SHORTNUM;i++) {
			do {
				idx=random(MAXEXAM);
			} while (lstrlen(arShort[Lang][idx])==0 || arUsed[Mode-1][Lang][idx]==TRUE);
			arUsed[Mode-1][Lang][idx]=TRUE;

			lstrcpy(arExam[i].Text,arShort[Lang][idx]);
		}
	}

	// 문제 배열의 끝 표시를 하고 타수,글자수를 계산한다.
	lstrcpy(arExam[i].Text,"");
	for (i=0;lstrlen(arExam[i].Text)!=0;i++) {
		arExam[i].TypeCount=GetHanStringCount(arExam[i].Text);
		arExam[i].CharCount=GetCharCount(arExam[i].Text);
		arExam[i].st=arExam[i].ed=0;
	}

	// 첫 문제 출력. NextExam에서 BuildGrid를 강제로 호출하도록 한다.
	MySetImeMode(hWndMain,(Lang == HAN));
	NowExam=-1;
	NowSlot=2;
	NextExam();
}

// 텍스트가 바뀔 때마다 호출된다. 단, 한글 조립중일 때는 호출되지 않는다.
void OnTextChange()
{
	tag_Exam *pEx=&arExam[NowExam];
	int i,w1,w2;

	// 끝까지 입력했으면 타수, 정확도를 계산하고 다음 문제로 넘어간다.
	if (GetCharCount(arSlot[NowSlot].buf) > GetCharCount(pEx->Text)) {
		pEx->ed=GetTickCount();
		pEx->Tpm=pEx->TypeCount*60000/(pEx->ed-pEx->st);
		TotalTpm+=pEx->Tpm;
		TotalSentence++;
		for (i=0,pEx->WrongCount=0;arSlot[NowSlot].Grid[i].Code[0];i++) {
			if (arSlot[NowSlot].Grid[i].Status==S_WRONG) pEx->WrongCount++;
		}

		if (NextExam()) {
			if (MessageBox(hWndMain,"연습이 끝났습니다. 계속 하시겠습니까?",
				"질문",MB_YESNO)==IDYES) {
				StartExam();
			} else {
				Mode=M_NONE;
				DestroyCaret();
			}
		}
	// 아니면 비교 결과를 작성한다.
	} else {
		for (i=0,w1=0;arSlot[NowSlot].Grid[i].Code[0];i++) {
			if (arSlot[NowSlot].Grid[i].Status == S_WRONG) w1++;
		}
		CompareGrid(arSlot[NowSlot].Grid,arSlot[NowSlot].buf);
		for (i=0,w2=0;arSlot[NowSlot].Grid[i].Code[0];i++) {
			if (arSlot[NowSlot].Grid[i].Status == S_WRONG) w2++;
		}
		if (w1 < w2 && bWrongSound) {
			PlaySound(MAKEINTRESOURCE(IDR_WRONG), g_hInst, SND_RESOURCE | SND_ASYNC);
		}
	}
	InvalidateRect(hWndMain,NULL,TRUE);
}

// Text로부터 각 문자를 잘라 그리드를 만든다.
void BuildGrid(tag_Grid *pGrid,TCHAR *pText)
{
	HDC hdc;
	TCHAR *p;
	int i,x;
	HFONT OldFont;
	SIZE sz;

	hdc=GetDC(hWndMain);
	OldFont=(HFONT)SelectObject(hdc,hFont);
	for (p=pText,i=0,x=0;*p;i++) {
		if (IsDBCSLeadByte(*p)) {
			pGrid[i].Len=2;
			pGrid[i].Code[0]=*p++;
			pGrid[i].Code[1]=*p++;
		} else {
			pGrid[i].Len=1;
			pGrid[i].Code[0]=*p++;
		}
		pGrid[i].x=x;
		if (pGrid[i].Code[0] == '\t') {
			x=(x/32+1)*32;
		} else {
			GetTextExtentPoint32(hdc,p-pGrid[i].Len,pGrid[i].Len,&sz);
			x+=sz.cx;
		}
		pGrid[i].Status=S_WAIT;
	}
	// 끝 처리 하되 마지막 널 문자도 위치를 가져야 한다. 그래야 원문을 다 입력했을 때의 캐럿 위치가 제대로 잡힌다.
	pGrid[i].Len=0;
	pGrid[i].x=x;
	SelectObject(hdc,OldFont);
	ReleaseDC(hWndMain,hdc);
}

void CompareGrid(tag_Grid *pGrid,TCHAR *pInput)
{
	int i,idx;

	// 입력된 부분에 대해 판정
	for (i=0,idx=0;;i++) {
		if (pGrid[i].Code[0] == 0) break;
		// 입력된 끝까지 비교한다. 한글 조립중일 때는 이 함수를 호출하지 않으므로 신경쓸 필요없다.
		if (pInput[idx] == 0) break;

		if (strncmp(pGrid[i].Code,pInput+idx,pGrid[i].Len)==0) {
			pGrid[i].Status=S_EXACT;
		} else {
			pGrid[i].Status=S_WRONG;
		}
		// 글자 단위로 비교한다.
		if (IsDBCSLeadByte(pInput[idx])) {
			idx+=2;
		} else {
			idx+=1;
		}
	}

	// 나머지 뒷부분은 아직 입력되지 않은 것으로
	for (;;i++) {
		if (pGrid[i].Code[0] == 0) break;
		pGrid[i].Status=S_WAIT;
	}
}

void MySetImeMode(HWND hEdit, BOOL bHan)
{
	HIMC hImc;
	hImc=ImmGetContext(hEdit);

	if (bHan == TRUE) {
		ImmSetConversionStatus(hImc,IME_CMODE_NATIVE,IME_SMODE_NONE);
	} else {
		ImmSetConversionStatus(hImc,0,IME_SMODE_NONE);
	}

	ImmReleaseContext(hEdit,hImc );
}

int GetCharCount(TCHAR *str)
{
	int i,idx;

	for (i=0,idx=0;;i++) {
		if (str[idx] == 0) break;
		if (IsDBCSLeadByte(str[idx])) {
			idx+=2;
		} else {
			idx+=1;
		}
	}
	return i;
}

void Insert(int nPos, TCHAR *str)
{
	int len;
	int movelen;

	len=lstrlen(str);
	if (len==0) return;
	movelen=lstrlen(arSlot[NowSlot].buf+nPos)+1;
	memmove(arSlot[NowSlot].buf+nPos+len,arSlot[NowSlot].buf+nPos,movelen);
	memcpy(arSlot[NowSlot].buf+nPos,str,len);
}

void Delete(int nPos, int nCount)
{
	int movelen;

	if (nCount == 0) return;
	if (lstrlen(arSlot[NowSlot].buf) < nPos+nCount) return;

	movelen=lstrlen(arSlot[NowSlot].buf+nPos+nCount)+1;
	memmove(arSlot[NowSlot].buf+nPos, arSlot[NowSlot].buf+nPos+nCount, movelen);
}

void SetCaret()
{
	int caretwidth;
	int Count;

	Count=GetCharCount(arSlot[NowSlot].buf);
	if (bComp) {
		Count--;
		caretwidth=FontWidth*2;
	} else {
		caretwidth=2;
	}
	CreateCaret(hWndMain,NULL,caretwidth,FontHeight);
	ShowCaret(hWndMain);

	SetCaretPos(arSlot[NowSlot].x+arSlot[NowSlot].Grid[Count].x,arSlot[NowSlot].y+FontHeight);
}

int GetPrevOff(int nPos)
{
	int n, size;

	if (nPos==0) return 0;
	for (n=0;;) {
		size = (IsDBCSLeadByte(arSlot[NowSlot].buf[n]) ? 2:1);
		n+=size;
		if (n >= nPos)
			break;
	}
	return n-size;
}

DWORD MyGetAbsSecond(SYSTEMTIME st)
{
	INT64 i64;
	FILETIME fst;

	SystemTimeToFileTime(&st,&fst);
	i64=(((INT64)fst.dwHighDateTime) << 32) + fst.dwLowDateTime;
	i64 = i64 / 10000000 - (INT64)145731 * 86400;

	return (DWORD)i64;
}

void MyAbsSecondToSystem(DWORD Abs, SYSTEMTIME &st)
{
	INT64 i64;
	FILETIME fst;

	i64=(Abs + (INT64)145731 * 86400)*10000000;
	fst.dwHighDateTime = (DWORD)(i64 >> 32);
	fst.dwLowDateTime = (DWORD)(i64 & 0xffffffff);
	FileTimeToSystemTime(&fst, &st);
}