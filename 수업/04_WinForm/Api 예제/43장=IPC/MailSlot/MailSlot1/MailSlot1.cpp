#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("MailSlot1");

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
		100,100,600,150,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);

	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

HANDLE hMail;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	DWORD cbMessage, cbRead;
	TCHAR lpBuffer[256];
	TCHAR *Mes="왼쪽 버튼을 누르면 메시지를 받습니다";
	switch (iMessage) {
	case WM_CREATE:
		hMail=CreateMailslot("\\\\.\\mailslot\\Test",0,0,NULL);
		if (hMail==INVALID_HANDLE_VALUE)
			MessageBox(hWnd,"메일슬롯 만들기 실패","에러",MB_OK);
		return 0;
	case WM_LBUTTONDOWN:
		GetMailslotInfo(hMail,NULL,&cbMessage,NULL,NULL);
		if (cbMessage == MAILSLOT_NO_MESSAGE) {
			MessageBox(hWnd,"대기중인 메시지가 없습니다","알림",MB_OK);
			return 0;
		}
		ReadFile(hMail,lpBuffer,cbMessage,&cbRead,NULL);
		MessageBox(hWnd,lpBuffer,"읽은 메시지",MB_OK);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,50,50,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		CloseHandle(hMail);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
