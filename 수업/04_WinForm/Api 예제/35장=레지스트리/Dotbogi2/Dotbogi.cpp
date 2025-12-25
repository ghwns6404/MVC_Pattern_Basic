#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("Dotbogi");

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
	WndClass.hIcon=LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,400,300,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);

	HACCEL hAccel;

	hAccel=LoadAccelerators(hInstance,MAKEINTRESOURCE(IDR_ACCELERATOR1));
	while (GetMessage(&Message,NULL,0,0)) {
		if (!TranslateAccelerator(hWnd,hAccel,&Message)) {
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
	}
	return (int)Message.wParam;
}

#include "ShReg.h"
#define KEY "Software\\MiyoungSoft\\Dotbogi\\"

void SavePosition(HWND hWnd, TCHAR *Key);
void LoadPosition(HWND hWnd, TCHAR *Key, RECT *Def=NULL);

void SavePosition(HWND hWnd, TCHAR *Key)
{
	WINDOWPLACEMENT wndpl;

	wndpl.length=sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(hWnd,&wndpl);

	SHRegWriteInt(SHCU,Key,"showCmd", wndpl.showCmd);
	SHRegWriteInt(SHCU,Key,"left", wndpl.rcNormalPosition.left);
	SHRegWriteInt(SHCU,Key,"top", wndpl.rcNormalPosition.top);
	SHRegWriteInt(SHCU,Key,"right", wndpl.rcNormalPosition.right);
	SHRegWriteInt(SHCU,Key,"bottom",wndpl.rcNormalPosition.bottom);
}

void LoadPosition(HWND hWnd, TCHAR *Key, RECT *Def/*=NULL*/)
{
	WINDOWPLACEMENT wndpl;
	RECT drt;

	if (Def==NULL) {
		SetRect(&drt,10,10,600,400);
	} else {
		CopyRect(&drt,Def);
	}
	wndpl.length=sizeof(WINDOWPLACEMENT);
	wndpl.flags=0;
	wndpl.rcNormalPosition.left=SHRegReadInt(SHCU,Key,"left",drt.left);
	wndpl.rcNormalPosition.top=SHRegReadInt(SHCU,Key,"top",drt.top);
	wndpl.rcNormalPosition.right=SHRegReadInt(SHCU,Key,"right",drt.right);
	wndpl.rcNormalPosition.bottom=SHRegReadInt(SHCU,Key,"bottom",drt.bottom);
	wndpl.showCmd=SHRegReadInt(SHCU,Key,"showCmd",drt.bottom);

	if (wndpl.showCmd == SW_SHOWMINIMIZED) {
		wndpl.showCmd=SW_RESTORE;
	}

	wndpl.ptMinPosition.x=wndpl.ptMinPosition.y=0;
	wndpl.ptMaxPosition.x=wndpl.ptMaxPosition.y=0;
	SetWindowPlacement(hWnd,&wndpl);
}

int GetCheckedRadioButton(HWND hDlg,int First,int Last)
{
	int id;

	for (id=First;id<=Last;id++) {
		if (IsDlgButtonChecked(hDlg,id)==BST_CHECKED)
			return id;
	}
	return -1;
}

void DrawBitmap(HDC hdc,int x,int y,HBITMAP hBit)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx,by;
	BITMAP bit;

	MemDC=CreateCompatibleDC(hdc);
	OldBitmap=(HBITMAP)SelectObject(MemDC, hBit);

	GetObject(hBit,sizeof(BITMAP),&bit);
	bx=bit.bmWidth;
	by=bit.bmHeight;

	BitBlt(hdc,x,y,bx,by,MemDC,0,0,SRCCOPY);

	SelectObject(MemDC,OldBitmap);
	DeleteDC(MemDC);
}

// 전역 변수들
int ratio;
int freq;
BOOL OnlyMove;
BOOL Pause;
BOOL StopInClient;
HBITMAP hBit;

