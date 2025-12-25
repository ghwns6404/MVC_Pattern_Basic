#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("MoveCaret");

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

TCHAR str[4096];		// 입력된 문자열을 저장할 버퍼
int off;				// 버퍼의 현재 오프셋

// 현재 오프셋 위치로 캐럿을 옮긴다.
void MySetCaretPos()
{
	SIZE sz;
	HDC hdc;

	hdc=GetDC(hWndMain);
	GetTextExtentPoint32(hdc, str, off, &sz);
	ReleaseDC(hWndMain, hdc);
	SetCaretPos(sz.cx, 0);
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	int len,i;

	switch (iMessage) {
	case WM_CHAR:
		if (wParam == 8)		// BS는 KEYDOWN에서 처리
			return 0;

		// 오프셋 위치에 문자를 삽입한다.
		len=lstrlen(str);
		for (i=len+1;i>=off;i--)
			str[i]=str[i-1];
		str[off++]=wParam;
		MySetCaretPos();
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
			if (off > 0) {
				off--;
				MySetCaretPos();
			}
			break;
		case VK_RIGHT:
			if (off < (int)lstrlen(str)) {
				off++;
				MySetCaretPos();
			}
			break;
		case VK_HOME:
			off=0;
			MySetCaretPos();
			break;
		case VK_END:
			off=lstrlen(str);
			MySetCaretPos();
			break;
		case VK_BACK:		// 한칸 뒤로 간 후 Delete
			if (off > 0) {
				off--;
				SendMessage(hWnd,WM_KEYDOWN,VK_DELETE,0);
				MySetCaretPos();
			}
			break;
		case VK_DELETE:
			len=lstrlen(str);
			for (i=off;i<len;i++) {
				str[i]=str[i+1];
			}
			InvalidateRect(hWnd,NULL,TRUE);
			break;
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,0,0,str,lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_LBUTTONDOWN:
		str[0]=NULL;
		InvalidateRect(hWnd, NULL, TRUE);
		SetCaretPos(0,0);
		return 0;
	case WM_SETFOCUS:
		CreateCaret(hWnd, NULL, 2, 16);
		ShowCaret(hWnd);
		return 0;
	case WM_KILLFOCUS:
		DestroyCaret();
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

