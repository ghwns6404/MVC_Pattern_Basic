#include <windows.h>
#include "richedit.h"
#include "resource.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("RichTest");
HWND hRich;

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;

	HMODULE hMod;
	
	// 리치 에디트가 정의되어 있는 DLL을 로드한다.
	hMod=LoadLibrary("Riched20.dll");

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

	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	// DLL을 해제한다.
	FreeLibrary(hMod);
	return (int)Message.wParam;
}

// 글자의 속성 설정
void SetChar()
{
	CHARFORMAT2 cf;
	CHOOSEFONT CFT;
	LOGFONT lf;
	HDC hdc;

	// 현재 설정된 문자 속성을 읽어온다.
	memset(&cf,0,sizeof(CHARFORMAT2));
	cf.cbSize=sizeof(CHARFORMAT2);
	SendMessage(hRich,EM_GETCHARFORMAT,1,(LPARAM)&cf);

	// 글꼴 공통 대화상자로 글자 속성을 입력받는다.
	memset(&CFT, 0, sizeof(CHOOSEFONT));
	CFT.lStructSize = sizeof(CHOOSEFONT);
	CFT.hwndOwner=hWndMain;
	lstrcpy(lf.lfFaceName, cf.szFaceName);
	if (cf.dwEffects & CFE_ITALIC) lf.lfItalic=TRUE; else lf.lfItalic=FALSE;
	if (cf.dwEffects & CFE_BOLD) lf.lfWeight=FW_BOLD; else lf.lfWeight=FW_NORMAL;
	if (cf.dwEffects & CFE_UNDERLINE) lf.lfUnderline=TRUE; else lf.lfUnderline=FALSE;
	if (cf.dwEffects & CFE_STRIKEOUT) lf.lfStrikeOut=TRUE; else lf.lfStrikeOut=FALSE;
	CFT.rgbColors=cf.crTextColor;
	hdc=GetDC(hWndMain);
	lf.lfHeight=-MulDiv(cf.yHeight/20,GetDeviceCaps(hdc,LOGPIXELSY),72);
	ReleaseDC(hWndMain,hdc);
	CFT.lpLogFont=&lf;
	CFT.Flags=CF_EFFECTS | CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;
	if (ChooseFont(&CFT)==FALSE) {
		return;
	}

	// 입력받은 글자 속성을 리치 에디트에 설정한다.
	memset(&cf,0,sizeof(CHARFORMAT2));
	cf.cbSize=sizeof(CHARFORMAT2);
	cf.dwMask=CFM_COLOR | CFM_FACE | CFM_SIZE | CFM_BOLD | CFM_ITALIC | 
		CFM_UNDERLINE | CFM_STRIKEOUT;
	cf.crTextColor=CFT.rgbColors;
	cf.yHeight=CFT.iPointSize*2;
	lstrcpy(cf.szFaceName,lf.lfFaceName);
	cf.dwEffects=0;
	if (lf.lfItalic) cf.dwEffects |= CFE_ITALIC;
	if (lf.lfWeight == FW_BOLD) cf.dwEffects |= CFE_BOLD;
	if (lf.lfUnderline) cf.dwEffects |= CFE_UNDERLINE;
	if (lf.lfStrikeOut) cf.dwEffects |= CFE_STRIKEOUT;

	SendMessage(hRich,EM_SETCHARFORMAT,SCF_SELECTION,(LPARAM)&cf);
}

