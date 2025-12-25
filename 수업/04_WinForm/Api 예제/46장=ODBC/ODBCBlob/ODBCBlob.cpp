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
HWND hList,hMemo;

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
	wsprintf((TCHAR *)InCon,"DRIVER={Microsoft Access Driver (*.mdb)};DBQ=%s\\odbcblob.mdb;",Dir);
	Ret=SQLDriverConnect(hDbc,hDlgMain,InCon,sizeof(InCon),OutCon,sizeof(OutCon),&cbOutCon, SQL_DRIVER_NOPROMPT);
	
	// SQL 서버에 연결하기.
	//Ret=SQLConnect(hDbc,(SQLCHAR *)"SQLBlob",SQL_NTS,(SQLCHAR *)"sa",SQL_NTS,(SQLCHAR *)"",SQL_NTS);
	if ((Ret != SQL_SUCCESS) && (Ret != SQL_SUCCESS_WITH_INFO))
		return FALSE;

	if (SQLAllocHandle(SQL_HANDLE_STMT,hDbc,&hStmt) != SQL_SUCCESS)
		return 0;

	return TRUE;
}

void DBDisConnect()
{
	// 뒷정리
	SQLFreeHandle(SQL_HANDLE_STMT,hStmt);
	if (hDbc) SQLDisconnect(hDbc);
	if (hDbc) SQLFreeHandle(SQL_HANDLE_DBC,hDbc);
	if (hEnv) SQLFreeHandle(SQL_HANDLE_ENV,hEnv);
}

BOOL DBExecuteSQL()
{
	// 결과값을 돌려받기 위한 변수들
	int id;
	SQLINTEGER lid;
	TCHAR temp[10];

	// 결과를 돌려받기 위해 바인딩한다.
	SQLBindCol(hStmt,1,SQL_C_ULONG,&id,0,&lid);

	// SQL문을 실행한다.
	if (SQLExecDirect(hStmt,(SQLCHAR *)"select id from tblBlob",SQL_NTS) != SQL_SUCCESS) {
		return FALSE;
	}

	// 읽어온 데이터 출력
	SendMessage(hList,LB_RESETCONTENT,0,0);
	while(SQLFetch(hStmt)!=SQL_NO_DATA) {
		wsprintf(temp,"%d",id);
		SendMessage(hList,LB_ADDSTRING,0,(LPARAM)temp);
	}

	if (hStmt) SQLCloseCursor(hStmt);
	SQLFreeStmt(hStmt, SQL_UNBIND);
	return TRUE;

}

// 현재 선택된 레코드를 삭제한다.
void DeleteRecord()
{
	int idx;
	TCHAR tmp[10];
	SQLCHAR SQL[100];

	idx=SendMessage(hList,LB_GETCURSEL,0,0);
	if (idx==-1)
		return;
	if (MessageBox(hDlgMain,"이 레코드를 삭제하시겠습니까?","질문",MB_YESNO)==IDNO)
		return;
	SendMessage(hList,LB_GETTEXT,idx,(LPARAM)tmp);
	SendMessage(hList,LB_DELETESTRING,idx,0);

	wsprintf((LPTSTR)SQL,"Delete from tblBlob where id=%s",tmp);
	SQLExecDirect(hStmt,SQL,SQL_NTS);
}

// 빈 레코드를 추가한다.
void AddRecord()
{
	SQLCHAR szSQL[255];
	int Max=0;
	SQLINTEGER MaxLen;

	// Max(id) 구하기
	SQLBindCol(hStmt,1,SQL_C_ULONG,&Max,0,&MaxLen);
	SQLExecDirect(hStmt,(SQLCHAR *)"select Max(id) as iMax from tblBlob",SQL_NTS);
	SQLFetch(hStmt);
	SQLCloseCursor(hStmt);

	wsprintf((LPTSTR)szSQL, "insert into tblBlob (id) values (%d)",Max+1);
	if (SQLExecDirect(hStmt,szSQL,SQL_NTS) != SQL_SUCCESS)
		return;
	DBExecuteSQL();
}

// id번의 레코드를 읽는다.
void ReadRecord(int id)
{
	TCHAR buf[66000]={0,};
	SQLCHAR BinaryPtr[100];
	SQLINTEGER LenBin;
	SQLCHAR SQL[100];
	SQLRETURN ret;
	TCHAR *p;
	int nGet;

	if (id==-1)
		return;
	wsprintf((LPTSTR)SQL, "select memo from tblblob where id=%d",id);
	ret=SQLExecDirect(hStmt,SQL,SQL_NTS);
	while ((ret=SQLFetch(hStmt)) != SQL_NO_DATA) {
		p=buf;
		while ((ret=SQLGetData(hStmt,1,SQL_C_BINARY,BinaryPtr,
			sizeof(BinaryPtr),&LenBin))!=SQL_NO_DATA) {
			if (LenBin==SQL_NULL_DATA)
				break;
			if (ret==SQL_SUCCESS)
				nGet=LenBin;
			else
				nGet=100;
			memcpy(p,BinaryPtr,nGet);
			p+=nGet;
		}
	}
	SetWindowText(hMemo,(LPCTSTR)buf);
	if (hStmt) SQLCloseCursor(hStmt);
}

// 현재 레코드를 저장한다.
void UpdateRecord()
{
	SQLRETURN ret;
	SQLINTEGER cbBlob;
	TCHAR buf[66000],tmp[100],*p;
	int idx,id;
	SQLPOINTER pToken;
	SQLCHAR SQL[100];
	int size,nPut;

	idx=SendMessage(hList,LB_GETCURSEL,0,0);
	SendMessage(hList,LB_GETTEXT,idx,(LPARAM)tmp);
	id=atoi(tmp);
	GetWindowText(hMemo,buf,66000);
	size=lstrlen(buf);

	cbBlob=SQL_LEN_DATA_AT_EXEC(size);
	SQLBindParameter(hStmt,1,SQL_PARAM_INPUT,SQL_C_BINARY,SQL_LONGVARBINARY,
		size,0,(SQLPOINTER)1,0,&cbBlob);
	wsprintf((LPTSTR)SQL,"Update tblBlob set memo=? where id=%d",id);
	ret=SQLExecDirect(hStmt,SQL,SQL_NTS);
	if (ret==SQL_NEED_DATA) {
		ret=SQLParamData(hStmt, &pToken);
		while (ret==SQL_NEED_DATA) {
			if ((int)pToken==1) {
				for (p=buf;p<buf+size;p+=100) {
					nPut=min(100,buf+size-p);
					memcpy(tmp,p,nPut);
					SQLPutData(hStmt,(PTR)tmp,nPut);
				}
			}
			ret=SQLParamData(hStmt, &pToken);
		}
	}
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
				SendMessage(hList,LB_GETTEXT,SendMessage(hList,LB_GETCURSEL,0,0),
					(LPARAM)tmp);
				ReadRecord(atoi(tmp));
				return TRUE;
			}
		}
		return FALSE;
	}
	return FALSE;
}