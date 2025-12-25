#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPSTR lpszClass=TEXT("GetSecurity2");
TCHAR gMes[1024]="마우스 왼쪽 버튼을 파일을 선택하십시오";

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
	hWndMain=hWnd;
	
	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

void GetSecurity2()
{
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH]="";
	HANDLE hFile;
	TCHAR Mes[255];

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

	lstrcpy(lpstrFile, "c:\\admin.txt");

	// 파일을 연다.
	hFile=CreateFile(lpstrFile, GENERIC_READ, 0, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return;

	// 파일명을 출력한다.
	wsprintf(Mes, "파일명=%s\n", lpstrFile);
	lstrcpy(gMes,Mes);

	// 보안 설명자를 읽는다.
	SECURITY_DESCRIPTOR *pSD;
	DWORD dwNeed;
	GetKernelObjectSecurity(hFile, OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION |
		DACL_SECURITY_INFORMATION, NULL, 0, &dwNeed);
	pSD=(SECURITY_DESCRIPTOR *)malloc(dwNeed);
	wsprintf(Mes, "보안 설명자의 크기=%d\n", dwNeed);
	lstrcat(gMes,Mes);
	GetKernelObjectSecurity(hFile, OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION |
		DACL_SECURITY_INFORMATION, pSD, dwNeed, &dwNeed);
	CloseHandle(hFile);

	// 소유자 정보 보여주기
	PSID pOwner;
	BOOL bDefault;
	GetSecurityDescriptorOwner(pSD, &pOwner, &bDefault);

	DWORD cbName, cbDomain;
	SID_NAME_USE peUse;
	TCHAR *Name, *Domain;
	cbName=0;
	cbDomain=0;
	LookupAccountSid(NULL, pOwner, NULL, &cbName, NULL, &cbDomain, &peUse);
	Name=(TCHAR *)malloc(cbName);
	Domain=(TCHAR *)malloc(cbDomain);
	LookupAccountSid(NULL, pOwner, Name, &cbName, Domain, &cbDomain, &peUse);

	wsprintf(Mes, "소유자=%s\n", Name);
	lstrcat(gMes,Mes);

	wsprintf(Mes, "사용자 도메인=%s\n", Domain);
	lstrcat(gMes,Mes);

	free(Name);
	free(Domain);

	// 그룹 정보 보여주기
	PSID pGroup;
	GetSecurityDescriptorGroup(pSD, &pGroup, &bDefault);

	cbName=0;
	cbDomain=0;
	LookupAccountSid(NULL, pGroup, NULL, &cbName, NULL, &cbDomain, &peUse);
	Name=(char *)malloc(cbName);
	Domain=(char *)malloc(cbDomain);
	LookupAccountSid(NULL, pGroup, Name, &cbName, Domain, &cbDomain, &peUse);

	wsprintf(Mes, "그룹=%s\n", Name);
	lstrcat(gMes,Mes);

	wsprintf(Mes, "그룹 도메인=%s\n", Domain);
	lstrcat(gMes,Mes);

	free(Name);
	free(Domain);

	// DACL에서 정보를 추출한다.
	BOOL bPresent;
	PACL pDacl;
	ACL_SIZE_INFORMATION aclInfo;
	ACCESS_ALLOWED_ACE *pAce;
	int i;
	TCHAR AceType[32];

	GetSecurityDescriptorDacl(pSD, &bPresent, &pDacl, &bDefault);

	// DACL이 없는 경우
	if (bPresent == FALSE) {
		wsprintf(Mes, "DACL이 없습니다.\n");
		lstrcat(gMes,Mes);
	} else {
		if (bDefault == TRUE) {
			wsprintf(Mes, "디폴트 DACL을 가지고 있습니다.\n");
			lstrcat(gMes,Mes);
		} else {
			wsprintf(Mes, "디폴트가 아닌 DACL을 가지고 있습니다.\n");
			lstrcat(gMes,Mes);
		}

		// ACE의 개수를 구하고 순회한다.
		GetAclInformation(pDacl,&aclInfo, sizeof(ACL_SIZE_INFORMATION), AclSizeInformation);
		for (i=0;i<(int)aclInfo.AceCount;i++) {
			GetAce(pDacl,i,(LPVOID *)&pAce);

			cbName=0;
			cbDomain=0;
			LookupAccountSid(NULL, &pAce->SidStart, NULL, &cbName, NULL, &cbDomain, &peUse);
			Name=(char *)malloc(++cbName);
			Domain=(char *)malloc(++cbDomain);
			LookupAccountSid(NULL, &pAce->SidStart, Name, &cbName, Domain, &cbDomain, &peUse);

			switch (pAce->Header.AceType) {
				case ACCESS_ALLOWED_ACE_TYPE:
					lstrcpy(AceType,"허가");
					break;
				case ACCESS_DENIED_ACE_TYPE:
					lstrcpy(AceType,"금지");
					break;
				case SYSTEM_AUDIT_ACE_TYPE:
					lstrcpy(AceType,"감사");
					break;
				default:
					lstrcpy(AceType,"기타");
					break;
			}

			wsprintf(Mes, "ACE(%d) 타입=%s, 마스크=%x, 소유자=%s, 도메인 = %s\n",i,AceType,pAce->Mask,Name,Domain);
			lstrcat(gMes,Mes);

			free(Name);
			free(Domain);
		}
	}

	// 메모리 해제하고 결과를 화면으로 출력한다.
	free(pSD);
	InvalidateRect(hWndMain, NULL, TRUE);
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT crt;

	switch (iMessage) {
	case WM_LBUTTONDOWN:
		GetSecurity2();
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
