#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("Except");

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

/* 예외 핸들러를 호출한다.
void func()
{
	TCHAR *str;
	__try {
		strcpy(str, "Test");
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		MessageBox(NULL, "str pointer is invalid", "Exception", MB_OK);
	}
}
//*/

/* 예외 문장 재실행
void func()
{
	int Div=0, i;
	__try {
		i=2/Div;
	}
	__except (Div=1,EXCEPTION_CONTINUE_EXECUTION) {
	}
}
//*/

/* 두 개의 예외 발생
void func()
{
	int Div=0, i;
	TCHAR *str;
	__try {
		i=2/Div;
		strcpy(str, "Test");
	}
	__except (GetExceptionCode()==EXCEPTION_INT_DIVIDE_BY_ZERO ?
		Div=1,EXCEPTION_CONTINUE_EXECUTION:EXCEPTION_EXECUTE_HANDLER) {
		MessageBox(NULL, "str pointer is invalid", "Exception", MB_OK);
	}
}
//*/

/* 예외 필터에서 함수 호출
int Div=0;
DWORD ExcFilter(DWORD exCode)
{
	if (exCode == EXCEPTION_INT_DIVIDE_BY_ZERO) {
		Div=1;
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	else {
		return EXCEPTION_EXECUTE_HANDLER;
	}
}

void func()
{
	int i;
	TCHAR *str;
	__try {
		i=2/Div;
		strcpy(str, "Test");
	}
	__except (ExcFilter(GetExceptionCode())) {
		MessageBox(NULL, "str pointer is invalid", "Exception", MB_OK);
	}
}
//*/

/* try 블록에서 호출한 함수에서 예외 발생
int Div=0;
int DivInt(int a, int b)
{
	int result;
	result=a/b;
	return result;
}

void func()
{
	int i;
	__try {
		i=DivInt(2,Div);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		MessageBox(NULL, "Divide by zero", "Exception", MB_OK);
	}
}
//*/

//* 이전 try 블록 찾기
int Div=0;
int DivInt(int a, int b)
{
	TCHAR *str;
	int result;
	__try {
		result=a/b;
		strcpy(str, "Test");
	}
	__except(GetExceptionCode()==EXCEPTION_INT_DIVIDE_BY_ZERO ?
		EXCEPTION_CONTINUE_SEARCH:EXCEPTION_EXECUTE_HANDLER) {
		MessageBox(NULL, "str pointer is invalid", "Exception", MB_OK);
	}
	return result;
}

void func()
{
	int i;
	__try {
		i=DivInt(2,Div);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		MessageBox(NULL, "Divide by zero", "Exception", MB_OK);
	}
}
//*/

LRESULT APIENTRY WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage) {
	case WM_LBUTTONDOWN:
		func();
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
