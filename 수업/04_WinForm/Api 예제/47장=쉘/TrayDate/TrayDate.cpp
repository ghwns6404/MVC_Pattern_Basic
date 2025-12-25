#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("TrayDate");

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;
	HANDLE hMutex;

	hMutex=CreateMutex(NULL,FALSE,"MutexOfTrayDate");
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		if (MessageBox(NULL, "트레이 달력이 이미 실행중입니다."
			" 이 프로그램은 두 번 실행할 필요가 전혀 없는 프로그램입니다."
			"\r\n그러나 꼭 원하신다면 그렇게 하십시요. 계속 하시겠습니까?",
			"알림",MB_YESNO)==IDNO)
			return 0;
	}

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
	ShowWindow(hWnd,SW_HIDE);

	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	CloseHandle(hMutex);
	return (int)Message.wParam;
}

#include "SHReg.h"
#define TRAY_NOTIFY (WM_APP + 100)
#define KEY "Software\\MiyoungSoft\\TrayDate\\"

// 전역 변수
HICON DayIcon[31];
HICON WeekIcon[7];
int gDay=32;		// 현재 날짜
int gWeek=8;		// 현재 요일
int Display;		// 옵션. 1=날짜,요일  2=날짜만  3=요일만
int CheckDate;		// 날짜 변경 체크. 1=체크, 0=체크안함

// 두 아이콘을 모두 제거한다.
void DeleteIcon()
{
	NOTIFYICONDATA nid;

	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hWndMain;
	nid.uID = 0;
	Shell_NotifyIcon(NIM_DELETE, &nid);
	nid.uID = 1;
	Shell_NotifyIcon(NIM_DELETE, &nid);
}

// Display값에 따라 아이콘을 등록한다.
void AddIcon()
{
	NOTIFYICONDATA nid;
	SYSTEMTIME st;
	TCHAR ToolTip[64];
	TCHAR Yoil[7][3]={"일","월","화","수","목","금","토"};

	// 오늘 날짜를 조사하여 툴팁을 작성한다.
	GetLocalTime(&st);
	gDay=st.wDay;
	gWeek=st.wDayOfWeek;
	wsprintf(ToolTip,"트레이 달력:%d월 %d일 %s요일",
		st.wMonth, st.wDay, Yoil[st.wDayOfWeek]);

	// 등록된 아이콘을 일단 삭제한다.
	DeleteIcon();
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = hWndMain;

	// 날짜 아이콘을 등록한다.
	if ((Display == 1) || (Display == 2)) {
		nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
		nid.uCallbackMessage = TRAY_NOTIFY;
		nid.uID = 0;
		nid.hIcon = DayIcon[gDay-1];
		lstrcpy(nid.szTip, ToolTip);
		Shell_NotifyIcon(NIM_ADD, &nid);
	}

	// 요일 아이콘을 등록한다.
	if ((Display == 1) || (Display == 3)) {
		nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
		nid.uCallbackMessage = TRAY_NOTIFY;
		nid.uID = 1;
		nid.hIcon = WeekIcon[gWeek];
		lstrcpy(nid.szTip, ToolTip);
		Shell_NotifyIcon(NIM_ADD, &nid);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	int i;
	HMENU hMenu, hPopupMenu;
	POINT pt;
	SYSTEMTIME st;

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		// 아이콘들을 모두 읽어들인다.
		for (i=0;i<31;i++)
			DayIcon[i]=LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_DAY01+i));
		for (i=0;i<7;i++)
			WeekIcon[i]=LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_WEEK1+i));
		// 옵션에 따라 아이콘을 등록한다.
		Display=SHRegReadInt(HKEY_CURRENT_USER,KEY"Option","Display",1);
		CheckDate=SHRegReadInt(HKEY_CURRENT_USER,KEY"Option","CheckDate",0);
		AddIcon();
		if (CheckDate == 1)
			SetTimer(hWnd,0,60000,NULL);
		return 0;
	// 1분당 한번씩 날짜가 변경되었는지를 점검해 본다.
	case WM_TIMER:
	case WM_TIMECHANGE:
		GetLocalTime(&st);
		if (st.wDay != gDay) {
			AddIcon();
		}
		return 0;
	case TRAY_NOTIFY:
		switch (lParam) {
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
			hMenu=LoadMenu(g_hInst, MAKEINTRESOURCE(IDR_MENU1));
			hPopupMenu=GetSubMenu(hMenu,0);
			CheckMenuItem(hPopupMenu,IDM_OPDAYWEEK+Display-1,MF_BYCOMMAND|MF_CHECKED);
			if (CheckDate == 1)
				CheckMenuItem(hPopupMenu,IDM_CHECKDATE,MF_BYCOMMAND|MF_CHECKED);
			GetCursorPos(&pt);
			SetForegroundWindow(hWnd);
			TrackPopupMenu(hPopupMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, 
				pt.x, pt.y, 0, hWnd, NULL);
			SetForegroundWindow(hWnd);
			DestroyMenu(hPopupMenu);
			DestroyMenu(hMenu);
			break;
		}
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_ABOUT:
			MessageBox(hWnd,"날짜와 요일을 트레이에 아이콘 형태로 보여주는"
				" 프로그램입니다\r\n완전한 공개 프로그램이므로 누구나 복사"
				"해서 사용할 수 있습니다\r\n\r\nhttp://www.winapi.co.kr",
				"프로그램 소개",MB_OK);
			break;
		case IDM_OPDAYWEEK:
		case IDM_OPDAY:
		case IDM_OPWEEK:
			if (Display == LOWORD(wParam)-IDM_OPDAYWEEK+1)
				break;
			Display=LOWORD(wParam)-IDM_OPDAYWEEK+1;
			AddIcon();
			SHRegWriteInt(HKEY_CURRENT_USER,KEY"Option","Display",Display);
			break;
		case IDM_CHECKDATE:
			MessageBox(hWnd,"'날짜 변경 체크' 옵션을 선택하시면 1분에 한번씩 날짜를"
				" 체크하여 자정에 날짜를 갱신하므로.\r\n항상 정확한 날짜를 유지하실"
				" 수 있으나 매분마다 호출되므로 시스템 속도가 조금 느려질 수도 있습"
				"니다\r\n컴퓨터를 주간에만 사용하신다면 이 옵션은 선택하지 않는 것이"
				" 좋습니다.","날짜 변경 체크",MB_OK);
			if (CheckDate == 1) {
				CheckDate = 0;
				KillTimer(hWnd,0);
			} else {
				CheckDate = 1;
				SetTimer(hWnd,0,60000,NULL);
			}
			SHRegWriteInt(HKEY_CURRENT_USER,KEY"Option","CheckDate",CheckDate);
			break;
		}
		return 0;
	case WM_DESTROY:
		if (CheckDate == 1)
			KillTimer(hWnd,0);
		DeleteIcon();
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

