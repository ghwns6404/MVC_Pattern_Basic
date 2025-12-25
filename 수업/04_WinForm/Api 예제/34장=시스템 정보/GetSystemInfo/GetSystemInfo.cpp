#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("GetSystemInfo");

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

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	SYSTEM_INFO si;
	TCHAR str[128];
	TCHAR Arch[100], Level[100];
	TCHAR KeyType[100];
	static int arFunc[]={0,10,12,10,12,10,24,7,8,9,10,11,12};

	switch (iMessage) {
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		GetSystemInfo(&si);
		wsprintf(str, "프로세서의 개수는 %d개 입니다", si.dwNumberOfProcessors);
		TextOut(hdc,10,10,str,lstrlen(str));
		switch (si.wProcessorArchitecture) {
			case PROCESSOR_ARCHITECTURE_INTEL:lstrcpy(Arch, "Intel");break;
			case PROCESSOR_ARCHITECTURE_MIPS:lstrcpy(Arch, "MIPS");break;
			case PROCESSOR_ARCHITECTURE_ALPHA:lstrcpy(Arch, "Alpha");break;
			case PROCESSOR_ARCHITECTURE_PPC:lstrcpy(Arch, "Power PC");break;
			case PROCESSOR_ARCHITECTURE_UNKNOWN:lstrcpy(Arch, "알수없음");break;
		}
		switch (si.wProcessorLevel) {
			case 3:lstrcpy(Level,"80386");break;
			case 4:lstrcpy(Level,"80486");break;
			case 5:lstrcpy(Level,"Pentium");break;
			case 6:lstrcpy(Level,"Pentium Pro or Pentium II");break;
			default:lstrcpy(Level,"기타");break;
		}
		wsprintf(str, "CPU 종류는 %s이며 레벨은 %s, 리비전은 %x입니다",
			Arch, Level, si.wProcessorRevision);
		TextOut(hdc,10,30,str,lstrlen(str));

		switch (GetKeyboardType(0)) {
			case 1:lstrcpy(KeyType, "IBM PC/XT or compatible(83 key)");break;
			case 2:lstrcpy(KeyType, "Olivetti ICO(102 key)");break;
			case 3:lstrcpy(KeyType, "IBM PC/AT (84 key)");break;
			case 4:lstrcpy(KeyType, "IBM enhanced(101 or 102 key)");break;
			case 5:lstrcpy(KeyType, "Nokia 1050");break;
			case 6:lstrcpy(KeyType, "Nokia 9140");break;
			case 7:lstrcpy(KeyType, "일본 키보드");break;
			default:lstrcpy(KeyType, "기타 키보드");break;
		}
		wsprintf(str,"키보드 타입은 %s이며 서브 타입은 %d, 펑션키는 %d개입니다",
			KeyType, GetKeyboardType(1), arFunc[GetKeyboardType(2)]);
		TextOut(hdc,10,50,str,lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

