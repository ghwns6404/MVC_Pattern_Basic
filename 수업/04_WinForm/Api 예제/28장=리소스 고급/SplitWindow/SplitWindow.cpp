#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("SplitWindow");

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;

	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)(COLOR_BTNFACE+1);
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=NULL;
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);

	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

HWND hList,hEdit,hBtn;
int ListWidth;			// 리스트의 폭
int EditRatio;			// 에디트의 높이 비율(백분율)
#define GAP 3			// 차일드끼리의 간격
enum SPLIT { SP_NONE, SP_VERT, SP_HORZ };
SPLIT NowSplit=SP_NONE;

// 어떤 스플릿 바의 위치인지 조사한다.
SPLIT GetSplitter(POINT pt)
{
	RECT crt,hrt,vrt;
	int EditHeight;

	GetClientRect(hWndMain,&crt);
	EditHeight=crt.bottom*EditRatio/100;
	SetRect(&vrt,ListWidth-GAP,0,ListWidth,crt.bottom);
	if (PtInRect(&vrt,pt)) {
		return SP_VERT;
	}
	SetRect(&hrt,ListWidth,EditHeight-GAP,crt.right,EditHeight);
	if (PtInRect(&hrt,pt)) {
		return SP_HORZ;
	}
	return SP_NONE;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	RECT crt;
	int EditHeight;
	POINT pt;
	SPLIT sp;

	switch (iMessage) {
	case WM_CREATE:
		ListWidth=200;
		EditRatio=60;
		hList=CreateWindowEx(WS_EX_CLIENTEDGE,"listbox",NULL,WS_CHILD | WS_VISIBLE | 
			WS_BORDER | LBS_NOINTEGRALHEIGHT,0,0,0,0,hWnd,(HMENU)0,g_hInst,NULL);
		hEdit=CreateWindowEx(WS_EX_CLIENTEDGE,"edit",NULL,WS_CHILD | WS_VISIBLE | 
			WS_BORDER | ES_MULTILINE,0,0,0,0,hWnd,(HMENU)1,g_hInst,NULL);
		SendMessage(hList,LB_ADDSTRING,0,(LPARAM)"리스트다");
		SetWindowText(hEdit,"에디트다");
		hBtn=CreateWindow("button","버튼이다",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			0,0,0,0,hWnd,(HMENU)2,g_hInst,NULL);
		hWndMain=hWnd;
		return 0;
	case WM_SIZE:
		if (wParam != SIZE_MINIMIZED) {
			GetClientRect(hWnd,&crt);
			EditHeight=crt.bottom*EditRatio/100;
			MoveWindow(hList,0,0,ListWidth-GAP,crt.bottom,TRUE);
			MoveWindow(hEdit,ListWidth,0,crt.right-ListWidth,EditHeight-GAP,TRUE);
			MoveWindow(hBtn,ListWidth,EditHeight,crt.right-ListWidth,crt.bottom-EditHeight,TRUE);
		}
		return 0;
	case WM_SETCURSOR:
		if (LOWORD(lParam)==HTCLIENT) {
			GetCursorPos(&pt);
			ScreenToClient(hWnd,&pt);
			sp=GetSplitter(pt);
			if (sp==SP_VERT) {
				SetCursor(LoadCursor(NULL,IDC_SIZEWE));
				return TRUE;
			}
			if (sp==SP_HORZ) {
				SetCursor(LoadCursor(NULL,IDC_SIZENS));
				return TRUE;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		pt.x=LOWORD(lParam);
		pt.y=HIWORD(lParam);
		NowSplit=GetSplitter(pt);
		if (NowSplit != SP_NONE) {
			SetCapture(hWnd);
		}
		return 0;
	case WM_MOUSEMOVE:
		switch (NowSplit) {
		case SP_VERT:
			ListWidth=min(max((int)(short)LOWORD(lParam),50),500);
			SendMessage(hWnd,WM_SIZE,SIZE_RESTORED,0);
			break;
		case SP_HORZ:
			EditHeight=(int)(short)HIWORD(lParam);
			GetClientRect(hWnd,&crt);
			EditRatio=EditHeight*100/crt.bottom;
			EditRatio=min(max(EditRatio,20),80);
			SendMessage(hWnd,WM_SIZE,SIZE_RESTORED,0);
			break;
		}
		return 0;
	case WM_LBUTTONUP:
		NowSplit=SP_NONE;
		ReleaseCapture();
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
