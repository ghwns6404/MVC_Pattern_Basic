#include <windows.h>
#include <shlobj.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("URLShortCut");

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

#include <intshcut.h>
BOOL MakeURLShortCut2(TCHAR *URL,TCHAR *Path)
{
	IUniformResourceLocator *pURL;
	IPersistFile *pPf;
	WCHAR wszSCPath[MAX_PATH]={0,};
	HRESULT hr;

	// 쇼트컷에 기록될 정보를 만든다.
	hr=CoCreateInstance(CLSID_InternetShortcut, NULL, CLSCTX_INPROC_SERVER,
		IID_IUniformResourceLocator, (void **)&pURL);
	if (FAILED(hr))
		return FALSE;
	pURL->SetURL(URL,0);

	// 쇼트컷 파일을 생성한다.
	hr=pURL->QueryInterface(IID_IPersistFile, (void **)&pPf);
	if (FAILED(hr)) {
		pURL->Release();
		return FALSE;
	}
	MultiByteToWideChar(CP_ACP, 0, Path, -1, wszSCPath, MAX_PATH);
	pPf->Save(wszSCPath,TRUE);
	pURL->Release();
	pPf->Release();
	return TRUE;
}

// 인터넷 URL에 대한 쇼트컷을 특수폴더나 지정한 경로에 만든다.
// URL:URL
// IconPath:아이콘 파일의 풀 패스
// nFolder:특수 폴더. 이 폴더에 FileName이 지정한 서브 디렉토리에 쇼트컷이 생성된다. 이 값이 -10이면 szFolder는 절대경로이다.
// szFileName:쇼트컷 파일의 파일명. 서브 디렉토리를 포함할 수 있으묘 nFolder가 -1이면 절대경로가 된다.
// 리턴값:쇼트컷 생성에 실패하면 FALSE를 리턴한다.
// 주의:쇼트컷이 만들어질 경로는 반드시 이미 존재해야 한다.
BOOL MakeURLShortCut(TCHAR *URL,TCHAR *IconPath,int nFolder,TCHAR *szFileName)
{
	LPMALLOC pMalloc;
	LPITEMIDLIST lpidl;
	TCHAR szSCPath[MAX_PATH];
	TCHAR szShortcutInfo[MAX_PATH];
	HANDLE hFile;
	DWORD dwWritten;

	// 쇼트컷이 만들어질 경로를 구한다. 특수 폴더와 절대경로값이 조합되어 하나의
	// 풀 패스가 만들어진다.
	if (nFolder != -1) {
		SHGetSpecialFolderLocation(HWND_DESKTOP, nFolder, &lpidl);
		SHGetPathFromIDList(lpidl, szSCPath);
		lstrcat(szSCPath, "\\");
		lstrcat(szSCPath, szFileName);
		SHGetMalloc(&pMalloc);
		pMalloc->Free(lpidl);
		pMalloc->Release();
	}
	if (nFolder == -1) {
		lstrcpy(szSCPath, szFileName);
	}

	// 쇼트컷에 기록될 정보를 만든다.
	wsprintf(szShortcutInfo, "[InternetShortcut]\r\nURL=%s\r\n"
		"IconIndex=0\r\nIconFile=%s\r\n"
		"Modified=000000000000000000\r\n",URL, IconPath);

	// 쇼트컷 파일을 생성한다.
	hFile = CreateFile(szSCPath, GENERIC_READ | GENERIC_WRITE, 0, NULL,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	WriteFile(hFile, szShortcutInfo, lstrlen(szShortcutInfo), &dwWritten, NULL);
	CloseHandle(hFile);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes="WinApi로 바로 가는 쇼트컷을 생성합니다";

	switch (iMessage) {
	case WM_CREATE:
		CoInitialize(0);
		return 0;
	case WM_LBUTTONDOWN:
		MakeURLShortCut("http://www.winapi.co.kr","c:\\winapi.ico",
			CSIDL_DESKTOP,"winapi.url");
		//MakeURLShortCut2("http://www.winapi.co.kr","c:\\winapi.url");
		MessageBox(hWnd,"URL 바로가기를 만들었습니다","알림",MB_OK);
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

