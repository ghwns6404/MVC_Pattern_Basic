#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("EnumPriv");

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

void EnumPrivileges(HDC hdc)
{
	int i;
	int y=10;
	TCHAR buf[255],Mes[255];
	DWORD size;
	DWORD LangID;
	
	TCHAR arPri[][50]={
		SE_CREATE_TOKEN_NAME,SE_ASSIGNPRIMARYTOKEN_NAME,SE_LOCK_MEMORY_NAME,
		SE_INCREASE_QUOTA_NAME,SE_UNSOLICITED_INPUT_NAME,SE_MACHINE_ACCOUNT_NAME,
		SE_TCB_NAME,SE_SECURITY_NAME,SE_TAKE_OWNERSHIP_NAME,SE_LOAD_DRIVER_NAME,
		SE_SYSTEM_PROFILE_NAME,SE_SYSTEMTIME_NAME,SE_PROF_SINGLE_PROCESS_NAME,
		SE_INC_BASE_PRIORITY_NAME,SE_CREATE_PAGEFILE_NAME,SE_CREATE_PERMANENT_NAME,
		SE_BACKUP_NAME,SE_RESTORE_NAME,SE_SHUTDOWN_NAME,SE_DEBUG_NAME,SE_AUDIT_NAME,
		SE_SYSTEM_ENVIRONMENT_NAME,SE_CHANGE_NOTIFY_NAME,SE_REMOTE_SHUTDOWN_NAME
	};
	
	for (i=0;i<sizeof(arPri)/sizeof(arPri[0]);i++) {
		size=255;
		LookupPrivilegeDisplayName(NULL,arPri[i],buf,&size,&LangID);
		wsprintf(Mes,"특권=%s,설명=%s",arPri[i],buf);
		TextOut(hdc,10,y,Mes,lstrlen(Mes));
		y+=20;
	}
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		EnumPrivileges(hdc);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
