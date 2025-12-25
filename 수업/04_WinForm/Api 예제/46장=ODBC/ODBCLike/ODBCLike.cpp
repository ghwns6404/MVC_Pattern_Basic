#include <windows.h>
#include <sql.h>
#include <sqlext.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ODBCLike");

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
TCHAR Mes[1024]="마우스 왼쪽 버튼을 누르면 날짜를 조사하며\r\n"
	"오른쪽 버튼을 누르면 날짜를 변경합니다.";

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
		"DBQ=%s\\address.mdb;",Dir);
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
	HDC hdc;
	SQLCHAR Name[21];
	SQLINTEGER lName;
	TCHAR str[255]="서울에 사는 사람은 ";

	SQLBindCol(hStmt,1,SQL_C_CHAR,Name,sizeof(Name),&lName);
	
	// SQL문을 실행한다.
	if (SQLExecDirect(hStmt,(SQLCHAR *)"select name from tblAddress where "
		"addr like '서울%'",SQL_NTS) != SQL_SUCCESS) {
		return FALSE;
	}

	// 읽어온 데이터 출력
	hdc=GetDC(hWndMain);
	while(SQLFetch(hStmt)!=SQL_NO_DATA) {
		lstrcat(str,(TCHAR *)Name);
		lstrcat(str," ");
	};
	lstrcat(str,"입니다.");
	TextOut(hdc,10,10,str,lstrlen(str));
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