#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("EnumFont");

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
	WndClass.hIcon=LoadIcon(g_hInst,MAKEINTRESOURCE(IDI_ICON1));
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1);
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

// 리스트 박스의 ID
#define MAXFONT 1000
#define ID_LISTBOX 100
#define ID_EDIT 101

// 전역 변수
LOGFONT logfont[MAXFONT];			// 폰트 정보 배열
int arFontType[MAXFONT];			// 폰트 타입 배열
int num;							// 발견된 폰트 수
HWND hList,hEdit;					// 리스트 박스 
int NowFont;						// 현재 선택된 폰트 인덱스

// 발견된 폰트의 정보를 배열에 복사한다.
int CALLBACK EnumFamCallBack(ENUMLOGFONT FAR *lpelf, NEWTEXTMETRIC FAR *lpntm, 
							 int FontType, LPARAM lParam)
{
	// 최대 MAXFONT개까지의 폰트를 조사할 수 있다. 
	if (num < MAXFONT) {
		logfont[num] = lpelf->elfLogFont;
		arFontType[num] = FontType;
		num++;
		return TRUE;
	} else {
		return FALSE;
	}
}

// 설치되어 있는 폰트의 목록을 조사한다.
void ReEnum()
{
	HDC hdc;
	int i;

	num=0;
	NowFont=-1;
	hdc=GetDC(hWndMain);
	EnumFontFamilies(hdc, NULL, (FONTENUMPROC)EnumFamCallBack, (LPARAM)NULL);
	ReleaseDC(hWndMain, hdc);

	// 발견된 모든 폰트의 타입 페이스를 리스트 박스에 출력한다.
	SendMessage(hList, LB_RESETCONTENT,0,0);
	for (i=0;i<num;i++) {
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)logfont[i].lfFaceName);
	}
}

void GetCharSetStr(int CharSet,TCHAR *str)
{
	switch (CharSet) {
		case 0  :lstrcpy(str, "ANSI_CHARSET");break;
		case 1  :lstrcpy(str, "DEFAULT_CHARSET");break;
		case 2  :lstrcpy(str, "SYMBOL_CHARSET");break;
		case 128:lstrcpy(str, "SHIFTJIS_CHARSET");break;
		case 129:lstrcpy(str, "HANGUL_CHARSET");break;
		case 136:lstrcpy(str, "CHINESEBIG5_CHARSET");break;
		case 255:lstrcpy(str, "OEM_CHARSET");break;
		case 130:lstrcpy(str, "JOHAB_CHARSET");break;
		case 177:lstrcpy(str, "HEBREW_CHARSET");break;
		case 178:lstrcpy(str, "ARABIC_CHARSET");break;
		case 161:lstrcpy(str, "GREEK_CHARSET");break;
		case 162:lstrcpy(str, "TURKISH_CHARSET");break;
		case 163:lstrcpy(str, "VIETNAMESE_CHARSET");break;
		case 222:lstrcpy(str, "THAI_CHARSET");break;
		case 238:lstrcpy(str, "EASTEUROPE_CHARSET");break;
		case 204:lstrcpy(str, "RUSSIAN_CHARSET");break;
		case 77:lstrcpy(str, "MAC_CHARSET");break;
		case 186:lstrcpy(str, "BALTIC_CHARSET");break;
		default:lstrcpy(str, "Etc");break;
	}
}

void GetPitchFamStr(int Pitch,TCHAR *str)
{
	switch (Pitch & 0x0f) {
		case 0:lstrcpy(str, "DEFAULT_PITCH");
			break;
		case 1:lstrcpy(str, "FIXED_PITCH");
			break;
		case 2:lstrcpy(str, "VARIABLE_PITCH");
			break;
		default:lstrcpy(str, "Etc");
	}
	switch (Pitch & 0xf0) {
		case 0x00:lstrcat(str, " | FF_DONTCARE");
			break;
		case 0x10:lstrcat(str, " | FF_ROMAN");
			break;
		case 0x20:lstrcat(str, " | FF_SWISS");
			break;
		case 0x30:lstrcat(str, " | FF_MODERN");
			break;
		case 0x40:lstrcat(str, " | FF_SCRIPT");
			break;
		case 0x50:lstrcat(str, " | FF_DECORATIVE");
			break;
	}
}

