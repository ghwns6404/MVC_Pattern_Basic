#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("AsyncIO");

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

TCHAR *buf;
HANDLE hFile;
OVERLAPPED ov;
#define MEGA 1048576
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes="마우스 왼쪽 버튼을 누르면 비동기 출력을 합니다";
	BOOL Result;
	DWORD dwWritten, Tran=0;
	TCHAR str[256],sTmp[256];
	TCHAR *p;
	static DWORD len, Count=1;
	int i,tlen;

	switch (iMessage) {
	case WM_CREATE:
		// 50M 분량의 버퍼를 준비한다.
		buf=(TCHAR *)calloc(50,MEGA);
		if (buf == NULL) {
			MessageBox(hWnd,"메모리 할당에 실패했습니다","에러",MB_OK);
			return -1;
		}

		// 버퍼에 문자열을 준비한다.
		p=buf;
		for (i=1;;i++) {
			wsprintf(sTmp,"%dLine : This file is a test file for overlapped file I/O\r\n",i);
			tlen=lstrlen(sTmp);
			if (p-buf+tlen >= 50*MEGA) break;
			lstrcpy(p,sTmp);
			p+=tlen;
		}
		len=p-buf;
		return 0;
	case WM_LBUTTONDOWN:
		// 출력 파일 생성
		hFile=CreateFile("c:\\BigFile.txt",GENERIC_WRITE,0,NULL,
			CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,NULL);
		if (hFile == INVALID_HANDLE_VALUE) {
			MessageBox(hWnd,"파일 생성 에러","",0);
			return 0;
		}

		// 비동기 출력
		ov.Offset=0;
		ov.OffsetHigh=0;
		ov.hEvent=NULL;
		Result=WriteFile(hFile,buf,len,&dwWritten,&ov);
		if ((Result==FALSE) && (GetLastError()==ERROR_IO_PENDING)) {
			MessageBeep(0);
		}
		SetTimer(hWnd,1,10,NULL);
		return 0;
	case WM_TIMER:
		// 출력 상태를 조사해 보여준다.
//* 파일 핸들을 사용하는 방법
		if (WaitForSingleObject(hFile,0)==WAIT_OBJECT_0) {
			KillTimer(hWnd,1);
			CloseHandle(hFile);
			hFile=NULL;
			wsprintf(str,"카운트=%d. 작업 완료",Count);
		} else {
			wsprintf(str,"카운트=%d. 출력중",Count);
		}
//*/
/* 비동기 입출력 상태를 조사하는 방법
		Result=GetOverlappedResult(hFile,&ov,&Tran,FALSE);
		if (Tran>=len) {
			wsprintf(str,"카운트=%d,총 %d바이트 출력을 완료했습니다.",Count,Tran);
			KillTimer(hWnd,1);
			CloseHandle(hFile);
			hFile=NULL;
		} else {
			wsprintf(str,"카운트=%d,%d바이트 출력 중입니다",Count,Tran);
		}
//*/
		Count++;
		hdc=GetDC(hWnd);
		TextOut(hdc,10,10,str,lstrlen(str));
		ReleaseDC(hWnd,hdc);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,10,30,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		if (hFile)
			CloseHandle(hFile);
		free(buf);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

