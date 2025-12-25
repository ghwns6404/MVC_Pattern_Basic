#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("DIBSection");

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

void DrawBitmap(HDC hdc,int x,int y,HBITMAP hBit)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx,by;
	BITMAP bit;

	MemDC=CreateCompatibleDC(hdc);
	OldBitmap=(HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit,sizeof(BITMAP),&bit);
	bx=bit.bmWidth;
	by=bit.bmHeight;

	BitBlt(hdc,x,y,bx,by,MemDC,0,0,SRCCOPY);

	SelectObject(MemDC,OldBitmap);
	DeleteDC(MemDC);
}

// DIB 섹션을 만들어 핸들을 리턴한다.
HBITMAP MakeDIBSection(LPCTSTR Path)
{
	HANDLE hFile;
	DWORD FileSize, dwRead;
	BITMAPFILEHEADER fh;
	HBITMAP hBit;
	BITMAPINFO *ih;
	PVOID pRaster;

	// 파일을 연다
	hFile=CreateFile(Path,GENERIC_READ,0,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile==INVALID_HANDLE_VALUE) {
		return NULL;
	}

	// 파일 헤더와 정보 구조체(색상 테이블 포함)를 읽어들인다.
	ReadFile(hFile,&fh,sizeof(BITMAPFILEHEADER),&dwRead,NULL);
	FileSize=fh.bfOffBits-sizeof(BITMAPFILEHEADER);
	ih=(BITMAPINFO *)malloc(FileSize);
	ReadFile(hFile,ih,FileSize,&dwRead,NULL);

	// DIB 섹션을 만들고 버퍼 메모리를 할당한다.
	hBit=CreateDIBSection(NULL,ih,DIB_RGB_COLORS,&pRaster,NULL,0);

	// 래스터 데이터를 읽어들인다.
	ReadFile(hFile,pRaster,fh.bfSize-fh.bfOffBits,&dwRead,NULL);
	free(ih);
	CloseHandle(hFile);

	return hBit;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static HBITMAP hBit;
	TCHAR Path[MAX_PATH];

	switch (iMessage) {
	case WM_CREATE:
		GetCurrentDirectory(MAX_PATH,Path);
		lstrcat(Path,"\\..\\hangyul.bmp");
		hBit=MakeDIBSection(Path);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		DrawBitmap(hdc,0,0,hBit);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		DeleteObject(hBit);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