void MakeCreateFontStr()
{
	TCHAR Argu[128];
	TCHAR Api[1024];
	TCHAR Mfc[1024];
	TCHAR CharSetStr[32];
	TCHAR PitchFamStr[32];

	GetCharSetStr(logfont[NowFont].lfCharSet,CharSetStr);
	GetPitchFamStr(logfont[NowFont].lfPitchAndFamily,PitchFamStr);

	wsprintf(Argu,"(20,0,0,0,0,0,0,0,%s,3,2,1,\r\n\t%s,TEXT(\"%s\"));",
		CharSetStr,PitchFamStr,logfont[NowFont].lfFaceName);
	wsprintf(Api,"//API\r\nHFONT hFont,hOldFont;\r\nhFont=CreateFont%s\r\n"
		"hOldFont=(HFONT)SelectObject(hdc,hFont);\r\n"
		"// Use Font\r\nSelectObject(hdc,hOldFont);\r\n"
		"DeleteObject(hFont);\r\n\r\n",Argu);
	wsprintf(Mfc,"//MFC\r\nCFont Font,*pOldFont;\r\nFont.CreateFont%s\r\n"
		"pOldFont=pDC->SelectObject(&Font);\r\n"
		"// Use Font\r\npDC->SelectObject(pOldFont);\r\n",Argu);
	lstrcat(Api,Mfc);
		
	SetWindowText(hEdit,Api);
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int TabPos[]={250};
	TCHAR str[256], str2[128];
	HFONT hFont, OldFont;
	switch (iMessage) {
	// 실행 시작시에 설치된 모든 폰트를 조사하여 배열에 집어 넣는다.
	case WM_CREATE:
		hList=CreateWindowEx(WS_EX_CLIENTEDGE,"listbox",NULL,WS_CHILD | WS_VISIBLE |
			WS_VSCROLL | LBS_NOTIFY,
			10,10,250,400,hWnd,(HMENU)ID_LISTBOX,g_hInst,NULL);
		hEdit=CreateWindowEx(WS_EX_CLIENTEDGE,"edit",NULL,WS_CHILD | WS_VISIBLE | 
			ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL,
			10,420,600,200,hWnd,(HMENU)ID_EDIT,g_hInst,NULL);
		ReEnum();
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		// 리스트 박스에서 타입 페이스를 선택하면 해당 폰트의 정보를 보여준다.
		case ID_LISTBOX:
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				NowFont=SendMessage(hList, LB_GETCURSEL,0,0);
				MakeCreateFontStr();
				InvalidateRect(hWnd, NULL, TRUE);
				break;
			}
			break;
		case ID_FILE_REENUM:
			ReEnum();
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case ID_FILE_ABOUT:
			MessageBox(hWnd, "시스템에 설치되어 있는 폰트 목록을 조사하는 프로그램입니다", 
				"EnumFont", MB_OK);
			break;
		case ID_FILE_EXIT:
			DestroyWindow(hWnd);
			break;
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		SetBkMode(hdc,TRANSPARENT);
		if (NowFont != -1) {
			wsprintf(str, "Height\t: %d", logfont[NowFont].lfHeight);
			TabbedTextOut(hdc,300,20,str,lstrlen(str),1,TabPos,0);

			wsprintf(str, "Width\t: %d", logfont[NowFont].lfWidth);
			TabbedTextOut(hdc,300,40,str,lstrlen(str),1,TabPos,0);

			wsprintf(str, "Escapement\t: %d", logfont[NowFont].lfEscapement);
			TabbedTextOut(hdc,300,60,str,lstrlen(str),1,TabPos,0);

			wsprintf(str, "Orientation\t: %d", logfont[NowFont].lfOrientation);
			TabbedTextOut(hdc,300,80,str,lstrlen(str),1,TabPos,0);

			wsprintf(str, "Weight\t: %d", logfont[NowFont].lfWeight);
			TabbedTextOut(hdc,300,100,str,lstrlen(str),1,TabPos,0);

			wsprintf(str, "Italic\t: %s", logfont[NowFont].lfItalic ? "TRUE":"FALSE");
			TabbedTextOut(hdc,300,120,str,lstrlen(str),1,TabPos,0);

			wsprintf(str, "Underline\t: %s", logfont[NowFont].lfUnderline ? "TRUE":"FALSE");
			TabbedTextOut(hdc,300,140,str,lstrlen(str),1,TabPos,0);

			wsprintf(str, "StrikeOut\t: %s", logfont[NowFont].lfStrikeOut ? "TRUE":"FALSE");
			TabbedTextOut(hdc,300,160,str,lstrlen(str),1,TabPos,0);

			GetCharSetStr(logfont[NowFont].lfCharSet,str2);
			wsprintf(str, "CharSet\t: %s", str2);
			TabbedTextOut(hdc,300,180,str,lstrlen(str),1,TabPos,0);

			wsprintf(str, "OutPrecision\t: %d", logfont[NowFont].lfOutPrecision);
			TabbedTextOut(hdc,300,200,str,lstrlen(str),1,TabPos,0);

			wsprintf(str, "ClipPrecision\t: %d", logfont[NowFont].lfClipPrecision);
			TabbedTextOut(hdc,300,220,str,lstrlen(str),1,TabPos,0);

			wsprintf(str, "Quality\t: %d", logfont[NowFont].lfQuality);
			TabbedTextOut(hdc,300,240,str,lstrlen(str),1,TabPos,0);

			GetPitchFamStr(logfont[NowFont].lfPitchAndFamily,str2);
			wsprintf(str, "PitchAndFamily\t: %s", str2);
			TabbedTextOut(hdc,300,260,str,lstrlen(str),1,TabPos,0);

			wsprintf(str, "FaceName\t: %s", logfont[NowFont].lfFaceName);
			TabbedTextOut(hdc,300,280,str,lstrlen(str),1,TabPos,0);

			// 폰트의 타입을 출력한다. 이 값은 LOGFONT 구조체에는 없다.
			switch (arFontType[NowFont]) {
			case 1:
				wsprintf(str, "FontType\t: %s", "RASTER_FONTTYPE");
				break;
			case 2:
				wsprintf(str, "FontType\t: %s", "DEVICE_FONTTYPE");
				break;
			case 4:
				wsprintf(str, "FontType\t: %s", "TRUETYPE_FONTTYPE");
				break;
			default:
				wsprintf(str, "FontType\t: %s-%d", "UnKnown", arFontType[NowFont]);
			}
			TabbedTextOut(hdc,300,300,str,lstrlen(str),1,TabPos,0);

			// 폰트의 사용 예를 출력한다.
			lstrcpy(str, "Font Test 대한민국 1234!@#$");
			hFont=CreateFontIndirect(&logfont[NowFont]);
			OldFont=(HFONT)SelectObject(hdc, hFont);
			TextOut(hdc,300,340,str,lstrlen(str));
			SelectObject(hdc, GetStockObject(SYSTEM_FONT));
			DeleteObject(hFont);

			// 발견된 폰트 개수를 출력한다.
			wsprintf(str, "총 폰트 수\t: %d", num);
			TabbedTextOut(hdc,300,380,str,lstrlen(str),1,TabPos,0);
		} else {
			lstrcpy(str, "폰트를 선택하십시오");
			TextOut(hdc,300,200,str,lstrlen(str));
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}


