#include <windows.h>
#include <io.h>
#include "ShReg.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("AddFont");

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

TCHAR Stat[255];
BOOL g_bExist;
// 폰트를 설치한다.
BOOL InstallFont()
{
	TCHAR FontPath[MAX_PATH];
	TCHAR FilePath[MAX_PATH];

	// 폰트 파일의 경로를 구한다.
	GetCurrentDirectory(MAX_PATH,FilePath);
	if (lstrlen(FilePath) == 3) {
		lstrcat(FilePath,"SF Balloons.ttf");
	} else {
		lstrcat(FilePath,"\\SF Balloons.ttf");
	}
	
	// 폰트를 설치할 경로를 구한다. 윈도우즈 디렉토리 아래의 Fonts 폴더
	GetWindowsDirectory(FontPath, MAX_PATH);
	lstrcat(FontPath, "\\Fonts\\SF Balloons.ttf");

	// 폰트 파일을 복사한다. 단 이미 있는 경우는 복사할 필요없다.
	if (_access(FontPath,0) != 0) {
		if (CopyFile(FilePath,FontPath,FALSE) == FALSE) {
			lstrcpy(Stat,"폰트 복사에 실패함. 설치를 취소합니다.");
			return FALSE;
		}
	}

	// 폰트를 등록한다.
	if (AddFontResource(FontPath) == FALSE) {
		lstrcpy(Stat,"폰트 등록에 실패함. 설치를 취소합니다.");
		return FALSE;
	}
	SendMessage(HWND_BROADCAST,WM_FONTCHANGE,0,0);

	// 레지스트리에 값을 써 넣는다.
	OSVERSIONINFO vi;
	vi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	TCHAR szKey[255];
	GetVersionEx(&vi);
	if (vi.dwPlatformId == VER_PLATFORM_WIN32_NT) {
		lstrcpy(szKey,"Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts");
	} else {
		lstrcpy(szKey,"Software\\Microsoft\\Windows\\CurrentVersion\\Fonts");
	}
	SHRegWriteString(HKEY_LOCAL_MACHINE,szKey,"SF Balloons (TrueType)","SF Balloons.ttf");

	lstrcpy(Stat,"폰트를 설치했습니다.");
	return TRUE;
}

// 폰트를 삭제한다.
BOOL UnInstallFont()
{
	TCHAR FontPath[MAX_PATH];

	// 삭제할 폰트의 위치를 구한다.
	GetWindowsDirectory(FontPath, MAX_PATH);
	lstrcat(FontPath, "\\Fonts\\SF Balloons.ttf");

	// 폰트 등록을 취소한다.
	if (RemoveFontResource(FontPath) == FALSE) {
		lstrcpy(Stat,"폰트 등록 해제 실패함.");
		return FALSE;
	}
	SendMessage(HWND_BROADCAST,WM_FONTCHANGE,0,0);

	// 레지스트리의 값을 지운다.
	OSVERSIONINFO vi;
	vi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	TCHAR szKey[255];
	GetVersionEx(&vi);
	if (vi.dwPlatformId == VER_PLATFORM_WIN32_NT) {
		lstrcpy(szKey,"Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts");
	} else {
		lstrcpy(szKey,"Software\\Microsoft\\Windows\\CurrentVersion\\Fonts");
	}

	HKEY key;
	DWORD dwDisp;
	RegCreateKeyEx(HKEY_LOCAL_MACHINE,szKey,0,NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,NULL,&key,&dwDisp);
	RegDeleteValue(key,"SF Balloons (TrueType)");
	RegCloseKey(key);

	// 폰트 파일을 삭제한다.
	if (DeleteFile(FontPath) == FALSE) {
		lstrcpy(Stat,"폰트 파일 삭제에 실패함.");
		return FALSE;
	}

	lstrcpy(Stat,"폰트를 제거했습니다.");
	return TRUE;
}

// SF Balloons 폰트가 발견되면 g_bExist를 TRUE로 변경한다.
int CALLBACK EnumFamCallBack(ENUMLOGFONT FAR *lpelf, NEWTEXTMETRIC FAR *lpntm, 
							 int FontType, LPARAM lParam)
{
	if (stricmp(lpelf->elfLogFont.lfFaceName, "SF Balloons")==0)
		g_bExist=TRUE;
	return TRUE;
}

// 이뉴머레이션을 하면서 폰트 설치 여부를 조사한다.
void ReEnum()
{
	HDC hdc;

	hdc=GetDC(hWndMain);
	EnumFontFamilies(hdc, NULL, (FONTENUMPROC)EnumFamCallBack, (LPARAM)NULL);
	ReleaseDC(hWndMain, hdc);
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HFONT font,oldfont;
	TCHAR *str="폰트를 설치, 또는 제거합니다. 왼쪽 마우스 버튼=설치, 오른쪽 마우스 버튼=제거";
	TCHAR *Sample="SF Baloons Font Test";

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		ReEnum();
		if (g_bExist==TRUE) {
			lstrcpy(Stat,"폰트가 설치되어 있습니다.");
		} else {
			lstrcpy(Stat,"폰트가 설치되어 있지 않습니다.");
		}
		return 0;
	case WM_LBUTTONDOWN:
		SetCursor(LoadCursor(NULL,IDC_WAIT));
		InstallFont();
		InvalidateRect(hWnd, NULL, TRUE);
		SetCursor(LoadCursor(NULL,IDC_ARROW));
		return 0;
	case WM_RBUTTONDOWN:
		SetCursor(LoadCursor(NULL,IDC_WAIT));
		UnInstallFont();
		InvalidateRect(hWnd, NULL, TRUE);
		SetCursor(LoadCursor(NULL,IDC_ARROW));
		return 0;
	case WM_FONTCHANGE:
		InvalidateRect(hWnd,NULL,TRUE);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,10,10,Stat,lstrlen(Stat));
		TextOut(hdc,10,30,str,lstrlen(str));
		font=CreateFont(50,0,0,0,0,0,0,0,ANSI_CHARSET,3,2,1,
			VARIABLE_PITCH | FF_DONTCARE,"SF Balloons");
		oldfont=(HFONT)SelectObject(hdc,font);
		TextOut(hdc,10,100,Sample,lstrlen(Sample));
		SelectObject(hdc,oldfont);
		DeleteObject(font);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
