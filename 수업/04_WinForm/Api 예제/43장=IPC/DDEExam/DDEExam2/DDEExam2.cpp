#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("DDEExam2");

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
		100,300,500,150,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);

	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

HWND hServer=NULL;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	ATOM aApp, aTopic, aItem;
	TCHAR szApp[255], szTopic[255],szItem[255];
	static TCHAR Mes[255]="마우스 오른쪽 버튼을 눌러 서버와 대화를 시작하십시요";
	HANDLE hData;
	DDEDATA *lpData;
	BOOL bRelease;
	LPARAM DDElParam;

	switch (iMessage) {
	// 프로그램 시작 직후 대화 시작을 서버에게 요청한다. 서버가 실행중이 아닌
	// 경우 대화는 성립되지 않으며 이 경우 클라이언트는 종료된다.
	case WM_CREATE:
		aApp=GlobalAddAtom("Server123");
		aTopic=GlobalAddAtom("Topic123");
		SendMessage((HWND)-1,WM_DDE_INITIATE,(WPARAM)hWnd,MAKELONG(aApp,aTopic));
		GlobalDeleteAtom(aApp);
		GlobalDeleteAtom(aTopic);
		if (hServer == NULL) {
			MessageBox(hWnd,"서버를 먼저 실행시킨 후 클라이언트를 실행하십시요",
				"알림",MB_OK);
			DestroyWindow(hWnd);
		}
		return 0;
	// 서버의 대화 요청 허가를 받아 들이고 이 후 대화를 위해 서버의 윈도우
	// 핸들을 전역 변수에 저장해 놓는다.
	case WM_DDE_ACK:
		GlobalGetAtomName(LOWORD(lParam),szApp,255);
		GlobalGetAtomName(HIWORD(lParam),szTopic,255);
		if ((lstrcmp(szApp,"Server123")==0) && (lstrcmp(szTopic,"Topic123")==0)) {
			hServer=(HWND)wParam;
			wsprintf(Mes, "%d 핸들의 서버와 대화를 시작하였습니다. "
				"마우스 버튼을 누르면 데이터를 요청합니다.",hServer);
			InvalidateRect(hWnd,NULL,TRUE);
		}
		return 0;
	// 마우스 왼쪽 버튼을 누르면 서버에게 데이터를 요청한다.
	case WM_LBUTTONDOWN:
		aItem=GlobalAddAtom("Item123");
		DDElParam=PackDDElParam(WM_DDE_REQUEST,CF_TEXT,aItem);
		if (!PostMessage(hServer,WM_DDE_REQUEST,(WPARAM)hWnd,DDElParam)) {
			GlobalDeleteAtom(aItem);
			FreeDDElParam(WM_DDE_REQUEST,DDElParam);
			MessageBox(hWnd,"서버가 응답하지 않습니다","알림",MB_OK);
		}
		return 0;
	// 마우스 오른쪽 버튼을 누르면 또 다른 요청을 보낸다.
	case WM_RBUTTONDOWN:
		aItem=GlobalAddAtom("Item456");
		DDElParam=PackDDElParam(WM_DDE_REQUEST,CF_TEXT,aItem);
		if (!PostMessage(hServer,WM_DDE_REQUEST,(WPARAM)hWnd,DDElParam)) {
			GlobalDeleteAtom(aItem);
			FreeDDElParam(WM_DDE_REQUEST,DDElParam);
			MessageBox(hWnd,"서버가 응답하지 않습니다","알림",MB_OK);
		}
		return 0;
	case WM_DDE_DATA:
		// 요청한 아이템인지 검사해 본다.
		UnpackDDElParam(WM_DDE_DATA,lParam,(PUINT)&hData,(PUINT)&aItem);
		GlobalGetAtomName(aItem,szItem,255);
		if ((lstrcmp(szItem,"Item123")!=0) && (lstrcmp(szItem,"Item456")!=0)) {
			break;
		}

		FreeDDElParam(WM_DDE_DATA,lParam);
		lpData=(DDEDATA *)GlobalLock(hData);

		// CF_TEXT가 아니면 데이터 거절
		if (lpData->cfFormat != CF_TEXT) {
			PostMessage(hServer,WM_DDE_ACK,(WPARAM)hWnd,
				PackDDElParam(WM_DDE_ACK,0,aItem));
			return 0;
		}
		
		// 데이터를 복사한다.
		wsprintf(Mes,"'%s'문자열을 받았습니다.",(LPSTR)lpData->Value);
		InvalidateRect(hWnd,NULL,TRUE);

		// 데이터를 받았음을 알려준다.
		if (lpData->fAckReq) {
			PostMessage(hServer,WM_DDE_ACK,(WPARAM)hWnd,
				PackDDElParam(WM_DDE_ACK,0x8000,aItem));
		}

		// 메모리 해제 요청이 있는 경우 메모리를 해제 한다.
		bRelease=lpData->fRelease;
		GlobalUnlock(hData);
		if (bRelease)
			GlobalFree(hData);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,50,50,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostMessage(hServer,WM_DDE_TERMINATE,
			PackDDElParam(WM_DDE_TERMINATE,(UINT)hWnd,0),0);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

