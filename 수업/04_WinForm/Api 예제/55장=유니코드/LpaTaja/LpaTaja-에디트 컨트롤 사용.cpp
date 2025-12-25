// 에디트 컨트롤을 사용했을 때의 문제점 - 폰트나 위치 입력 등은 별 문제가 없다.
// 그리고 팝업 메뉴나 좌우 이동 가능 등은 서브클래싱으로 해결할 수 있다.
// 1.제일 마지막 문자의 조립중 여부를 판단할 수 없다.
// 2.틀린 문자를 입력할 경우 폭이 맞지 않고 한글 하나에 영문 두 글자가 대응된다.
#include <windows.h>
#include <imm.h>
#include "resource.h"

#define ID_EDIT 100
#define random(n) (rand()%n)
#define MAXAR 256

TCHAR *arShort[3][MAXAR]={
	{
	"돌아만 간다고 해서 프로그램이 아니다.",
	"강사 말을 잘 들으면 자다가도 떡이 생긴다.",
	"강의실에서 게임을 해서는 안된다.",
	"과제는 스스로의 힘으로 풀어야 한다.",
	"게임만 시작하면 꼭 강사가 나타난다.",
	"못된 수강생 엉덩이에 뿔난다.",
	"마누라",
	},
	{
	"great LPA campus",
	"study hard",
	"Microsoft Windows",
	"I love you very much",
	"see you tomorrow",
	"merry christmas and happy new year",
	"welcome to korea",
	"The following topics explain various conventions",
	"An edit control is a rectangular control window typically used in a dialog box to permit the user to enter and edit text by typing on the keyboard.",
	},
	{
	"for (i=0;i<64;i++)",
	"switch (arTable[choice]) {",
	"LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);",
	"LPCTSTR lpszClass=TEXT(\"LpaTaja\");",
	"WndClass.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);",
	"while(GetMessage(&Message,0,0,0)) {",
	"return (int)Message.wParam;",
	"hdc=BeginPaint(hWnd,&ps);",
	"return(DefWindowProc(hWnd,iMessage,wParam,lParam));",
	}
};
BOOL arUsed[MAXAR];

HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("LpaTaja");
enum tag_Language { HAN, ENG, CODE };
tag_Language Language;
HWND hEdit;
LOGFONT LogFont;
HFONT hFont;
enum tag_Mode { M_NONE, M_SHORT, M_LONG, M_FILE };
tag_Mode Mode;
int Remain;
TCHAR *pText;
enum tag_Status { S_WAIT, S_EXACT, S_WRONG };
struct tag_Grid {
	BOOL Len;
	TCHAR Code[2];
	int x;
	tag_Status Status;
};
tag_Grid Grid[256];

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
LRESULT OnCreate(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnDestroy(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnPaint(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnCommand(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnInitMenu(HWND hWnd,WPARAM wParam,LPARAM lParam);
LRESULT OnSetFocus(HWND hWnd,WPARAM wParam,LPARAM lParam);

void MySetImeMode(HWND hEdit, BOOL bHan);
void OnEnChange();
void BuildGrid(TCHAR *pText);
void CompareGrid(TCHAR *pInput);
void SelectExam();

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
	WndClass.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	
	while(GetMessage(&Message,0,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch(iMessage) {
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
	case WM_SETFOCUS:
		return OnSetFocus(hWnd,wParam,lParam);
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

LRESULT OnCreate(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;

	hWndMain=hWnd;
	srand(GetTickCount());
	hEdit=CreateWindow("edit",NULL,WS_CHILD,
		0,0,0,0,hWnd,(HMENU)ID_EDIT,g_hInst,NULL);
	hdc=GetDC(hWnd);
	LogFont.lfHeight=20*GetDeviceCaps(hdc,LOGPIXELSY)/72;
	ReleaseDC(hWnd,hdc);
	LogFont.lfWidth=0;
	LogFont.lfEscapement=0;
	LogFont.lfOrientation=0;
	LogFont.lfWeight=FW_BOLD;
	LogFont.lfItalic=0;
	LogFont.lfUnderline=0;
	LogFont.lfStrikeOut=0;
	LogFont.lfCharSet=HANGEUL_CHARSET;
	LogFont.lfOutPrecision=3;
	LogFont.lfClipPrecision=2;
	LogFont.lfQuality=1;
	LogFont.lfPitchAndFamily=FF_MODERN | FIXED_PITCH;	// 반드시 고정폭
	lstrcpy(LogFont.lfFaceName,"굴림체");
	hFont=CreateFontIndirect(&LogFont);
	SendMessage(hEdit,WM_SETFONT,(WPARAM)hFont,MAKELPARAM(FALSE,0));
	Language=ENG;
	Mode=M_NONE;
#ifdef _DEBUG
	SendMessage(hWnd,WM_COMMAND,MAKEWPARAM(IDM_EXE_SHORT,0),0);
#endif
	return 0;
}

LRESULT OnDestroy(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	DeleteObject(hFont);
	PostQuitMessage(0);
	return 0;
}

LRESULT OnPaint(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HFONT OldFont;
	int i;

	hdc=BeginPaint(hWnd, &ps);
	if (Mode != M_NONE && pText) {
		OldFont=(HFONT)SelectObject(hdc,hFont);
		for (i=0;;i++) {
			if (Grid[i].Code[0]==0) break;
			switch (Grid[i].Status) {
			case S_WAIT:
				SetTextColor(hdc,RGB(0,0,0));
				break;
			case S_EXACT:
				SetTextColor(hdc,RGB(0,0,255));
				break;
			case S_WRONG:
				SetTextColor(hdc,RGB(255,0,0));
				break;
			}
			TextOut(hdc,Grid[i].x+10,40,Grid[i].Code,Grid[i].Len);
		}
		SelectObject(hdc,OldFont);
	}
	EndPaint(hWnd, &ps);
	return 0;
}

LRESULT OnCommand(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	switch (LOWORD(wParam)) {
	case IDM_LANG_HAN:
		Language=HAN;
		break;
	case IDM_LANG_ENG:
		Language=ENG;
		break;
	case IDM_LANG_CODE:
		Language=CODE;
		break;
	case IDM_EXE_SHORT:
		Mode=M_SHORT;
		Remain=10;
		memset(arUsed,FALSE,sizeof(arUsed));
		SelectExam();
		ShowWindow(hEdit,SW_SHOW);
		SetWindowPos(hEdit,NULL,10,100,1800,50,SWP_NOZORDER);
		if (Language == HAN) MySetImeMode(hEdit,TRUE);
		SetFocus(hEdit);
		InvalidateRect(hWnd,NULL,TRUE);
		break;
	case ID_EDIT:
		if (HIWORD(wParam) == EN_CHANGE) {
			OnEnChange();
		}
		break;
	}
	return 0;
}

LRESULT OnInitMenu(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	CheckMenuItem((HMENU)wParam,IDM_LANG_HAN,MF_BYCOMMAND | 
		(Language == HAN ? MF_CHECKED:MF_UNCHECKED));
	CheckMenuItem((HMENU)wParam,IDM_LANG_ENG,MF_BYCOMMAND | 
		(Language == ENG ? MF_CHECKED:MF_UNCHECKED));
	CheckMenuItem((HMENU)wParam,IDM_LANG_CODE,MF_BYCOMMAND | 
		(Language == CODE ? MF_CHECKED:MF_UNCHECKED));
	return 0;
}

LRESULT OnSetFocus(HWND hWnd,WPARAM wParam,LPARAM lParam)
{
	if (IsWindowVisible(hEdit)) {
		SetFocus(hEdit);
	}
	return 0;
}

void SelectExam()
{
	int idx;

	for (;;) {
		idx=random(MAXAR);
		if (arShort[Language][idx] == NULL) continue;
		if (lstrlen(arShort[Language][idx]) == 0) continue;
		if (arUsed[idx]==TRUE) continue;
		break;
	}
	arUsed[idx]=TRUE;
	pText=arShort[Language][idx];
	BuildGrid(pText);
	Remain--;
}

void OnEnChange()
{
	// 버퍼를 깨끗하게 비울 것. CompareGrid에서 Text 전체를 점검하며 뒤쪽을 미리 볼 수도 있므로 쓰레기가 있어서는 안된다.
	TCHAR Text[256]={0,};
	DWORD Sel;
	TCHAR str[128];

	Sel=SendMessage(hEdit,EM_GETSEL,0,0);
	wsprintf(str,"%x-%d",Sel,SendMessage(hEdit,EM_GETIMESTATUS,EMSIS_COMPOSITIONSTRING,0));
	SetWindowText(hWndMain,str);
	GetWindowText(hEdit,Text,256);
	if (lstrlen(Text) > lstrlen(pText)) {
		SelectExam();
		SetWindowText(hEdit,"");
	} else {
		CompareGrid(Text);
	}
	InvalidateRect(hWndMain,NULL,TRUE);
}

void BuildGrid(TCHAR *pText)
{
	HDC hdc;
	TCHAR *p;
	int i,x;
	HFONT OldFont;
	SIZE sz;

	// 문자열을 그리드로 분리한다.
	hdc=GetDC(hWndMain);
	OldFont=(HFONT)SelectObject(hdc,hFont);
	for (p=pText,i=0,x=0;*p;i++) {
		if (IsDBCSLeadByte(*p)) {
			Grid[i].Len=2;
			Grid[i].Code[0]=*p++;
			Grid[i].Code[1]=*p++;
		} else {
			Grid[i].Len=1;
			Grid[i].Code[0]=*p++;
		}
		Grid[i].x=x;
		GetTextExtentPoint32(hdc,p-Grid[i].Len,Grid[i].Len,&sz);
		x+=sz.cx;
		Grid[i].Status=S_WAIT;
	}
	Grid[i].Code[0]=0;
	SelectObject(hdc,OldFont);
	ReleaseDC(hWndMain,hdc);
}

void CompareGrid(TCHAR *pInput)
{
	int i,idx;

	// 입력된 부분에 대해 판정
	for (i=0,idx=0;;i++) {
		if (Grid[i].Code[0] == 0) break;
		// 한글이 아닐 때는 입력된 끝까지 비교한다.
		if (Language != HAN) if (pInput[idx] == 0) break;
		if (strncmp(Grid[i].Code,pInput+idx,Grid[i].Len)==0) {
			Grid[i].Status=S_EXACT;
		} else {
			Grid[i].Status=S_WRONG;
		}
		idx+=Grid[i].Len;
		// 한글인 경우 조립중인 문자는 제외한다. - 마지막 문자가 조립중인지를 정확히 알 방법이 없음
		if (Language == HAN) if (pInput[idx] == 0) break;
	}

	// 나머지 뒷부분은 아직 입력되지 않은 것으로
	for (;;i++) {
		if (Grid[i].Code[0] == 0) break;
		Grid[i].Status=S_WAIT;
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
