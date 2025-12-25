#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("RegEnum");

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

// 함수 원형
void RegEnum(HKEY);
void DblClk(void);
void SelChange(void);

// 차일드 컨트롤들
#define ID_LISTBOX 100
#define ID_EDIT 101
#define ID_BUTTON 102
HWND hList;
HWND hEdit;
HWND hButton;
HWND hStatic;

// 윈도우 프로시저
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HKEY key;
	TCHAR str[MAX_PATH];
	switch (iMessage) {
	// 차일드 윈도우를 만든다.
	case WM_CREATE:
		hWndMain=hWnd;
		hList=CreateWindow("listbox",NULL,WS_CHILD | WS_VISIBLE | WS_BORDER |
		LBS_NOTIFY | WS_VSCROLL,
		10,50,200,400,hWnd,(HMENU)ID_LISTBOX,g_hInst,NULL);

		hEdit=CreateWindow("edit","software",WS_CHILD | WS_VISIBLE | WS_BORDER | 
		ES_AUTOHSCROLL,10,10,400,25,hWnd,(HMENU)ID_EDIT,g_hInst,NULL);

		hButton=CreateWindow("button","Enumerate",WS_CHILD | WS_VISIBLE | 
			BS_PUSHBUTTON,420,10,100,25,hWnd,(HMENU)ID_BUTTON,g_hInst,NULL);

		hStatic=CreateWindow("static","표시할 내용이 없습니다.",
			WS_CHILD | WS_VISIBLE,
			,0,0,0,0,hWnd,(HMENU)-1,g_hInst,NULL);

		// 프로그램 시작 직후에 software 아래의 키를 조사한다.
		SendMessage(hWnd,WM_COMMAND,MAKEWPARAM(ID_BUTTON,BN_CLICKED),
			(LPARAM)hButton);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		// 버튼 누름:에디트의 키값으로 순회한다.
		case ID_BUTTON:
			GetWindowText(hEdit,str,MAX_PATH);
			if (RegOpenKeyEx(HKEY_CURRENT_USER, 
				str,0,KEY_ALL_ACCESS,&key) == ERROR_SUCCESS) {
				RegEnum(key);
				RegCloseKey(key);
			} else {
				MessageBox(hWnd,"지정한 키를 열 수 없습니다","에러",MB_OK);
			}
			break;
		// 리스트 박스의 클릭과 더블 클릭 처리
		case ID_LISTBOX:
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK:
				DblClk();
				break;
			case LBN_SELCHANGE:
				SelChange();
				break;
			}
			break;
		}
		return 0;
	// 메인 윈도우의 크기가 변경되면 새끼들의 크기,위치도 같이 변경한다.
	case WM_SIZE:
		MoveWindow(hEdit,10,10,LOWORD(lParam)-130,25,TRUE);
		MoveWindow(hButton,LOWORD(lParam)-110,10,100,25,TRUE);
		MoveWindow(hList,10,50,LOWORD(lParam)-20,HIWORD(lParam)-80,TRUE);
		MoveWindow(hStatic,10,HIWORD(lParam)-30,LOWORD(lParam)-20,25,TRUE);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

// key아래의 모든 서브 키와 값을 조사한다.
void RegEnum(HKEY key)
{
	TCHAR lpSubKey[MAX_PATH];
	TCHAR lpValue[MAX_PATH];
	TCHAR lpBuffer[MAX_PATH];
	DWORD i;
	LONG Result;
	DWORD Size;
	FILETIME FileTime;

	SendMessage(hList,LB_RESETCONTENT,0,0);
	SendMessage(hList,LB_ADDSTRING,0,(LPARAM)" ^^^ 한 단계 위로 ^^^ ");

	// 서브키의 목록의 조사해 리스트 박스에 채워 넣는다.
	Result=ERROR_SUCCESS;
	for (i=0;Result==ERROR_SUCCESS;i++) {
		Size=MAX_PATH;
		Result=RegEnumKeyEx(key,i,lpSubKey,&Size,NULL,NULL,NULL,&FileTime);
		if (Result==ERROR_SUCCESS) {
			wsprintf(lpBuffer,"K : %s",lpSubKey);
			SendMessage(hList,LB_ADDSTRING,0,(LONG)lpBuffer);
		}
	}

	// 값의 목록을 조사해 리스트 박스에 채워 넣는다.
	Result=ERROR_SUCCESS;
	for (i=0;Result==ERROR_SUCCESS;i++) {
		Size=MAX_PATH;
		Result=RegEnumValue(key,i,lpValue,&Size,NULL,NULL,NULL,NULL);
		if (Result==ERROR_SUCCESS) {
			wsprintf(lpBuffer,"V : %s",lpValue);
			SendMessage(hList,LB_ADDSTRING,0,(LONG)lpBuffer);
		}
	}
}

