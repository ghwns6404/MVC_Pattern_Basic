#include <windows.h>
#include <sql.h>
#include <sqlext.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ODBCDate");

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
	TCHAR szTmp[256];
    SQLCHAR Date[30];
    SQL_DATE_STRUCT stDate;
    SQLINTEGER lDate;

    // 문자열로 날짜 읽기
    SQLBindCol(hStmt,1,SQL_C_CHAR,Date,sizeof(Date),&lDate);
    SQLExecDirect(hStmt,(SQLCHAR *)"select birth from tblAddress where "
		"name='박미영'",SQL_NTS);
    SQLFetch(hStmt);
	wsprintf(Mes,"박미영의 생일은 %s입니다\r\n",Date);
    SQLCloseCursor(hStmt);

    // 구조체로 날짜 읽기
    SQLBindCol(hStmt,1,SQL_C_TYPE_DATE,&stDate,sizeof(stDate),&lDate);
    SQLExecDirect(hStmt,(SQLCHAR *)"select birth from tblAddress where "
		"name='오광섭'",SQL_NTS);
    SQLFetch(hStmt);
    wsprintf(szTmp,"오광섭의 생일은 %d-%d-%d입니다.\r\n",
		stDate.year,stDate.month,stDate.day);
	lstrcat(Mes,szTmp);
    SQLCloseCursor(hStmt);

	InvalidateRect(hWndMain,NULL,TRUE);

	return TRUE;
}

BOOL DBExecuteSQL2()
{
    SQL_DATE_STRUCT stDate;
    SQLINTEGER lDate=0;

    // 문자열로 날짜 쓰기
    SQLExecDirect(hStmt, (SQLCHAR *)"update tblAddress set birth="
		"{d '1988-12-30'} where name='이승우'",SQL_NTS);

    // 구조체로 날자 쓰기
    SQLBindParameter(hStmt,1,SQL_PARAM_INPUT,SQL_C_TYPE_DATE,SQL_TYPE_DATE,
		0,0,&stDate,0,&lDate);
    stDate.year=2000;
    stDate.month=10;
    stDate.day=1;
    SQLExecDirect(hStmt,(SQLCHAR *)"update tblAddress set birth=? where "
		"name='박미영'",SQL_NTS);
    SQLFreeStmt(hStmt,SQL_UNBIND);

    // 오늘 날짜로 바꾸기
    SQLExecDirect(hStmt, (SQLCHAR *)"update tblAddress set birth="
		"{fn CURDATE()} where name='오광섭'",SQL_NTS);

	lstrcpy(Mes,"날짜를 변경했습니다");
	InvalidateRect(hWndMain,NULL,TRUE);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rt;

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		if (DBConnect() == FALSE) {
			MessageBox(hWnd,"데이터 베이스에 연결할 수 없습니다","에러",MB_OK);
			return -1;
		}
		return 0;
		return 0;
	case WM_LBUTTONDOWN:
		DBExecuteSQL();
		return 0;
	case WM_RBUTTONDOWN:
		DBExecuteSQL2();
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		SetRect(&rt,10,10,500,300);
		DrawText(hdc,Mes,-1,&rt,0);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		DBDisConnect();
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
