#include <windows.h>
#include "resource.h"

BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);
HINSTANCE g_hInst;
HWND hDlgMain;

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	,LPSTR lpszCmdParam,int nCmdShow)
{
	g_hInst=hInstance;
	DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), HWND_DESKTOP, MainDlgProc);
	return 0;
}

#include <commctrl.h>
HWND hTip;
HFONT hFont=NULL;
BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	INITCOMMONCONTROLSEX iccex;
	RECT rt={300,50,500,150};
	TOOLINFO ti;
	CHOOSECOLOR COL;
	COLORREF crTemp[16];
	RECT mrt;
	CHOOSEFONT CFT;
	LOGFONT lf;

	switch (iMessage)
	{
	case WM_INITDIALOG:
		hDlgMain = hDlg;

		iccex.dwICC=ICC_WIN95_CLASSES;
		iccex.dwSize=sizeof(INITCOMMONCONTROLSEX);
		InitCommonControlsEx(&iccex);

		hTip=CreateWindowEx(WS_EX_TOPMOST,TOOLTIPS_CLASS,NULL,0,
			CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
			hDlg,NULL,g_hInst,NULL);

		ti.cbSize=sizeof(TOOLINFO);
		ti.uFlags=TTF_SUBCLASS | TTF_IDISHWND;
		ti.uId=(WPARAM)GetDlgItem(hDlg,IDC_BUTTON1);
		ti.lpszText="버튼 컨트롤입니다. 이 예제를 사용하여 툴팁의 다양한 세팅 상태를 변경해 보십시오.";
		SendMessage(hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti);

		ti.uId=(WPARAM)GetDlgItem(hDlg,IDC_EDIT1);
		ti.lpszText="에디트입니다. 아래쪽의 컨트롤로 세팅을 조사하거나 변경합니다.";
		SendMessage(hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti);

		ti.uId=(WPARAM)GetDlgItem(hDlg,IDC_LIST1);
		ti.lpszText="리스트 박스입니다.\r\n이 줄은 두 줄로 출력됩니다";
		SendMessage(hTip,TTM_ADDTOOL,0,(LPARAM)(LPTOOLINFO)&ti);

		// 지연 시간 조사
		SetDlgItemInt(hDlg,IDC_EDINITIAL,SendMessage(hTip,
			TTM_GETDELAYTIME,(WPARAM)TTDT_INITIAL,0),FALSE);
		SetDlgItemInt(hDlg,IDC_EDPOPUP,SendMessage(hTip,
			TTM_GETDELAYTIME,(WPARAM)TTDT_AUTOPOP,0),FALSE);
		SetDlgItemInt(hDlg,IDC_EDRESHOW,SendMessage(hTip,
			TTM_GETDELAYTIME,(WPARAM)TTDT_RESHOW,0),FALSE);

		// 마진 조사
		SendMessage(hTip,TTM_GETMARGIN,0,(LPARAM)&mrt);
		SetDlgItemInt(hDlg,IDC_EDLEFT,mrt.left,TRUE);
		SetDlgItemInt(hDlg,IDC_EDTOP,mrt.top,TRUE);
		SetDlgItemInt(hDlg,IDC_EDRIGHT,mrt.right,TRUE);
		SetDlgItemInt(hDlg,IDC_EDBOTTOM,mrt.bottom,TRUE);

		// 최대 폭 조사
		SetDlgItemInt(hDlg,IDC_EDWIDTH,SendMessage(hTip,TTM_GETMAXTIPWIDTH,0,0),TRUE);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
			EndDialog(hDlg,0);
			return TRUE;
		case IDC_BTNRESET:
			SendMessage(hTip,TTM_SETDELAYTIME,(WPARAM)TTDT_AUTOMATIC,MAKELONG(-1,0));
		case IDC_BTNSETDELAY:
			SendMessage(hTip,TTM_SETDELAYTIME,(WPARAM)TTDT_INITIAL,MAKELONG(
				GetDlgItemInt(hDlg,IDC_EDINITIAL,NULL,FALSE),0));
			SendMessage(hTip,TTM_SETDELAYTIME,(WPARAM)TTDT_AUTOPOP,MAKELONG(
				GetDlgItemInt(hDlg,IDC_EDPOPUP,NULL,FALSE),0));
			SendMessage(hTip,TTM_SETDELAYTIME,(WPARAM)TTDT_RESHOW,MAKELONG(
				GetDlgItemInt(hDlg,IDC_EDRESHOW,NULL,FALSE),0));
			return TRUE;
		case IDC_BTNBKCOLOR:
		case IDC_BTNTEXTCOLOR:
			memset(&COL, 0, sizeof(CHOOSECOLOR));
			COL.lStructSize = sizeof(CHOOSECOLOR);
			COL.hwndOwner=hDlg;
			COL.lpCustColors=crTemp;
			COL.Flags=CC_RGBINIT | CC_FULLOPEN;
			if (LOWORD(wParam)==IDC_BTNBKCOLOR) {
				COL.rgbResult=SendMessage(hTip,TTM_GETTIPBKCOLOR,0,0);
				if (ChooseColor(&COL)!=0) {
					SendMessage(hTip,TTM_SETTIPBKCOLOR,(WPARAM)COL.rgbResult,0);
				}
			} else {
				COL.rgbResult=SendMessage(hTip,TTM_GETTIPTEXTCOLOR,0,0);
				if (ChooseColor(&COL)!=0) {
					SendMessage(hTip,TTM_SETTIPTEXTCOLOR,(WPARAM)COL.rgbResult,0);
				}
			}
			return TRUE;
		case IDC_BTNSETMARGIN:
			mrt.left=GetDlgItemInt(hDlg,IDC_EDLEFT,NULL,TRUE);
			mrt.top=GetDlgItemInt(hDlg,IDC_EDTOP,NULL,TRUE);
			mrt.right=GetDlgItemInt(hDlg,IDC_EDRIGHT,NULL,TRUE);
			mrt.bottom=GetDlgItemInt(hDlg,IDC_EDBOTTOM,NULL,TRUE);
			SendMessage(hTip,TTM_SETMARGIN,0,(LPARAM)&mrt);
			return TRUE;
		case IDC_BTNSETWIDTH:
			SendMessage(hTip,TTM_SETMAXTIPWIDTH,0,
				(LPARAM)GetDlgItemInt(hDlg,IDC_EDWIDTH,NULL,TRUE));
			return TRUE;
		case IDC_BTNFONT:
			memset(&CFT, 0, sizeof(CHOOSEFONT));
			CFT.lStructSize = sizeof(CHOOSEFONT);
			CFT.hwndOwner=hDlg;
			CFT.lpLogFont=&lf;
			CFT.Flags=CF_EFFECTS | CF_SCREENFONTS;
			if (ChooseFont(&CFT)) {
				hFont=(HFONT)SendMessage(hTip,WM_GETFONT,0,0);
				if (hFont)
					DeleteObject(hFont);
				hFont=CreateFontIndirect(&lf);
				SendMessage(hTip,WM_SETFONT,(WPARAM)hFont,(LPARAM)TRUE);
			}
			return TRUE;
		}
		return FALSE;
	case WM_DESTROY:
		if (hFont)
			DeleteObject(hFont);
		return TRUE;
	}
	return FALSE;
}
