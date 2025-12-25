#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("DDEExam1");

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
		100,100,500,150,
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
	PAINTSTRUCT ps;
	ATOM aApp, aTopic, aItem;
	TCHAR szApp[255], szTopic[255], szItem[255];
	HANDLE hData;
	DDEDATA *lpData;
	LPARAM DDElParam;
	TCHAR szItem123[]="아이템123에 대한 응답 문자열입니다.";
	TCHAR szItem456[]="Item456에 대한 답변 데이터입니다.";
	TCHAR *WhichItem=NULL;
	int Format;
	static TCHAR Mes[255]="클라이언트에서 대화를 먼저 요청하십시요";

	switch (iMessage) {
	// 클라이언트로부터의 대화 요청을 받아들이고 요청이 받아들여졌음을 알린다.
	// 와일드 카드 아톰이 전달된 경우는 이보다 더 복잡한 처리가 필요하다.
	case WM_DDE_INITIATE:
		GlobalGetAtomName(LOWORD(lParam),szApp,255);
		GlobalGetAtomName(HIWORD(lParam),szTopic,255);
		if ((lstrcmp(szApp,"Server123")==0) && (lstrcmp(szTopic,"Topic123")==0)) {
			aApp=GlobalAddAtom("Server123");
			aTopic=GlobalAddAtom("Topic123");
			SendMessage((HWND)wParam, WM_DDE_ACK,(WPARAM)hWnd,
				MAKELPARAM(aApp, aTopic));
			GlobalDeleteAtom(aApp);
			GlobalDeleteAtom(aTopic);
			wsprintf(Mes,"%d 클라이언트와 대화를 시작합니다",wParam);
			InvalidateRect(hWnd,NULL,TRUE);
		}
		return 0;
	// 클라이언트의 데이터 요청을 처리한다.
	case WM_DDE_REQUEST:
		UnpackDDElParam(WM_DDE_REQUEST,lParam,(PUINT)&Format,(PUINT)&aItem);
		FreeDDElParam(WM_DDE_REQUEST,lParam);
		GlobalGetAtomName(aItem,szItem,255);
		GlobalDeleteAtom(aItem);
		if (lstrcmp(szItem,"Item123")==0) {
			WhichItem=szItem123;
		}
		if (lstrcmp(szItem,"Item456")==0) {
			WhichItem=szItem456;
		}

		if (WhichItem != NULL) {
			// 보낼 데이터를 조립한다.
			hData=GlobalAlloc(GMEM_MOVEABLE, (LONG)sizeof(DDEDATA) + 
				lstrlen(WhichItem)+2);
			lpData=(DDEDATA *)GlobalLock(hData);
			lpData->cfFormat=CF_TEXT;
			lpData->fAckReq=TRUE;
			lpData->fRelease=TRUE;
			lpData->fResponse=TRUE;
			lstrcpy((LPSTR)lpData->Value,WhichItem);
			GlobalUnlock(hData);

			// 데이터를 클라이언트에게 보낸다.
			aItem=GlobalAddAtom(szItem);
			DDElParam=PackDDElParam(WM_DDE_DATA,(UINT)hData, aItem);
			if (!PostMessage((HWND)wParam,WM_DDE_DATA,(WPARAM)hWnd,DDElParam)) {
				GlobalFree(hData);
				GlobalDeleteAtom(aItem);
				FreeDDElParam(WM_DDE_DATA,DDElParam);
			}
		} else {
			// 지원하는 토픽이 아닌 경우의 처리
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,50,50,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

