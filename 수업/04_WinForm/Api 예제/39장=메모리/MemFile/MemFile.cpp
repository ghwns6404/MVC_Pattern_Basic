#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("MemFile");

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

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	HANDLE hFile, hFMap;
	TCHAR *PtrInFile;
	RECT rt;
	switch (iMessage) {
	case WM_LBUTTONDOWN:
		hdc=GetDC(hWnd);
		// 파일을 연다
		hFile=CreateFile("NaRu.txt",GENERIC_READ, 0, 
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) {
			MessageBox(hWnd, "파일이 없습니다", "에러", MB_OK);
		} else {
			// 파일 맵핑 오브젝트를 만든다.
			hFMap=CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
			// 주소 공간에 맵한다.
			PtrInFile=(TCHAR *)MapViewOfFile(hFMap, FILE_MAP_READ,0,0,0);
			SetRect(&rt,10,10,640,400);
			DrawText(hdc,PtrInFile,GetFileSize(hFile, NULL),&rt,DT_EXPANDTABS);
			UnmapViewOfFile(PtrInFile);
			CloseHandle(hFMap);
			CloseHandle(hFile);
		}
		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_RBUTTONDOWN:
		// 파일을 연다
		hFile=CreateFile("Alpha.txt",GENERIC_READ | GENERIC_WRITE, 0, 
			NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) {
			MessageBox(hWnd, "파일이 없습니다", "에러", MB_OK);
		} else {
			// 파일 맵핑 오브젝트를 만든다.
			hFMap=CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
			// 주소 공간에 맵한다.
			PtrInFile=(TCHAR *)MapViewOfFile(hFMap, FILE_MAP_WRITE,0,0,0);
			strncpy(PtrInFile, "TEST", 4);
			PtrInFile[4]=' ';
			UnmapViewOfFile(PtrInFile);
			CloseHandle(hFMap);
			CloseHandle(hFile);
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
