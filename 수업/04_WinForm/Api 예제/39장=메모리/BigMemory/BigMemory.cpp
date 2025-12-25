#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("BigMemory");

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

#define MEGA 1048576
PBYTE stptr=NULL;
TCHAR Status[256]="할당되지 않았습니다";

void FreeRecords()
{
	// 할당되어 있던 메모리를 해제한다.
	if (stptr != NULL) {
		if (VirtualFree(stptr, 100*MEGA, MEM_DECOMMIT)==0) {
			MessageBox(hWndMain, "메모리 확정 해제 실패","에러",MB_OK);
		}
		if (VirtualFree(stptr, 0, MEM_RELEASE)==0) {
			MessageBox(hWndMain, "메모리 예약 해제 실패","에러",MB_OK);
		}
		stptr=NULL;
	}
	wsprintf(Status,"할당되지 않았습니다");
	InvalidateRect(hWndMain,NULL,TRUE);
}

// 세 개의 포인터 변수가 필요하다.
// stptr:최초 할당된 메모리 공간의 시작 주소
// endptr:현재까지 확정된 메모리 공간의 끝 주소.
// nowptr:현재 사용중인 메모리의 주소
void ReadRecords()
{
	int i, c;
	int RecSize;
	PBYTE nowptr, endptr;

	FreeRecords();

	// 100M의 주소 공간을 예약한다.
	stptr=(PBYTE)VirtualAlloc(NULL, 100*MEGA, MEM_RESERVE, PAGE_READWRITE);
	if (stptr == NULL) {
		MessageBox(hWndMain, "메모리 예약 실패","에러",MB_OK);
		return;
	}
	nowptr=stptr;
	endptr=stptr;

	// 최대 100개의 레코드를 읽어들인다.
	c=rand() % 90 + 10;
	for (i=0;i<c;i++) {
		// 확정된 메모리가 1M 미만이면 1M를 더 확정한다.
		if (endptr-nowptr < MEGA) {
			VirtualAlloc(endptr,MEGA,MEM_COMMIT,PAGE_READWRITE);
			endptr+=MEGA;
		}

		// 10K~1M 사이의 레코드를 읽어들인다.
		RecSize=((rand() % 100)+1)*10240;
		memset(nowptr,i,RecSize);
		nowptr+=RecSize;
	}

	wsprintf(Status,"예약:100 메가, 확정:%d 메가, 사용:%d 메가", 
		(endptr-stptr)/MEGA,(nowptr-stptr)/MEGA);
	InvalidateRect(hWndMain,NULL,TRUE);
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes="왼쪽 마우스 버튼:메모리 할당, 오른쪽 마우스 버튼:메모리 해제";
	
	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		srand(GetTickCount());
		return 0;
	case WM_LBUTTONDOWN:
		SetCursor(LoadCursor(NULL,IDC_WAIT));
		ReadRecords();
		SetCursor(LoadCursor(NULL,IDC_ARROW));
		return 0;
	case WM_RBUTTONDOWN:
		FreeRecords();
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,50,50,Mes,lstrlen(Mes));
		TextOut(hdc,50,80,Status,lstrlen(Status));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		FreeRecords();
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

