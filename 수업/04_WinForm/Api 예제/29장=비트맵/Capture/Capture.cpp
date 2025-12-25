#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("Capture");

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
	WndClass.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style=0;
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

// DDB를 DIB로 바꾸어 파일로 저장한다.
void DDB2DIB(HBITMAP hbit,LPCTSTR Path)
{
	BITMAPFILEHEADER fh;
	BITMAPINFOHEADER ih;
	BITMAP bit;
	BITMAPINFO *pih;
	int PalSize;
	HANDLE hFile;
	DWORD dwWritten,Size;
	HDC hdc;

	// 전체 화면에 대한 DC를 구한다.
	hdc=GetDC(NULL);

	// 비트맵 정보로부터 정보 구조체를 초기화한다.
	GetObject(hbit,sizeof(BITMAP),&bit);
	ih.biSize=sizeof(BITMAPINFOHEADER);
	ih.biWidth=bit.bmWidth;
	ih.biHeight=bit.bmHeight;
	ih.biPlanes=1;
	ih.biBitCount=bit.bmPlanes*bit.bmBitsPixel;
	ih.biCompression=BI_RGB;
	ih.biSizeImage=0;
	ih.biXPelsPerMeter=0;
	ih.biYPelsPerMeter=0;
	ih.biClrUsed=0;
	ih.biClrImportant=0;

	// 정보 구조체 + 팔레트 크기만큼 메모리를 할당하고 이 버퍼에 
	// 정보 구조체를 복사한다.
	PalSize=(ih.biBitCount > 8 ? 0:1 << ih.biBitCount)*sizeof(RGBQUAD);
	pih=(BITMAPINFO *)malloc(ih.biSize+PalSize);
	pih->bmiHeader=ih;

	// 비트맵의 크기를 구한다.
	GetDIBits(hdc,hbit,0,bit.bmHeight,NULL,pih,DIB_RGB_COLORS);
	ih=pih->bmiHeader;

	// 비트맵 크기가 구해지지 않았을 경우 수작업으로 직접 계산한다.
	if (ih.biSizeImage == 0) {
		ih.biSizeImage=((((ih.biWidth*ih.biBitCount)+31) & ~31) >> 3) * ih.biHeight;
	}

	// 래스터 데이터를 읽기위해 메모를 재할당한다.
	Size=ih.biSize+PalSize+ih.biSizeImage;
	pih=(BITMAPINFO *)realloc(pih,Size);

	// 래스터 데이터를 읽어들인다.
	GetDIBits(hdc,hbit,0,bit.bmHeight,(PBYTE)pih+ih.biSize+PalSize,pih,DIB_RGB_COLORS);

	// 파일 헤더를 만든다.
	fh.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+PalSize;
	fh.bfReserved1=0;
	fh.bfReserved2=0;
	fh.bfSize=Size+sizeof(BITMAPFILEHEADER);
	fh.bfType=0x4d42;

	// 파일을 생성하고 파일 헤더와 정보 구조체, 팔레트, 래스터 데이터를 출력한다.
	hFile=CreateFile(Path,GENERIC_WRITE,0,NULL,
		CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	WriteFile(hFile,&fh,sizeof(fh),&dwWritten,NULL);
	WriteFile(hFile,pih,Size,&dwWritten,NULL);

	free(pih);
	ReleaseDC(NULL,hdc);
	CloseHandle(hFile);
}

HBITMAP hBit=NULL;		// 캡처된 비트맵
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rt;
	HBITMAP hOldBitmap;
	HDC hMemDC,hScrDC;
	TCHAR *Mes="핫키 F5를 사용하거나 타이머로 원하는 화면을 캡처하십시오";
	OPENFILENAME OFN;
	TCHAR lpstrFile[MAX_PATH]="";

	switch (iMessage) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		// 핫키로 캡처하기:핫키를 등록한다.
		case IDM_FILE_HOTKEY:
			RegisterHotKey(hWnd,0x0000,0,VK_F5);
			ShowWindow(hWnd,SW_MINIMIZE);
			break;
		// 타이머로 캡처하기:타이머를 설치한다.
		case IDM_FILE_TIMER:
			SetTimer(hWnd,0,5000,NULL);
			ShowWindow(hWnd,SW_MINIMIZE);
			break;
		// 캡처된 그림을 파일로 저장하기
		case IDM_FILE_SAVE:
			if (hBit==NULL)
				break;
			memset(&OFN, 0, sizeof(OPENFILENAME));
			OFN.lStructSize = sizeof(OPENFILENAME);
			OFN.hwndOwner=hWnd;
			OFN.lpstrFilter="Bmp File(*.Bmp)\0";
			OFN.lpstrFile=lpstrFile;
			OFN.nMaxFile=MAX_PATH;
			OFN.lpstrDefExt="bmp";
			OFN.lpstrTitle="저장할 파일 이름을 입력하시오";
			if (GetSaveFileName(&OFN)!=0) {
				DDB2DIB(hBit,lpstrFile);
			}
			break;
		}
		return 0;
	// 핫키를 누르거나 지정한 시간이 경과했을 때 화면을 캡처한다.
	case WM_HOTKEY:
	case WM_TIMER:
		// 포커스를 가진 윈도우의 좌표를 조사한다.
		GetWindowRect(GetForegroundWindow(),&rt);

		// 캡처 대상이 화면 영역밖이 되지 않도록 한다.
		rt.left=max(0,rt.left);
		rt.top=max(0,rt.top);
		rt.right=min(rt.right,GetSystemMetrics(SM_CXSCREEN));
		rt.bottom=min(rt.bottom,GetSystemMetrics(SM_CYSCREEN));

		// 전체 화면 DC와 메모리 DC를 만들고 메모리 비트맵을 선택한다.
		hScrDC=CreateDC("DISPLAY",NULL,NULL,NULL);
		hMemDC=CreateCompatibleDC(hScrDC);
		hBit=CreateCompatibleBitmap(hScrDC,
			rt.right-rt.left,rt.bottom-rt.top);
		hOldBitmap=(HBITMAP)SelectObject(hMemDC,hBit);

		// 화면을 메모리 비트맵으로 복사한다.
		BitBlt(hMemDC,0,0,rt.right-rt.left,rt.bottom-rt.top,
			hScrDC,rt.left,rt.top,SRCCOPY);
		SelectObject(hMemDC,hOldBitmap);
		DeleteDC(hMemDC);
		DeleteDC(hScrDC);

		// 핫키를 해제하고 타이머를 없앤다.
		if (iMessage == WM_HOTKEY) {
			UnregisterHotKey(hWnd,0);
		} else {
			KillTimer(hWnd,0);
		}

		ShowWindow(hWnd,SW_RESTORE);
		return 0;
	// 캡처된 비트맵을 작업 영역에 출력한다.
	case WM_PAINT:
		hdc=BeginPaint(hWnd,&ps);
		if (hBit != NULL) {
			DrawBitmap(hdc,0,0,hBit);
		} else {
			TextOut(hdc,50,50,Mes,lstrlen(Mes));
		}
		EndPaint(hWnd,&ps);
		return 0;

	// 비트맵을 파괴하고 핫키를 등록해제한다.
	case WM_DESTROY:
		if (hBit != NULL)
			DeleteObject(hBit);
		UnregisterHotKey(hWnd,0);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
