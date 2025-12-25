#include <windows.h>
#include <sql.h>
#include <sqlext.h>
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
HWND hList,hFName,hFPrice,hFKorean;

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
		  ,LPSTR lpszCmdParam,int nCmdShow)
{
	g_hInst=hInstance;
	
	if (DBConnect() == FALSE) {
		MessageBox(NULL,"데이터 베이스에 연결할 수 없습니다","에러",MB_OK);
		return 0;
	}

	DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOGMAIN), HWND_DESKTOP, MainDlgProc);

	DBDisConnect();
	return 0;
}


BOOL DBConnect()
{
	// 연결 설정을 위한 변수들
	SQLCHAR InCon[255];
	SQLCHAR OutCon[1024];
    SQLSMALLINT cbOutCon;
	TCHAR Dir[MAX_PATH];
	SQLRETURN Ret;

	// 환경 핸들을 할당하고 버전 속성을 설정한다.
	if (SQLAllocHandle(SQL_HANDLE_ENV,SQL_NULL_HANDLE,&hEnv) != SQL_SUCCESS) 
		return FALSE;
	if (SQLSetEnvAttr(hEnv,SQL_ATTR_ODBC_VERSION,(SQLPOINTER)SQL_OV_ODBC3,SQL_IS_INTEGER) != SQL_SUCCESS) 
		return FALSE;

	// 연결 핸들을 할당하고 연결한다.
	if (SQLAllocHandle(SQL_HANDLE_DBC,hEnv,&hDbc) != SQL_SUCCESS) 
		return FALSE;

	// MDB 파일에 연결하기
	GetCurrentDirectory(MAX_PATH,Dir);
	wsprintf((TCHAR *)InCon,"DRIVER={Microsoft Access Driver (*.mdb)};DBQ=%s\\cigarette.mdb;",Dir);
	Ret=SQLDriverConnect(hDbc,hDlgMain,InCon,sizeof(InCon),OutCon,sizeof(OutCon),&cbOutCon, SQL_DRIVER_NOPROMPT);

	if ((Ret != SQL_SUCCESS) && (Ret != SQL_SUCCESS_WITH_INFO))
		return FALSE;

	if (SQLAllocHandle(SQL_HANDLE_STMT,hDbc,&hStmt) != SQL_SUCCESS)
		return 0;

	return TRUE;
}

void DBDisConnect()
{
	// 뒷정리
	if (hStmt) SQLFreeHandle(SQL_HANDLE_STMT,hStmt);
	if (hDbc) SQLDisconnect(hDbc);
	if (hDbc) SQLFreeHandle(SQL_HANDLE_DBC,hDbc);
	if (hEnv) SQLFreeHandle(SQL_HANDLE_ENV,hEnv);
}

BOOL DBExecuteSQL()
{
	// 결과값을 돌려받기 위한 변수들
	SQLCHAR Name[21];
	SQLINTEGER lName;

	// 결과를 돌려받기 위해 바인딩한다.
	SQLBindCol(hStmt,1,SQL_C_CHAR,Name,sizeof(Name),&lName);

	// SQL문을 실행한다.
	if (SQLExecDirect(hStmt,(SQLCHAR *)"select name from tblCigar",SQL_NTS) != SQL_SUCCESS) {
		return FALSE;
	}

	// 읽어온 데이터 출력
	SendMessage(hList,LB_RESETCONTENT,0,0);
	while (SQLFetch(hStmt)!=SQL_NO_DATA) {
		SendMessage(hList,LB_ADDSTRING,0,(LPARAM)Name);
	}

	if (hStmt) SQLCloseCursor(hStmt);
	return TRUE;
}

// 현재 선택된 레코드를 읽어 에디트에 출력한다.
BOOL ReadRecord()
{
	// 결과값을 돌려받기 위한 변수들
	SQLCHAR Name[21];
	int Price;
	BOOL Korean;
	SQLINTEGER lName,lPrice,lKorean;
	int idx;
	TCHAR Key[256], szSQL[256];

	idx=SendMessage(hList,LB_GETCURSEL,0,0);
	if (idx == -1)
		return FALSE;
	SendMessage(hList,LB_GETTEXT,idx,(LPARAM)Key);
	wsprintf(szSQL,"select name,price,korean from tblCigar where name='%s'",Key);

	// 결과를 돌려받기 위해 바인딩한다.
	SQLBindCol(hStmt,1,SQL_C_CHAR,Name,sizeof(Name),&lName);
	SQLBindCol(hStmt,2,SQL_C_ULONG,&Price,0,&lPrice);
	SQLBindCol(hStmt,3,SQL_C_ULONG,&Korean,0,&lKorean);

	// SQL문을 실행한다.
	if (SQLExecDirect(hStmt,(SQLCHAR *)szSQL,SQL_NTS) != SQL_SUCCESS) {
		return FALSE;
	}

	// 읽어온 데이터 출력
	if (SQLFetch(hStmt)!=SQL_NO_DATA) {
		SetWindowText(hFName,(LPCTSTR)Name);
		wsprintf(szSQL,"%d",Price);
		SetWindowText(hFPrice,szSQL);
		SendMessage(hFKorean,BM_SETCHECK,(Korean ? BST_CHECKED:BST_UNCHECKED),0);
	}
	
	if (hStmt) SQLCloseCursor(hStmt);
	return TRUE;
}

