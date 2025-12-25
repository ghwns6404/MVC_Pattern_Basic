#include <windows.h>
#include "CQuery.h"
#include "resource.h"

BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);
BOOL DBConnect();
void DBDisConnect();
BOOL DBExecuteSQL();
HINSTANCE g_hInst;
HWND hDlgMain;
HWND hList,hFName,hFPrice,hFKorean;
CQuery qry;
TCHAR szSQL[256],szTmp[256];

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
		  ,LPSTR lpszCmdParam,int nCmdShow)
{
	g_hInst=hInstance;

	if (qry.Connect(1,"Cigarette.mdb") == FALSE) {
		MessageBox(NULL,"데이터 베이스에 연결할 수 없습니다","에러",MB_OK);
		return 0;
	}

	DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOGMAIN), HWND_DESKTOP, MainDlgProc);

	return 0;
}

BOOL DBExecuteSQL()
{
	TCHAR Name[21];

	SendMessage(hList,LB_RESETCONTENT,0,0);
	qry.Exec("select name from tblCigar");
	while (qry.Fetch() != SQL_NO_DATA) {
		qry.GetStr("name",Name);
		SendMessage(hList,LB_ADDSTRING,0,(LPARAM)Name);
	}
	qry.Clear();
	return TRUE;
}

// 현재 선택된 레코드를 읽어 에디트에 출력한다.
BOOL ReadRecord()
{
	int idx;
	TCHAR Key[256];

	idx=SendMessage(hList,LB_GETCURSEL,0,0);
	if (idx == -1)
		return FALSE;
	SendMessage(hList,LB_GETTEXT,idx,(LPARAM)Key);
	wsprintf(szSQL,"select name,price,korean from tblCigar where name='%s'",Key);
	qry.Exec(szSQL);
	if (qry.Fetch() != SQL_NO_DATA) {
		qry.GetStr("name",szTmp);
		SetWindowText(hFName,szTmp);
		qry.GetStr("price",szTmp);
		SetWindowText(hFPrice,szTmp);
		SendMessage(hFKorean,BM_SETCHECK,(qry.GetInt("Korean") ? BST_CHECKED:BST_UNCHECKED),0);
	}
	qry.Clear();
	return TRUE;
}

// 레코드를 추가한다.
void AddRecord()
{
	TCHAR Name[21];
	int Price;
	BOOL Korean;

	// 값 읽음
	GetWindowText(hFName,Name,20);
	if (lstrlen(Name) == 0) {
		MessageBox(hDlgMain,"추가할 레코드의 이름을 먼저 입력하십시요","알림",MB_OK);
		return;
	}
	GetWindowText(hFPrice,szTmp,256);
	Price=atoi(szTmp);
	Korean=(SendMessage(hFKorean,BM_GETCHECK,0,0)==BST_CHECKED);
	wsprintf(szSQL,"Insert into tblCigar (name,price,korean) VALUES ('%s',%d,%d)",
		Name,Price,Korean);

	qry.Exec(szSQL);
	DBExecuteSQL();
}

// 선택된 레코드를 삭제한다.
void DeleteRecord()
{
	TCHAR Key[256];
	int idx;

	idx=SendMessage(hList,LB_GETCURSEL,0,0);
	if (idx == -1) {
		MessageBox(hDlgMain,"삭제할 레코드를 먼저 선택하십시요","알림",MB_OK);
		return;
	}
	SendMessage(hList,LB_GETTEXT,idx,(LPARAM)Key);
	wsprintf(szSQL,"Delete from tblCigar where name='%s'",Key);
	qry.Exec(szSQL);

	DBExecuteSQL();
}

// 업데이트 한다.
void UpdateRecord()
{
	TCHAR OldName[21], NewName[21];
	int Price;
	BOOL Korean;
	int idx;

	// 값 읽음
	idx=SendMessage(hList,LB_GETCURSEL,0,0);
	if (idx == -1)
		return;
	SendMessage(hList,LB_GETTEXT,idx,(LPARAM)OldName);

	GetWindowText(hFName,NewName,20);
	if (lstrlen(NewName) == 0) {
		MessageBox(hDlgMain,"변경할 레코드의 이름을 먼저 입력하십시요","알림",MB_OK);
		return;
	}
	GetWindowText(hFPrice,szTmp,256);
	Price=atoi(szTmp);
	Korean=(SendMessage(hFKorean,BM_GETCHECK,0,0)==BST_CHECKED);
	wsprintf(szSQL,"Update tblCigar set name='%s', price=%d, korean=%d where name='%s'",
		NewName,Price,Korean,OldName);

	qry.Exec(szSQL);

	DBExecuteSQL();
}

// hWnd 윈도우를 화면 중앙으로 보낸다.
void CenterWindow(HWND hWnd)
{
	RECT rcTemp1, rcTemp2 ;
	LONG iX, iY, iWidth, iHeight ;

	GetWindowRect(GetDesktopWindow(),&rcTemp1) ;
	GetWindowRect(hWnd,&rcTemp2) ;
	iWidth = rcTemp2.right - rcTemp2.left ;
	iHeight = rcTemp2.bottom - rcTemp2.top ;
	iX = LONG((rcTemp1.right - iWidth) / 2) ;
	iY = LONG((rcTemp1.bottom - iHeight) / 2) ;
	MoveWindow(hWnd,iX,iY,iWidth,iHeight,TRUE) ;
}

BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage) {
	case WM_INITDIALOG:
		hDlgMain = hDlg;
		hList=GetDlgItem(hDlgMain,IDC_LISTCIGAR);
		hFName=GetDlgItem(hDlgMain,IDC_FNAME);
		hFPrice=GetDlgItem(hDlgMain,IDC_FPRICE);
		hFKorean=GetDlgItem(hDlgMain,IDC_FKOREAN);
		CenterWindow(hDlg);
		DBExecuteSQL();
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
		case IDCANCEL:
			EndDialog(hDlgMain,IDOK);
			return TRUE;
		case IDC_BTNADD:
			AddRecord();
			return TRUE;
		case IDC_BTNDEL:
			DeleteRecord();
			return TRUE;
		case IDC_BTNUPDATE:
			UpdateRecord();
			return TRUE;
		case IDC_LISTCIGAR:
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				ReadRecord();
				return TRUE;
			}
		}
		return FALSE;
	}
	return FALSE;
}