// 문단 속성 설정 대화상자
BOOL CALLBACK ParaDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	static PARAFORMAT2 *pf;

	switch (iMessage) {
	case WM_INITDIALOG:
		pf=(PARAFORMAT2 *)lParam;
		if (pf->wAlignment==PFA_LEFT) CheckDlgButton(hDlg,IDC_ALLEFT,BST_CHECKED);
		if (pf->wAlignment==PFA_RIGHT) CheckDlgButton(hDlg,IDC_ALRIGHT,BST_CHECKED);
		if (pf->wAlignment==PFA_CENTER) CheckDlgButton(hDlg,IDC_ALCENTER,BST_CHECKED);

		if (pf->bLineSpacingRule==0) CheckDlgButton(hDlg,IDC_LINE1,BST_CHECKED);
		if (pf->bLineSpacingRule==1) CheckDlgButton(hDlg,IDC_LINE1P5,BST_CHECKED);
		if (pf->bLineSpacingRule==2) CheckDlgButton(hDlg,IDC_LINE2,BST_CHECKED);

		SetDlgItemInt(hDlg,IDC_INFIRST,pf->dxStartIndent,FALSE);
		SetDlgItemInt(hDlg,IDC_INLEFT,pf->dxOffset,FALSE);
		SetDlgItemInt(hDlg,IDC_INRIGHT,pf->dxRightIndent,FALSE);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDCANCEL:
			EndDialog(hDlg,IDCANCEL);
			return TRUE;
		case IDOK:
			if (IsDlgButtonChecked(hDlg,IDC_ALLEFT)) pf->wAlignment=PFA_LEFT;
			if (IsDlgButtonChecked(hDlg,IDC_ALRIGHT)) pf->wAlignment=PFA_RIGHT;
			if (IsDlgButtonChecked(hDlg,IDC_ALCENTER)) pf->wAlignment=PFA_CENTER;

			if (IsDlgButtonChecked(hDlg,IDC_LINE1)) pf->bLineSpacingRule=0;
			if (IsDlgButtonChecked(hDlg,IDC_LINE1P5)) pf->bLineSpacingRule=1;
			if (IsDlgButtonChecked(hDlg,IDC_LINE2)) pf->bLineSpacingRule=2;

			pf->dxStartIndent=GetDlgItemInt(hDlg,IDC_INFIRST,NULL,FALSE);
			pf->dxOffset=GetDlgItemInt(hDlg,IDC_INLEFT,NULL,FALSE);
			pf->dxRightIndent=GetDlgItemInt(hDlg,IDC_INRIGHT,NULL,FALSE);
			EndDialog(hDlg,IDOK);
		}
		return FALSE;
	}
	return FALSE;
}

// 문단 속성 설정
void SetPara()
{
	PARAFORMAT2 pf;

	// 현재 설정된 문단 속성을 읽어온다.
	memset(&pf,0,sizeof(PARAFORMAT2));
	pf.cbSize=sizeof(PARAFORMAT2);
	pf.dwMask=PFM_ALIGNMENT | PFM_OFFSETINDENT | PFM_OFFSET | 
		PFM_RIGHTINDENT | PFM_LINESPACING;
	SendMessage(hRich,EM_GETPARAFORMAT,0,(LPARAM)&pf);

	// 새로운 문단 속성을 입력받아 설정한다.
	if (DialogBoxParam(g_hInst, MAKEINTRESOURCE(IDD_DLGPARA), hWndMain, 
		ParaDlgProc, (LPARAM)&pf)==IDOK) {
		pf.dwMask=PFM_ALIGNMENT | PFM_OFFSETINDENT | PFM_OFFSET | 
			PFM_RIGHTINDENT | PFM_LINESPACING;
		SendMessage(hRich,EM_SETPARAFORMAT,0,(LPARAM)&pf);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage) {
	case WM_CREATE:
		hRich=CreateWindow(RICHEDIT_CLASS,NULL,WS_CHILD | WS_VISIBLE | 
			WS_BORDER | ES_MULTILINE | WS_VSCROLL,
			0,0,0,0,hWnd,(HMENU)0,g_hInst,NULL);
		SendMessage(hRich,EM_HIDESELECTION,0,0);
		SetWindowText(hRich,"이 컨트롤은 글자와 문단의 다양한 서식을 표현할 "
			"수 있는 리치 에디트 컨트롤입니다\r\n"
			"문단 서식 설정을 테스트하기 위해 에디트의 폭을 300픽셀로 고정해 "
			"두었으며 메뉴에는 클립보드 관련 기능과 글자 서식, 문단 서식을 "
			"설정할 수 있는 항목이 있으므로 테스트해 보시기 바랍니다");
		return 0;
	case WM_SIZE:
		MoveWindow(hRich,0,0,min(LOWORD(lParam),300),HIWORD(lParam),TRUE);
		return 0;
	case WM_SETFOCUS:
		SetFocus(hRich);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDM_EXIT:
			DestroyWindow(hWnd);
			return 0;
		case IDM_CUT:
			SendMessage(hRich,WM_CUT,0,0);
			return 0;
		case IDM_COPY:
			SendMessage(hRich,WM_COPY,0,0);
			return 0;
		case IDM_PASTE:
			SendMessage(hRich,WM_PASTE,0,0);
			return 0;
		case IDM_SETCHAR:
			SetChar();
			return 0;
		case IDM_SETPARA:
			SetPara();
			return 0;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