// 옵션 대화상자
BOOL CALLBACK OptionDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	static int arRatio[]={1,2,4,8,10,20};
	static int arFreq[]={1,2,4,8,10,20};
	int idx;

	switch (iMessage) {
	case WM_INITDIALOG:
		for (idx=0;idx<sizeof(arRatio)/sizeof(arRatio[0]);idx++) {
			if (arRatio[idx]==ratio) break;
		}
		CheckRadioButton(hDlg,IDC_RADIO1,IDC_RADIO6,IDC_RADIO1+idx);
		for (idx=0;idx<sizeof(arFreq)/sizeof(arFreq[0]);idx++) {
			if (arFreq[idx]==freq) break;
		}
		CheckRadioButton(hDlg,IDC_RADIO7,IDC_RADIO12,IDC_RADIO7+idx);
		CheckDlgButton(hDlg,IDC_CHKONLYMOVE,OnlyMove ? BST_CHECKED:BST_UNCHECKED);
		CheckDlgButton(hDlg,IDC_CHKSTOPINCLIENT,StopInClient ? BST_CHECKED:BST_UNCHECKED);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			idx=GetCheckedRadioButton(hDlg,IDC_RADIO1,IDC_RADIO6)-IDC_RADIO1;
			ratio=arRatio[idx];
			idx=GetCheckedRadioButton(hDlg,IDC_RADIO7,IDC_RADIO12)-IDC_RADIO7;
			freq=arFreq[idx];
			OnlyMove=(IsDlgButtonChecked(hDlg,IDC_CHKONLYMOVE)==BST_CHECKED);
			StopInClient=(IsDlgButtonChecked(hDlg,IDC_CHKSTOPINCLIENT)==BST_CHECKED);
			EndDialog(hDlg,IDOK);
			return TRUE;
		case IDCANCEL:
			EndDialog(hDlg,IDCANCEL);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc,MemDC;			// 화면 DC
	PAINTSTRUCT ps;
	HBITMAP OldBit;
	POINT pt;				// 마우스 위치
	static POINT oldpt;		// 이전 마우스 위치
	RECT drt;				// 작업 영역의 크기
	POINT spt;				// 캡처할 부분의 좌상단 좌표
	int ScrX, ScrY;			// 화면의 크기
	int sWidth, sHeight;	// 캡처할 부분의 폭과 높이
	HMENU hPopup;
	static bFirstActivate=TRUE;

	switch (iMessage) {
	// 옵션을 읽어오고 타이머를 설치한다.
	case WM_CREATE:
		hWndMain=hWnd;
		ratio=SHRegReadInt(SHCU,KEY"Option","ratio",4);
		freq=SHRegReadInt(SHCU,KEY"Option","freq",10);
		OnlyMove=SHRegReadInt(SHCU,KEY"Option","OnlyMove",TRUE);;
		StopInClient=SHRegReadInt(SHCU,KEY"Option","StopInClient",TRUE);;
		Pause=FALSE;
		SetTimer(hWnd,0,1000/freq,NULL);
		hBit=NULL;
		return 0;
	case WM_SIZE:
		if (wParam != SIZE_MINIMIZED) {
			DeleteObject(hBit);
			hBit=NULL;
		}
		return 0;
	case WM_ACTIVATEAPP:
		if (wParam == TRUE) {
			if (bFirstActivate) {
				bFirstActivate=FALSE;
				LoadPosition(hWnd,KEY"Position");
			}
		}
		return 0;
	case WM_TIMER:
		// 화면 크기를 구한다. 해상도가 바뀔 수 있으므로 매번 조사한다.
		ScrX=GetSystemMetrics(SM_CXSCREEN);
		ScrY=GetSystemMetrics(SM_CYSCREEN);
		GetClientRect(hWnd,&drt);

		// 마우스 커서가 움직이지 않았으면 다시 그리지 않는다.
		GetCursorPos(&pt);
		if (OnlyMove==TRUE && oldpt.x == pt.x && oldpt.y == pt.y) {
			return 0;
		}
		oldpt.x=pt.x;
		oldpt.y=pt.y;
		ScreenToClient(hWnd,&pt);

		// 자신의 작업영역에서는 확대하지 않는다.
		if (StopInClient && PtInRect(&drt,pt)) {
			return 0;
		}

		hdc=GetDC(hWnd);
		MemDC=CreateCompatibleDC(hdc);
		if (hBit == NULL) {
			hBit=CreateCompatibleBitmap(hdc,drt.right,drt.bottom);
		}
		OldBit=(HBITMAP)SelectObject(MemDC, hBit);
		
		// 캡처할 소스 영역의 폭과 높이를 구한다.
		sWidth=drt.right/ratio;
		sHeight=drt.bottom/ratio;

		// 캡처할 소스 영역의 좌상단 좌표를 구한다. 마우스 위치가 중앙이 
		// 되도록 하기 위해 폭,높이 절반만큼 좌상으로 이동시킨다.
		spt.x=pt.x-sWidth/2;
		spt.y=pt.y-sHeight/2;

		// 화면 영역을 벗어나지 않도록 한다.
		ClientToScreen(hWnd,&spt);
		if (spt.x < 0) spt.x=0;
		if (spt.y < 0) spt.y=0;
		if (spt.x + sWidth > ScrX) spt.x=ScrX-sWidth;
		if (spt.y + sHeight > ScrY) spt.y=ScrY-sHeight;
		ScreenToClient(hWnd,&spt);

		// 커서 위치의 화면을 작업 영역으로 확대 복사한다.
		StretchBlt(MemDC,0,0,drt.right,drt.bottom,
			hdc,spt.x,spt.y,sWidth,sHeight,SRCCOPY);
		ReleaseDC(hWnd, hdc);
		SelectObject(MemDC,OldBit);
		DeleteDC(MemDC);
		InvalidateRect(hWnd,NULL,FALSE);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDM_MENU_OPTION:
			if (DialogBox(g_hInst,MAKEINTRESOURCE(IDD_DIALOG1),
				hWnd,OptionDlgProc)==IDOK) {
				if (!Pause) {
					SetTimer(hWnd,0,1000/freq,NULL);
					SendMessage(hWnd,WM_TIMER,0,0);
				}
			}
			break;
		case IDM_MENU_PAUSE:
			Pause=!Pause;
			if (Pause) {
				KillTimer(hWnd,0);
			} else {
				SetTimer(hWnd,0,1000/freq,NULL);
			}
			break;
		case ID_MENU_EXIT:
			DestroyWindow(hWnd);
			break;
		}
		return 0;
	case WM_INITMENU:
		hPopup=GetSubMenu((HMENU)wParam,0);
		CheckMenuItem(hPopup,IDM_MENU_PAUSE,MF_BYCOMMAND | 
			Pause ? MF_CHECKED:MF_UNCHECKED);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (hBit) {
			DrawBitmap(hdc,0,0,hBit);
		}
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		SHRegWriteInt(SHCU,KEY"Option","ratio", ratio);
		SHRegWriteInt(SHCU,KEY"Option","freq", freq);
		SHRegWriteInt(SHCU,KEY"Option","OnlyMove", OnlyMove);
		SHRegWriteInt(SHCU,KEY"Option","StopInClient", StopInClient);
		SavePosition(hWnd,KEY"Position");
		DeleteObject(hBit);
		KillTimer(hWnd,0);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

