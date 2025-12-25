#include <windows.h>
#include <shlobj.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ShortCut");

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

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR Note[MAX_PATH];
	TCHAR *Mes="왼쪽 버튼을 누르면 메모장으로 바로가기를 만듭니다.";

	switch (iMessage) {
	case WM_CREATE:
		CoInitialize(0);
		return 0;
	case WM_LBUTTONDOWN:
		GetWindowsDirectory(Note,MAX_PATH);
		lstrcat(Note,"\\Notepad.exe");
		MyCreateShortCut(Note, "c:\\메모장으로 가기.lnk", "", "메모장");
		MessageBox(hWnd,"바로가기를 만들었습니다","알림",MB_OK);
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,10,10,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		CoUninitialize();
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

