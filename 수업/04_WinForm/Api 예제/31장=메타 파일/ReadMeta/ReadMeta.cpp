#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("ReadMeta");

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

// 16비트 메타 파일을 32비트 메타 파일로 변환하여 메타 파일 핸들을 리턴한다. 
// 에러 발생시 NULL을 리턴한다.
HENHMETAFILE ConvertWinToEnh(LPTSTR wmf)
{
	HENHMETAFILE hEnh;
	HMETAFILE wfile;
	DWORD dwSize;
	LPBYTE pBits;
	METAFILEPICT mp;
	HDC hdc;

	// 16비트 메타 파일을 읽고 메타 파일 크기만큼 메모리를 할당한다.
	wfile=GetMetaFile(wmf);
	if (wmf==NULL) 
		return NULL;
	dwSize=GetMetaFileBitsEx(wfile, 0,NULL);
	if (dwSize==0) {
		DeleteMetaFile(wfile);
		return NULL;
	}
	pBits=(LPBYTE)malloc(dwSize);
	
	// 메타 파일의 내용을 버퍼로 읽어들인다.
	GetMetaFileBitsEx(wfile, dwSize, pBits);
	mp.mm=MM_ANISOTROPIC;
	mp.xExt=1000;
	mp.yExt=1000;
	mp.hMF=NULL;

	// 32비트 메타 파일을 만든다.
	hdc=GetDC(NULL);
	hEnh=SetWinMetaFileBits(dwSize, pBits, hdc, &mp);
	ReleaseDC(NULL, hdc);
	DeleteMetaFile(wfile);
	free(pBits);
	return hEnh;
}

#pragma pack(push)
#pragma pack(2)
typedef struct
{
	DWORD		dwKey;
	WORD		hmf;
	SMALL_RECT	bbox;
	WORD		wInch;
	DWORD		dwReserved;
	WORD		wCheckSum;
} APMHEADER, *PAPMHEADER;
#pragma pack(pop)

// 플레이스블 메타 파일을 32비트 메타 파일로 변경해 준다. 
// 에러 발생시 NULL을 리턴한다.
HENHMETAFILE ConvertPlaToEnh(LPTSTR szFileName)
{
	HENHMETAFILE	hEnh;
	DWORD			dwSize;
	LPBYTE			pBits;
	METAFILEPICT	mp;
	HDC				hdc;
	HANDLE			hFile;

	// 32비트 메타 파일이 아니면 플레이스블 메타 파일로 읽는다.
	// 파일 크기만큼 메모리를 할당하고 메타 파일을 읽어들인다.
	hFile = CreateFile( szFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )
		return NULL;
	dwSize = GetFileSize( hFile, NULL );
	pBits = (LPBYTE)malloc( dwSize );
	ReadFile( hFile, pBits, dwSize, &dwSize, NULL );
	CloseHandle( hFile );
	// 플레이스블 메타 파일이 맞는지 확인한다.
	if( ((PAPMHEADER)pBits)->dwKey != 0x9ac6cdd7l )	{
		free( pBits );
		return NULL;
	}

	// 구조체를 채운다.
	mp.mm = MM_ANISOTROPIC;
	mp.xExt = ((PAPMHEADER)pBits)->bbox.Right - ((PAPMHEADER)pBits)->bbox.Left;
	mp.xExt = ( mp.xExt * 2540l ) / (DWORD)(((PAPMHEADER)pBits)->wInch);
	mp.yExt = ((PAPMHEADER)pBits)->bbox.Bottom - ((PAPMHEADER)pBits)->bbox.Top;
	mp.yExt = ( mp.yExt * 2540l ) / (DWORD)(((PAPMHEADER)pBits)->wInch);
	mp.hMF = NULL;
	// 메타 파일을 만든다.
	hdc = GetDC( NULL );
	hEnh = SetWinMetaFileBits( dwSize, &(pBits[sizeof(APMHEADER)]), hdc, &mp );
	ReleaseDC( NULL, hdc );
	free( pBits );
	return hEnh;
}

// 32비트 메타 파일의 핸들을 리턴한다. 16비트 메타 파일이나 플레이스블 메타 파일
// 일 경우 32비트 메타 파일로 변환해준다.
HENHMETAFILE ReadMeta(LPTSTR FileName)
{
	HENHMETAFILE hEnh;
	// 32비트 메타 파일의 핸들을 구해 리턴한다.
	hEnh=GetEnhMetaFile(FileName);
	if (hEnh != NULL)
		return hEnh;
	// 32비트 메타 파일이 아닐 경우 16비트 포멧으로 읽어보고 32비트 전환한다.
	hEnh=ConvertWinToEnh(FileName);
	if (hEnh != NULL)
		return hEnh;
	// 16비트 메타 파일도 아닐 경우 플레이스블 메타 파일을 32비트로 전환한다.
	hEnh=ConvertPlaToEnh(FileName);
	if (hEnh != NULL)
		return hEnh;
	// 세 경우 다 해당하지 않을 경우 NULL을 리턴한다.
	return NULL;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	OPENFILENAME ofn;
	TCHAR szFileName[MAX_PATH];
	static HENHMETAFILE hEnh;
	RECT rt;
	switch (iMessage) {
	case WM_LBUTTONDOWN:
		// 출력하고자 하는 메타 파일의 핸들을 구한다.
		szFileName[0]=0;
		memset(&ofn, 0, sizeof(OPENFILENAME));
		ofn.lStructSize=sizeof(OPENFILENAME);
		ofn.hwndOwner=hWnd;
		ofn.lpstrFilter="Meta File\0*.?MF\0";
		ofn.nFilterIndex=1;
		ofn.lpstrFile=szFileName;
		ofn.nMaxFile=MAX_PATH;
		if (GetOpenFileName(&ofn)) {
			if (hEnh) DeleteEnhMetaFile(hEnh);
			hEnh=ReadMeta(szFileName);
			if (hEnh) {
				InvalidateRect(hWnd,NULL,TRUE);
			}
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd,&ps);
		if (hEnh) {
			GetClientRect(hWnd, &rt);
			PlayEnhMetaFile(hdc, hEnh, &rt);
		}
		EndPaint(hWnd,&ps);
		return 0;
	case WM_DESTROY:
		if (hEnh) DeleteEnhMetaFile(hEnh);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
