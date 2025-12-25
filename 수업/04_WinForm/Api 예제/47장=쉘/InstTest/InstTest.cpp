#include <windows.h>
#include <shlobj.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("InstTest");

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

// 파일에 대한 쇼트컷을 만든다.
// szSrcFile:쇼트컷 대상 파일의 풀 패스
// szLnkFile:쇼트컷 파일의 풀 패스
// szArgument:쇼트컷에 전달될 인수 문자열
// szDesc:쇼트컷 설명 문자열
HRESULT MyCreateShortCut(LPCSTR szSrcFile, LPSTR szLnkFile, LPSTR szArgument, LPSTR szDesc)
{
	HRESULT hr;
	IShellLink *pSl;
	IPersistFile *pPf;
	WCHAR wszLnkFile[MAX_PATH]={0,};

	// IShellLink 객체를 생성하고 포인터를 구한다.
	hr=CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
		IID_IShellLink, (void **)&pSl);
	if (FAILED(hr))
		return E_FAIL;

	// 쇼트컷의 대상체와 설명을 설정한다.
	pSl->SetPath(szSrcFile);
	pSl->SetDescription(szDesc);
	pSl->SetArguments(szArgument);

	// 저장하기 위해 IPersistFile 객체를 생성한다.
	hr=pSl->QueryInterface(IID_IPersistFile, (void **)&pPf);
	if (FAILED(hr)) {
		pSl->Release();
		return E_FAIL;
	}

	// 유니코드로 파일 패스를 변경한 후 저장한다.
	MultiByteToWideChar(CP_ACP, 0, szLnkFile, -1, wszLnkFile, MAX_PATH);
	hr=pPf->Save(wszLnkFile, TRUE);

	// 객체 해제
	pPf->Release();
	pSl->Release();
	return hr;
}

void MyGetSpecialFolderPath(HWND hWnd, int nFolder, TCHAR *szPath)
{
	LPITEMIDLIST pidl;
	LPMALLOC pMalloc;

	SHGetSpecialFolderLocation(hWnd,nFolder,&pidl);
	SHGetPathFromIDList(pidl, szPath);
	SHGetMalloc(&pMalloc);
	pMalloc->Free(pidl);
	pMalloc->Release();
}

// 디렉토리 이하를 삭제한다.
BOOL DeleteTree(LPCTSTR Path)
{
	HANDLE hSrch;
	WIN32_FIND_DATA wfd;
	BOOL bResult=TRUE;
	TCHAR DelPath[MAX_PATH];
	TCHAR FullPath[MAX_PATH];
	TCHAR Drive[_MAX_DRIVE];
	TCHAR Dir[MAX_PATH];

	// 삭제대상 디렉토리는 최소한 C:\D 정도의 길이를 가져야 한다. C: 등과 인수가 혹시
	// 잘못 전달될 경우를 위한 최소한의 안전장치이다.
	if (lstrlen(Path) < 4)
		return FALSE;

	// \*.*가 붙어있지 않을 경우 붙인다.
	lstrcpy(DelPath,Path);
	if (lstrcmp(DelPath+lstrlen(DelPath)-4,"\\*.*") != 0)
		lstrcat(DelPath,"\\*.*");
	hSrch=FindFirstFile(DelPath,&wfd);
	if (hSrch == INVALID_HANDLE_VALUE) 
		return FALSE;
	_splitpath(DelPath,Drive,Dir,NULL,NULL);
	while (bResult) {
		wsprintf(FullPath,"%s%s%s",Drive,Dir,wfd.cFileName);
		// 읽기 전용 속성을 가진 경우 이 속성을 먼저 해제한다.
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_READONLY) {
			SetFileAttributes(FullPath,FILE_ATTRIBUTE_NORMAL);
		}
		// 서브 디렉토리가 발견되면 디렉토리 안까지 삭제한다. '.', '..'은 제외
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (lstrcmp(wfd.cFileName,".") && lstrcmp(wfd.cFileName,"..")) {
				DeleteTree(FullPath);
			}
		} else {
			DeleteFile(FullPath);
		}
		bResult=FindNextFile(hSrch,&wfd);
	}
	FindClose(hSrch);
	// 마지막으로 자기 자신을 삭제한다.끝의 \를 제거한다.
	wsprintf(FullPath, "%s%s", Drive, Dir);
	FullPath[lstrlen(FullPath)-1] = 0;
	if (RemoveDirectory(FullPath) == FALSE) {
		// 탐색기가 사용하고 있는 폴더는 삭제되지 않으므로 필요할 경우
		// 에러 처리를 하도록 한다.
	}
	return TRUE;
}

void Install()
{
	TCHAR szPath[MAX_PATH];
	TCHAR szLink[MAX_PATH];

	MyGetSpecialFolderPath(hWndMain, CSIDL_PROGRAMS, szPath);
	lstrcat(szPath, "\\설치 테스트");
	CreateDirectory(szPath,NULL);

	lstrcpy(szLink, szPath);
	lstrcat(szLink, "\\계산기.lnk");
	MyCreateShortCut("c:\\Program Files\\InstTest\\Calc.exe", szLink, "", "계산기");
	lstrcpy(szLink, szPath);
	lstrcat(szLink, "\\카드놀이.lnk");
	MyCreateShortCut("c:\\Program Files\\InstTest\\Sol.exe", szLink, "", "카드놀이");
	lstrcpy(szLink, szPath);
	lstrcat(szLink, "\\Readme.lnk");
	MyCreateShortCut("c:\\Program Files\\InstTest\\Readme.txt", szLink, "", "Readme");
}

void Uninstall()
{
	TCHAR szPath[MAX_PATH];

	MyGetSpecialFolderPath(hWndMain, CSIDL_PROGRAMS, szPath);
	lstrcat(szPath, "\\설치 테스트");
	DeleteTree(szPath);
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes="마우스 왼쪽 버튼을 누르면 설치, 오른쪽 버튼을 누르면 제거";

	switch (iMessage) {
	case WM_CREATE:
		CoInitialize(0);
		return 0;
	case WM_LBUTTONDOWN:
		Install();
		MessageBox(hWnd,"설치가 완료되었습니다","알림",MB_OK);
		return 0;
	case WM_RBUTTONDOWN:
		Uninstall();
		MessageBox(hWnd,"제거되었습니다.","알림",MB_OK);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,50,50,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		CoUninitialize();
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
