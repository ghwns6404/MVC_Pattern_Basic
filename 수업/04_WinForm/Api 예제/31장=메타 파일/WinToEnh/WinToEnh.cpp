#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("WinToEnh");

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

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	HENHMETAFILE hEnh;
	RECT rt;
	switch (iMessage) {
	case WM_LBUTTONDOWN:
		hdc=GetDC(hWnd);
		hEnh=ConvertWinToEnh("Test.wmf");
		if (hEnh==NULL) {
			MessageBox(hWnd, "변환할 수 없습니다.","에러",MB_OK);
		} else {
			SetRect(&rt, LOWORD(lParam), HIWORD(lParam), 
				LOWORD(lParam)+100, HIWORD(lParam)+100);
			PlayEnhMetaFile(hdc, hEnh, &rt);
			DeleteEnhMetaFile(hEnh);
		}
		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

