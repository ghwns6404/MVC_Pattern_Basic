#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("DDDList");

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

// DDD 번호 데이터
TCHAR *DODATA[]={"경기도","강원도","경상남도","경상북도","전라남도","전라북도","충청남도","충청북도","제주도"};
struct {
	TCHAR *name;
	int num;
} KUNDATA[9][50]={
	{"서울",2,"인천",32,"수원",331,"가평",356,"청평",356,"강화",349,"온수",349,"광주",347,"신장",347,"구리",346,},
	{"춘천",361,"강릉",391,"주문진",391,"동해",394,"북평",394,"삼척",397,"근덕",397,"상동",3736,},
	{"부산",51,"마산",551,"창원",551,"진북",551,"거제",558,"장승포",558,"고현",558,},
	{"대구",53,"경주",561,"건천",561,"안강",561,"고령",543,"구미",546,"선산",546,},
	{"광주",62,"송정",062,"강진",638,"고흥",666,"녹동",666,"곡성",688,"광양",667,},
	{"전주",652,"삼례",652,"고창",677,"군산",654,"대야",654,"옥구",654,"김제",658,"남원",671,"무주",657,"부안",683,"순창",674,"이리",653,"임실",673,"장수",656,"장계",656,"정주",681,"신태인",681,"진안",655,},
	{"대전",42,"신탄진",42,"유성",42,"공주",416,"유구",416,"금산",412,"논산",461,},
	{"청주",431,"괴산",445,"증평",445,"단양",444,"보은",433,"영동",414,"옥천",475,"음성",446,"무극",446,"제천",443,"진천",434,"충주",441,},
	{"제주전역",64}
};

// 리스트 박스의 ID와 윈도우 핸들
#define ID_LISTDO 100
#define ID_LISTKUN 101
HWND hListDo, hListKun;
// 조사한 DDD번호 문자열
TCHAR str[128];
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	int i, index,num;
	RECT crt;
	PAINTSTRUCT ps;
	HDC hdc;
	switch (iMessage) {
	case WM_CREATE:
		// 두 개의 리스트 박스를 만든다.
		hListDo=CreateWindow(TEXT("listbox"),NULL,WS_CHILD | WS_VISIBLE | WS_BORDER |
			WS_VSCROLL | LBS_NOTIFY,
			0,0,0,0,hWnd,(HMENU)ID_LISTDO,g_hInst,NULL);
		hListKun=CreateWindow(TEXT("listbox"),NULL,WS_CHILD | WS_VISIBLE | WS_BORDER |
			WS_VSCROLL | LBS_NOTIFY,
			0,0,0,0,hWnd,(HMENU)ID_LISTKUN,g_hInst,NULL);
		// 도 리스트 박스에 데이터를 채운다.
		for (i=0;i<9;i++) {
			SendMessage(hListDo, LB_ADDSTRING, 0, (LPARAM)DODATA[i]);
		}
		return 0;
	case WM_SIZE:
		// 메인 윈도우의 크기가 변경되면 리스트 박스의 크기도 같이 변경한다.
		GetClientRect(hWnd, &crt);
		MoveWindow(hListDo, 10,10,200,crt.bottom-50, TRUE);
		MoveWindow(hListKun, 220,10,200,crt.bottom-50, TRUE);
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		// 도 리스트의 항목이 변경된 경우
		case ID_LISTDO:
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				// 군 리스트 박스를 비운 후 다시 채운다.
				SendMessage(hListKun, LB_RESETCONTENT,0,0);
				index=SendMessage(hListDo, LB_GETCURSEL,0,0);
				for (i=0;i<50;i++) {
					if (KUNDATA[index][i].num!=0) {
						SendMessage(hListKun, LB_ADDSTRING, 0, 
							(LPARAM)KUNDATA[index][i].name);
						SendMessage(hListKun, LB_SETITEMDATA, i, 
							(LPARAM)KUNDATA[index][i].num);
					}
				}
				break;
			}
			break;
		// 군 리스트의 항목이 변경된 경우 DDD번호를 조사하여 화면으로 출력한다.
		case ID_LISTKUN:
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE:
				index=SendMessage(hListKun, LB_GETCURSEL,0,0);
				num=SendMessage(hListKun, LB_GETITEMDATA, index, 0);
				wsprintf(str, TEXT("선택한 지역의 DDD 번호는 0%d번입니다"),num);
				InvalidateRect(hWnd, NULL, TRUE);
				break;
			}
			break;
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &crt);
		TextOut(hdc,10,crt.bottom-30,str,lstrlen(str));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