// 리스트 박스의 더블클릭 처리
void DblClk(void)
{
	int index;
	TCHAR buffer[MAX_PATH];
	TCHAR Caption[MAX_PATH];
	TCHAR Post[MAX_PATH];
	TCHAR *backslash;
	index=SendMessage(hList,LB_GETCURSEL,0,0);

	// 한단계 위로 처리
	if (index==0) {
		GetWindowText(hEdit,Caption,MAX_PATH);
		backslash=strrchr(Caption, '\\');
		if (backslash==NULL) {
			MessageBox(hWndMain,"더 올라갈 곳이 없습니다요","잠깐!",MB_OK);
			return;
		}
		backslash[0]=0;
		SetWindowText(hEdit,Caption);
		SendMessage(hWndMain,WM_COMMAND,MAKEWPARAM(ID_BUTTON,BN_CLICKED),
			(LPARAM)hButton);
		return;
	}

	// 키를 더클릭했을 경우 해당 키를 조사한다.
	SendMessage(hList,LB_GETTEXT,index,(LPARAM)buffer);
	lstrcpy(Post,buffer+4);
	if (buffer[0]=='K') {
		GetWindowText(hEdit,Caption,MAX_PATH);
		wsprintf(buffer,"%s\\%s",Caption,Post);
		SetWindowText(hEdit,buffer);
		SendMessage(hWndMain,WM_COMMAND,MAKEWPARAM(ID_BUTTON,BN_CLICKED),
			(LPARAM)hButton);
	}
}

// 리스트 박스의 항목이 빠뀌었을 때의 처리
void SelChange(void)
{
	int index;
	TCHAR buffer[MAX_PATH];
	TCHAR Caption[MAX_PATH];
	TCHAR Post[MAX_PATH];
	HKEY key;
	DWORD cSubKeys, cValues;
	DWORD Type, Size;
	BYTE Data[MAX_PATH];

	index=SendMessage(hList,LB_GETCURSEL,0,0);
	// 한단계 위로에 대해서는 전혀 반응하지 않는다.
	if (index==0)
		return;
	SendMessage(hList,LB_GETTEXT,index,(LPARAM)buffer);
	lstrcpy(Post,buffer+4);

	// 키에 속한 서브키와 값의 개수 조사
	if (buffer[0]=='K') {
		GetWindowText(hEdit,Caption,MAX_PATH);
		wsprintf(buffer,"%s\\%s",Caption,Post);
		if (RegOpenKeyEx(HKEY_CURRENT_USER, 
			buffer,0,KEY_ALL_ACCESS,&key) == ERROR_SUCCESS) {
			RegQueryInfoKey(key,NULL,NULL,NULL,&cSubKeys,NULL,NULL,
				&cValues,NULL,NULL,NULL,NULL);
			RegCloseKey(key);
			wsprintf(Caption,"서브키의 수:%d, 값의 수:%d",cSubKeys,cValues);
			SetWindowText(hStatic,Caption);
		}
	}

	// 값의 데이터 타입과 데이터 조사
	else if (buffer[0]=='V') {
		GetWindowText(hEdit,Caption,MAX_PATH);
		if (RegOpenKeyEx(HKEY_CURRENT_USER, 
			Caption,0,KEY_ALL_ACCESS,&key) == ERROR_SUCCESS) {
			Size=MAX_PATH;
			RegQueryValueEx(key,Post,NULL,&Type,Data,&Size);
			RegCloseKey(key);
			switch (Type) {
			case REG_DWORD:
				wsprintf(Caption,"데이터 타입:정수형, 데이터:%d",*(LPDWORD)Data);
				break;
			case REG_SZ:
				wsprintf(Caption,"데이터 타입:문자열, 데이터:%s",Data);
				break;
			default:
				wsprintf(Caption,"데이터 타입:%d, 데이터:%s",Type,Data);
				break;
			}
			SetWindowText(hStatic,Caption);
		}
	}
}


