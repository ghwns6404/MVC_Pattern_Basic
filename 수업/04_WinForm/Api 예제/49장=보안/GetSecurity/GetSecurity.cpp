#include <windows.h>
#include <aclapi.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("GetSecurity");

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

TCHAR gMes[1024]="마우스 왼쪽 버튼을 눌러 보안 정보를 보고자"
	" 하는 파일을 선택하십시오";
void GetSecurity()
{
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH]="";
	HANDLE hFile;
	TCHAR Mes[255];
	PSECURITY_DESCRIPTOR pSD;
	PSID pOwner;
	PACL pDacl;
	int i;
	DWORD cbName, cbDomain;
	TCHAR *Name, *Domain;
	SID_NAME_USE peUse;
	ULONG nAce;
	PEXPLICIT_ACCESS pEntry;

	// 대상 파일의 이름을 얻는다.
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner=hWndMain;
	OFN.lpstrFilter="Every File(*.*)\0*.*\0";
	OFN.lpstrFile=lpstrFile;
	OFN.nMaxFile=256;
	OFN.lpstrInitialDir="c:\\";
	if (GetOpenFileName(&OFN)==0) {
		return;
	}

	// 파일을 연다.
	hFile=CreateFile(lpstrFile, GENERIC_READ, 0, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return;

	// 파일명을 출력한다.
	wsprintf(Mes, "파일명=%s\n", lpstrFile);
	lstrcpy(gMes,Mes);

	// 보안 설명자를 읽는다.
	if (GetSecurityInfo(hFile,SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION |
		DACL_SECURITY_INFORMATION,&pOwner,NULL,&pDacl,NULL,
		(LPVOID *)&pSD)!=ERROR_SUCCESS) {
		MessageBox(hWndMain, "보안 설명자를 읽을 수 없습니다","에러",MB_OK);
		return;
	}
	CloseHandle(hFile);

	// 소유자 정보 보여주기
	cbName=0;
	cbDomain=0;
	LookupAccountSid(NULL, pOwner, NULL, &cbName, NULL, &cbDomain, &peUse);
	Name=(TCHAR *)malloc(cbName);
	Domain=(TCHAR *)malloc(cbDomain);
	LookupAccountSid(NULL, pOwner, Name, &cbName, Domain, &cbDomain, &peUse);

	wsprintf(Mes, "소유자=%s\n", Name);
	lstrcat(gMes,Mes);

	wsprintf(Mes, "소유자 도메인=%s\n", Domain);
	lstrcat(gMes,Mes);

	free(Name);
	free(Domain);

	// DACL의 정보 보여주기
	GetExplicitEntriesFromAcl(pDacl, &nAce, &pEntry);
	wsprintf(Mes, "ACE의 개수=%d\n", nAce);
	lstrcat(gMes,Mes);

	for (i=0;i<(int)nAce;i++) {
		cbName=0;
		cbDomain=0;
		LookupAccountSid(NULL, pEntry[i].Trustee.ptstrName, NULL, &cbName, 
			NULL, &cbDomain, &peUse);
		Name=(TCHAR *)malloc(cbName);
		Domain=(TCHAR *)malloc(cbDomain);
		LookupAccountSid(NULL, pEntry[i].Trustee.ptstrName, Name, &cbName, 
			Domain, &cbDomain, &peUse);

		wsprintf(Mes, "%s사용자에게 %x 액세스 권한을 %s한다.\n", Name, 
			pEntry[i].grfAccessPermissions, 
			pEntry[i].grfAccessMode == GRANT_ACCESS ? "허가":"거부");
		lstrcat(gMes,Mes);
		free(Name);
		free(Domain);
	}

	// 메모리 해제하고 결과를 화면으로 출력한다.
	LocalFree(pEntry);
	LocalFree(pSD);
	InvalidateRect(hWndMain, NULL, TRUE);
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT crt;

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		return 0;
	case WM_LBUTTONDOWN:
		GetSecurity();
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		GetClientRect(hWnd,&crt);
		DrawText(hdc,gMes,-1,&crt,0);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