// 레코드를 추가한다.
void AddRecord()
{
	SQLCHAR Name[21];
	int Price;
	BOOL Korean;
	TCHAR szSQL[256];

	// 값 읽음
	GetWindowText(hFName,(LPTSTR)Name,20);
	if (lstrlen((LPCTSTR)Name) == 0) {
		MessageBox(hDlgMain,"추가할 레코드의 이름을 먼저 입력하십시요","알림",MB_OK);
		return;
	}
	GetWindowText(hFPrice,szSQL,256);
	Price=atoi(szSQL);
	Korean=(SendMessage(hFKorean,BM_GETCHECK,0,0)==BST_CHECKED);
	wsprintf(szSQL,"Insert into tblCigar (name,price,korean) VALUES ('%s',%d,%d)",
		Name,Price,Korean);

	if (SQLExecDirect(hStmt,(SQLCHAR *)szSQL,SQL_NTS) != SQL_SUCCESS) {
		MessageBox(hDlgMain,"레코드 추가 에러. 이름이 같은 레코드는 추가할 수 없습니다","에러",MB_OK);
		return;
	}

	if (hStmt) SQLCloseCursor(hStmt);
	DBExecuteSQL();
}

// 선택된 레코드를 삭제한다.
void DeleteRecord()
{
	TCHAR Key[256], szSQL[256];
	int idx;

	idx=SendMessage(hList,LB_GETCURSEL,0,0);
	if (idx == -1) {
		MessageBox(hDlgMain,"삭제할 레코드를 먼저 선택하십시요","알림",MB_OK);
		return;
	}
	SendMessage(hList,LB_GETTEXT,idx,(LPARAM)Key);
	wsprintf(szSQL,"Delete from tblCigar where name='%s'",Key);

	if (SQLExecDirect(hStmt,(SQLCHAR *)szSQL,SQL_NTS) != SQL_SUCCESS)
		return;

	if (hStmt) SQLCloseCursor(hStmt);
	DBExecuteSQL();
}

// 파라미터를 사용하여 업데이트하기
void UpdateRecord()
{
	SQLCHAR OldName[21], NewName[21];
	int Price;
	BOOL Korean;
	TCHAR szSQL[256];
	int idx;
	SQLINTEGER lenNewName=SQL_NTS,lenPrice=0,lenKorean=0,lenOldName=SQL_NTS;

	idx=SendMessage(hList,LB_GETCURSEL,0,0);
	if (idx == -1)
		return;
	SendMessage(hList,LB_GETTEXT,idx,(LPARAM)OldName);
	GetWindowText(hFName,(LPTSTR)NewName,20);
	GetWindowText(hFPrice,szSQL,256);
	Price=atoi(szSQL);
	Korean=(SendMessage(hFKorean,BM_GETCHECK,0,0)==BST_CHECKED);

	SQLBindParameter(hStmt,1,SQL_PARAM_INPUT,SQL_C_CHAR,SQL_CHAR,21,0,
		NewName,0,&lenNewName);
	SQLBindParameter(hStmt,2,SQL_PARAM_INPUT,SQL_C_ULONG,SQL_INTEGER,0,0,
		&Price,0,&lenPrice);
	SQLBindParameter(hStmt,3,SQL_PARAM_INPUT,SQL_C_ULONG,SQL_INTEGER,0,0,
		&Korean,0,&lenKorean);
	SQLBindParameter(hStmt,4,SQL_PARAM_INPUT,SQL_C_CHAR,SQL_CHAR,21,0,
		OldName,0,&lenOldName);

	if (SQLExecDirect(hStmt,(SQLCHAR *)"Update tblCigar set name=?, price=?, "
		"korean=? where name=?",SQL_NTS)!=SQL_SUCCESS)
		return;

	if (hStmt) SQLCloseCursor(hStmt);
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