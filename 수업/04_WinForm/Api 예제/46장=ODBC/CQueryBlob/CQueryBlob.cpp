#include <windows.h>
#include "CQuery.h"
#include "resource.h"

// 핸들
SQLHENV hEnv;
SQLHDBC hDbc;
SQLHSTMT hStmt;

BOOL CALLBACK MainDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam);
BOOL DBConnect();
void DBDisConnect();
BOOL DBExecuteSQL();
HINSTANCE g_hInst;
HWND hDlgMain;
HWND hList,hMemo;
CQuery qry;
TCHAR szSQL[256], szTmp[256];

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
		  ,LPSTR lpszCmdParam,int nCmdShow)
{
	g_hInst=hInstance;
	
	if (qry.Connect(1,"Odbcblob.mdb") == FALSE) {
		MessageBox(NULL,"데이터 베이스에 연결할 수 없습니다","에러",MB_OK);
		return 0;
	}

	DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOGMAIN), HWND_DESKTOP, MainDlgProc);

	return 0;
}

BOOL DBExecuteSQL()
{
	SendMessage(hList,LB_RESETCONTENT,0,0);
	qry.Exec("select id from tblBlob");
	while(qry.Fetch()!=SQL_NO_DATA) {
		qry.GetStr("id",szTmp);
		SendMessage(hList,LB_ADDSTRING,0,(LPARAM)szTmp);
	}
	qry.Clear();
	
	return TRUE;
}

// 현재 선택된 레코드를 삭제한다.
void DeleteRecord()
{
	int idx;

	idx=SendMessage(hList,LB_GETCURSEL,0,0);
	if (idx==-1)
		return;
	if (MessageBox(hDlgMain,"이 레코드를 삭제하시겠습니까?","질문",MB_YESNO)==IDNO)
		return;
	SendMessage(hList,LB_GETTEXT,idx,(LPARAM)szTmp);
	SendMessage(hList,LB_DELETESTRING,idx,0);

	wsprintf(szSQL,"Delete from tblBlob where id=%s",szTmp);
	qry.Exec(szSQL);
}

// 빈 레코드를 추가한다.
void AddRecord()
{
	int Max=0;

	// Max(id) 구하기
	Max=max(qry.ExecGetInt("select Max(id) from tblBlob"),0);
	wsprintf(szSQL, "insert into tblBlob (id) values (%d)",Max+1);
	qry.Exec(szSQL);
	DBExecuteSQL();
}

// id번의 레코드를 읽는다.
void ReadRecord(int id)
{
	TCHAR buf[66000]={0,};

	if (id==-1)
		return;
	wsprintf(szSQL, "select memo from tblblob where id=%d",id);
	qry.ReadBlob(szSQL,buf);
	SetWindowText(hMemo,buf);
}

// 현재 레코드를 저장한다.
void UpdateRecord()
{
	TCHAR buf[66000];
	int idx,id;

	idx=SendMessage(hList,LB_GETCURSEL,0,0);
	SendMessage(hList,LB_GETTEXT,idx,(LPARAM)szTmp);
	id=atoi(szTmp);
	GetWindowText(hMemo,buf,66000);

	wsprintf(szSQL,"Update tblBlob set memo=? where id=%d",id);
	qry.WriteBlob(szSQL,buf,lstrlen(buf));
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
	TCHAR tmp[10];

	switch (iMessage) {
	case WM_INITDIALOG:
		hDlgMain = hDlg;
		CenterWindow(hDlg);
		hList=GetDlgItem(hDlgMain,IDC_LIST1);
		hMemo=GetDlgItem(hDlgMain,IDC_MEMO);
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
		case IDC_LIST1:
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				SendMessage(hList,LB_GETTEXT,SendMessage(hList,LB_GETCURSEL,0,0),(LPARAM)tmp);
				ReadRecord(atoi(tmp));
				return TRUE;
			}
		}
		return FALSE;
	}
	return FALSE;
}
