#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("Mouse5");

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

HBITMAP hbit;

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

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc, hMemDC;
	HBITMAP OldBitmap;
	static int x;
	static int y;
	static BOOL bNowDraw=FALSE;
	switch (iMessage) {
	// 1024*768 크기의 메모리 비트맵을 만들고 초기화한다.
	case WM_CREATE:
		hdc=GetDC(hWnd);
		hbit=CreateCompatibleBitmap(hdc, 1024,768);
		hMemDC=CreateCompatibleDC(hdc);
		OldBitmap=(HBITMAP)SelectObject(hMemDC, hbit);
		SelectObject(hMemDC, GetStockObject(WHITE_PEN));
		Rectangle(hMemDC, 0,0,1024,768);
		SelectObject(hMemDC, OldBitmap);
		DeleteDC(hMemDC);
		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_RBUTTONDOWN:
		DDB2DIB(hbit,"test.bmp");
		MessageBox(hWnd,"그림 파일을 test.bmp로 저장했습니다.","알림",MB_OK);
		return 0;
	// 그리기를 시작한다.
	case WM_LBUTTONDOWN:
		x=(int)(short)LOWORD(lParam);
		y=(int)(short)HIWORD(lParam);
		bNowDraw=TRUE;
		SetCapture(hWnd);
		return 0;
	// 화면과 메모리 비트맵으로 동시에 출력한다.
	case WM_MOUSEMOVE:
		if (bNowDraw==TRUE) {
			hdc=GetDC(hWnd);
			hMemDC=CreateCompatibleDC(hdc);
			OldBitmap=(HBITMAP)SelectObject(hMemDC, hbit);
			MoveToEx(hdc,x,y,NULL);
			MoveToEx(hMemDC,x,y,NULL);
			x=(int)(short)LOWORD(lParam);
			y=(int)(short)HIWORD(lParam);
			LineTo(hdc,x,y);
			LineTo(hMemDC,x,y);
			SelectObject(hMemDC, OldBitmap);
			DeleteDC(hMemDC);
			ReleaseDC(hWnd,hdc);
		}
		return 0;
	case WM_LBUTTONUP:
		bNowDraw=FALSE;
		ReleaseCapture();
		return 0;
	// 메모리 비트맵에 저장된 그림을 화면으로 고속 전송한다.
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		hMemDC=CreateCompatibleDC(hdc);
		OldBitmap=(HBITMAP)SelectObject(hMemDC, hbit);
		BitBlt(hdc,0,0,1024,768,hMemDC,0,0,SRCCOPY);
		SelectObject(hMemDC, OldBitmap);
		DeleteDC(hMemDC);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		DeleteObject(hbit);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
