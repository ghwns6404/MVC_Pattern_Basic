#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("PlaToEnh");

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

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HENHMETAFILE wmf;
	RECT rt;
	switch (iMessage) {
	case WM_PAINT:
		hdc=BeginPaint(hWnd,&ps);
		GetClientRect(hWnd, &rt);
		wmf=ConvertPlaToEnh("buttrfly.wmf");
		PlayEnhMetaFile(hdc, wmf, &rt);
		EndPaint(hWnd,&ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}



