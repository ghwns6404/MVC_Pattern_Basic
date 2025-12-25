#include <windows.h>
#include <sql.h>
#include <sqlext.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("Diagnostics");

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
	WndClass.lpszMenuName=NULL;
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
	return (int)Message.wParam;
}

// 핸들
SQLHENV hEnv;
SQLHDBC hDbc;
SQLHSTMT hStmt;

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
	if (SQLSetEnvAttr(hEnv,SQL_ATTR_ODBC_VERSION,(SQLPOINTER)SQL_OV_ODBC3,
		SQL_IS_INTEGER) != SQL_SUCCESS) 
		return FALSE;

	// 연결 핸들을 할당하고 연결한다.
	if (SQLAllocHandle(SQL_HANDLE_DBC,hEnv,&hDbc) != SQL_SUCCESS) 
		return FALSE;

	// MDB 파일에 연결하기
	GetCurrentDirectory(MAX_PATH,Dir);
	wsprintf((TCHAR *)InCon,"DRIVER={Microsoft Access Driver (*.mdb)};"
		"DBQ=%s\\cigarette.mdb;",Dir);
	Ret=SQLDriverConnect(hDbc,hWndMain,InCon,sizeof(InCon),OutCon,sizeof(OutCon),
		&cbOutCon, SQL_DRIVER_NOPROMPT);

	if ((Ret != SQL_SUCCESS) && (Ret != SQL_SUCCESS_WITH_INFO))
		return FALSE;

	// 명령 핸들을 할당한다.
	if (SQLAllocHandle(SQL_HANDLE_STMT,hDbc,&hStmt) != SQL_SUCCESS)
		return FALSE;

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
	int Price;
	BOOL Korean;
	SQLINTEGER lName,lPrice,lKorean;
	// 화면 출력을 위한 변수들
	int y=1;
	HDC hdc;
	int arTab[2]={200,400};
	TCHAR str[255];

	// 결과를 돌려받기 위해 바인딩한다.
	SQLBindCol(hStmt,1,SQL_C_CHAR,Name,sizeof(Name),&lName);
	SQLBindCol(hStmt,2,SQL_C_ULONG,&Price,0,&lPrice);
	SQLBindCol(hStmt,3,SQL_C_ULONG,&Korean,0,&lKorean);

	// SQL문을 실행한다.
	SQLRETURN ret;
	int iDiag;
	SQLINTEGER NativeError;
	SQLCHAR SqlState[6], Msg[255];
	SQLSMALLINT MsgLen;
	TCHAR Mes[1024];
	ret=SQLExecDirect(hStmt,(SQLCHAR *)"select name,price,korean from tblCigar2",SQL_NTS);
	if ((ret != SQL_SUCCESS) && (ret != SQL_SUCCESS_WITH_INFO)) {
		SQLGetDiagField(SQL_HANDLE_STMT, hStmt,0,SQL_DIAG_NUMBER,&iDiag,0,&MsgLen);
		wsprintf(Mes,"상태 레코드의 개수 = %d",iDiag);
		MessageBox(hWndMain,Mes,"진단 정보",MB_OK);

		for (iDiag=1;;iDiag++) {
			ret=SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, iDiag, SqlState, 
				&NativeError, Msg, sizeof(Msg), &MsgLen);
			if (ret==SQL_NO_DATA)
				break;
			wsprintf(Mes, "SQLSTATE:%s, NativeError:%d, 진단정보:%s",
				(LPCTSTR)SqlState,NativeError, (LPCTSTR)Msg);
			MessageBox(hWndMain,Mes,"진단 정보",MB_OK);
		}
		return FALSE;
	}

	// 읽어온 데이터 출력
	hdc=GetDC(hWndMain);
	while(SQLFetch(hStmt)!=SQL_NO_DATA) {
		wsprintf(str,"이름:%s\t가격:%d\t국산:%s",Name,Price,(Korean ? "예":"아니오"));
		TabbedTextOut(hdc,10,y*20,str,lstrlen(str),2,arTab,0);
		y++;
	};
	ReleaseDC(hWndMain,hdc);

	if (hStmt) SQLCloseCursor(hStmt);
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		if (DBConnect() == FALSE) {
			MessageBox(hWnd,"데이터 베이스에 연결할 수 없습니다","에러",MB_OK);
			return -1;
		}
		return 0;
	case WM_LBUTTONDOWN:
		DBExecuteSQL();
		return 0;
	case WM_DESTROY:
		DBDisConnect();
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
