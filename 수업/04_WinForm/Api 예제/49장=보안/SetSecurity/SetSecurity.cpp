#include <windows.h>
#include <aclapi.h>

void SetSecurity();
HINSTANCE g_hInst;

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
		  ,LPSTR lpszCmdParam,int nCmdShow)
{
	g_hInst=hInstance;

	SetSecurity();

	MessageBox(NULL, "보안 설명자를 생성했습니다","알림",MB_OK);
	return 0;
	
}

void SetSecurity()
{
	HANDLE hFile;
	TCHAR *str="누구나 읽을 수 있으며 User1만 쓸 수 있습니다";
	DWORD dwWritten;
	EXPLICIT_ACCESS EA[2];
	TCHAR pSid[255];
	TCHAR pDomain[255];
	DWORD cbSid=255,cbDomain=255;
	SID_NAME_USE peUse;
	PACL pAcl;
	SECURITY_DESCRIPTOR SD;
	SECURITY_ATTRIBUTES SA;

	// Everyone의 SID를 구한다.
	SID_IDENTIFIER_AUTHORITY SIDEvery=SECURITY_WORLD_SID_AUTHORITY;
	PSID pEverySID;
	AllocateAndInitializeSid(&SIDEvery,1,SECURITY_WORLD_RID,0,0,0,0,0,0,0,&pEverySID);

	memset(EA,0,sizeof(EXPLICIT_ACCESS)*2);
	EA[0].grfAccessPermissions=GENERIC_READ;
	EA[0].grfAccessMode=SET_ACCESS;
	EA[0].grfInheritance=NO_INHERITANCE;
	EA[0].Trustee.TrusteeForm=TRUSTEE_IS_SID;
	EA[0].Trustee.TrusteeType=TRUSTEE_IS_WELL_KNOWN_GROUP;
	EA[0].Trustee.ptstrName=(LPTSTR)pEverySID;

	// User1의 SID를 구한다.
	LookupAccountName(NULL,"User1",(PSID)pSid,&cbSid,pDomain, &cbDomain, &peUse);

	// ACE를 만든다.
	EA[1].grfAccessPermissions=GENERIC_ALL;
	EA[1].grfAccessMode=SET_ACCESS;
	EA[1].grfInheritance=NO_INHERITANCE;
	EA[1].Trustee.TrusteeForm=TRUSTEE_IS_SID;
	EA[1].Trustee.TrusteeType=TRUSTEE_IS_USER;
	EA[1].Trustee.ptstrName=(LPTSTR)pSid;

	// ACE를 ACL에 포함시키고 새 ACL을 만든다.
	SetEntriesInAcl(2,EA,NULL,&pAcl);

	// SD를 초기화한다.
	InitializeSecurityDescriptor(&SD, SECURITY_DESCRIPTOR_REVISION);

	// ACL을 SD에 포함시킨다.
	SetSecurityDescriptorDacl(&SD,TRUE,pAcl,FALSE);

	// 보안 속성을 만든다.
	SA.nLength=sizeof(SECURITY_ATTRIBUTES);
	SA.lpSecurityDescriptor=&SD;
	SA.bInheritHandle=FALSE;

	// 파일을 생성한다.
	hFile=CreateFile("c:\\User1Only.txt", GENERIC_ALL, 0, &SA,
		CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	WriteFile(hFile,str,lstrlen(str),&dwWritten,NULL);
	CloseHandle(hFile);

	// 실행중에 할당한 메모리를 해제한다.
	FreeSid(pEverySID);
	LocalFree(pAcl);
}