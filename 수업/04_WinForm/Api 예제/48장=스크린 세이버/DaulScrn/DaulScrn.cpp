// 다울 스크린 세이버 1.0
// Programmer:김상형(mituri@daulsoft.com)
// Date:1999.12.20
#include <windows.h>
#include <scrnsave.h>
#include <time.h>
#include <mmsystem.h>
#include "resource.h"
#include "SHReg.h"

#define TOTALBIT 10							// 총 사진 개수
#define KEY "Software\\KBSTeps\\ScrnSave"	// 루트 레지스트리 키
#define random(n) (rand()%(n))				// 난수 발생 함수

// 전역 변수들
int SX, SY;					// 화면의 크기
HBITMAP hBanner[5];			// 배너 배트맵 배열
HBITMAP hBit[TOTALBIT];		// 비트맵 배열
TCHAR *Name[TOTALBIT]={"이승우","김상형","김지연","김태경","박대희","조기봉","손정욱","양철승","오광섭","최봉우"};
BOOL Sel[TOTALBIT];			// n번 사진의 출력 여부

int g_Banner;				// 배너 카운터
int NumPic = 0;				// 현재까지 출력된 그림 개수
int nSleep;					// 속도에 따른 지연값
int o_Stretch;				// 그림 확대 비율. 0=원래대로, 15=1.5배, 20=2배, 30=3배
int o_Sound;				// 사운드 연주 기능. 1=항상, 0=조용히, 5=5번에 한번, 10=10번에 한번
int o_Reset;				// 화면 리셋 0=안함. 10=10개 출력후, 100=100개 출력 후, 200=심심할 때마다.
int o_Speed;				// 출력 속도 1=느리게, 2=보통, 3=빠르게,4=엄청 빠르게
int o_PrintName;			// 이름 보여주기 1=보여줌, 0=안보여줌
int o_IgnoreMove;			// 마우스 이동 무시
int o_IgnoreClick;			// 마우스 클릭 무시

// 함수 원형
void OnCreate(HWND hWnd);
VOID CALLBACK BannerTimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime);

// 비트맵을 출력한다. StretchBlt 함수와 거의 유사함
void DrawBitmap(HDC hdc, int x1, int y1, int w1,int h1,int x2, int y2, int w2, int h2, HBITMAP bit)
{
	HDC MemDC;
	HBITMAP OldBitmap;

	MemDC=CreateCompatibleDC(hdc);
	OldBitmap=(HBITMAP)SelectObject(MemDC, bit);
	StretchBlt(hdc,x1,y1,w1,h1,MemDC,x2,y2,w2,h2,SRCCOPY);
	SelectObject(MemDC,OldBitmap);
	DeleteDC(MemDC);
} 

// n번 사진을 화면의 x,y좌표에 출력한다. 출력 효과는 난수로 선택하며 옵션들이 적용된다.
void PopupBitmap(HDC hdc, int x, int y, int n)
{
	int bw,bh;				// 사진의 크기
	BITMAP bitmap;
	int i,delta, delta2;
	double e=1;				// 확대 배율
	int Ani = 1;			// 적용 출력 효과
	HBITMAP bit = hBit[n];	// 출력 대상 비트맵

	// 일정 개수 출력 후 화면을 지운다. 단 o_Reset이 500이고 20개 이상 출력되었을 때 1/100의 확률로 리셋한다.
	NumPic ++;
	if (o_Reset != 0) {
		if ((NumPic == o_Reset) || ((o_Reset == 500) && (NumPic > 20) && (random(100) == 0))) {
			InvalidateRect(hMainWindow, NULL, TRUE);
			NumPic = 0;
			return;
		}
	}

	// 확대 배율 지정 1.0~3.0까지 가능하다.
	// 메모:o_Stream값을 10으로 나누면 최대 배열이 된다. 예를 들어 15는 1.5배, 20은 2배이다.
	//      마지막 끝 배율도 포함하기 위해 빼기는 9를 빼고 더하기는 10을 더했다. 
	//      이렇게 하지 않으면 20의 경우 1~1.9배까지만 선택된다.
	if (o_Stretch != 0) {
		e = random(o_Stretch-9) + 10;
		e = e / 10;
	}

	// 사운드 연주
	if (o_Sound != 0) {
		if ((o_Sound == 1) || ((o_Sound == 5) && (NumPic % 5 == 0)) || ((o_Sound == 10) && (NumPic % 10 == 0))) {
			Ani = random(6);
			PlaySound(MAKEINTRESOURCE(IDR_WAVE1 + Ani), hMainInstance, SND_RESOURCE | SND_ASYNC);
		}
	}

	// 비트맵의 크기를 구해 놓는다. 현재는 80*60으로 고정되어 있다.
	GetObject(bit,sizeof(bitmap),&bitmap);
 	bw = bitmap.bmWidth;
	bh = bitmap.bmHeight;

	Ani = random(6)+1;
	switch (Ani) {
	case 1:		// 아래에서
		delta = bh / 8;
		for (i=1;i<=8;i++) {
			DrawBitmap(hdc, x, y+(bh - i*delta)*e,bw*e,i*delta*e,0,0,bw,i*delta,bit);
			Sleep(nSleep);
		}
		break;
	case 2:		// 위에서
		delta = bh / 8;
		for (i=1;i<=8;i++) {
			DrawBitmap(hdc, x, y,bw*e,i*delta*e,0,bh-i*delta,bw,i*delta,bit);
			Sleep(nSleep);
		}
		break;
	case 3:		// 왼쪽에서
		delta = bw / 6;
		for (i=1;i<=6;i++) {
			DrawBitmap(hdc, x, y,(i*delta)*e,bh*e,bw-i*delta,0,i*delta,bh,bit);
			Sleep(nSleep);
		}
		break;
	case 4:		// 오른쪽에서
		delta = bw / 6;
		for (i=1;i<=6;i++) {
			DrawBitmap(hdc, x+(bw-(i*delta))*e, y,(i*delta)*e,bh*e,0,0,i*delta,bh,bit);
			Sleep(nSleep);
		}
		break;
	case 5:		// 안쪽에서 확대
		delta = bw / 10;
		delta2 = bh / 10;
		for (i=1;i<=5;i++) {
			DrawBitmap(hdc,x+bw*e/2-delta*i*e,y+bh*e/2-delta2*i*e,delta*i*e*2,delta2*i*e*2,0,0,bw,bh,bit);
			Sleep(nSleep);
		}
		break;
	case 6:		// 안쪽에서 열리기
		delta = bw / 10;
		delta2 = bh / 10;
		for (i=1;i<=5;i++) {
			DrawBitmap(hdc,x+bw*e/2-delta*i*e,y+bh*e/2-delta2*i*e,delta*i*e*2,delta2*i*e*2,bw/2-delta*i,bh/2-delta2*i,delta*i*2,delta2*i*2,bit);
			Sleep(nSleep);
		}
		break;
	}

	// 이름 출력하기-사진 좌하단에 이름을 출력하되 색상은 약간 밝은 색으로 선택하였다.
	if (o_PrintName == 1) {
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(random(155)+100, random(155)+100, random(155)+100));
		SetTextAlign(hdc, TA_LEFT | TA_TOP);
		TextOut(hdc, x, y + bh*e, Name[n], lstrlen(Name[n]));
	}
}

// 메인 메시지 프로시저
LRESULT WINAPI ScreenSaverProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes = "다울소프트 스크린 세이버 - Version 1.0  http://www.daulsoft.com";
	int i;
	TCHAR Value[128];

	switch (iMessage) {
	case WM_CREATE:
		OnCreate(hWnd);
		return 0;
	// 정해진 시간 간격으로 사진을 화면으로 출력한다.
	case WM_TIMER:
		hdc = GetDC(hWnd);
		switch (wParam) {
			case 0:				// 사진 출력. 출력이 허가된 사진만 출력한다.
				do {
					i = random(TOTALBIT);
				} while (Sel[i] == 0);
				PopupBitmap(hdc, random(SX-60), random(SY-100)+20,i);
				break;
		}
		ReleaseDC(hWnd, hdc);
		return 0;
	// 비트맵과 타이머를 해제한다.
	case WM_DESTROY:
		for (i=0;i<TOTALBIT;i++)
			DeleteObject(hBit[i]);
		for (i=0;i<5;i++)
			DeleteObject(hBanner[i]);
		KillTimer(hWnd,0);
		KillTimer(hWnd,1);
		return 0;
	// 스크린 세이버 제목을 화면 상단에 출력한다.
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(255,255,255));
		SetTextAlign(hdc, TA_CENTER);
		TextOut(hdc, SX / 2, 5, Mes, lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_MOUSEMOVE:
		if (o_IgnoreMove == 1)
			return 0;
		break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		if (o_IgnoreClick == 1)
			return 0;
		break;
	}
	return DefScreenSaverProc(hWnd, iMessage, wParam, lParam);
}

// 스크린 세이버 설정 대화상자 프로시저
BOOL WINAPI ScreenSaverConfigureDialog(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	int i;
	TCHAR Value[256];
	int iData;
	int idx, count;

	switch (iMessage) {
	case WM_INITDIALOG:
		// 그림 선택 상태에 따라 적당한 리스트 박스에 이름 입력
		for (i=0;i<TOTALBIT;i++) {
			wsprintf(Value, "%dSelect", i);
			iData = SHRegReadInt(SHCU,KEY"\\Picture", Value, 1);
			if (iData == 0) {
				SendMessage(GetDlgItem(hWnd, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM)Name[i]);
			} else {
				SendMessage(GetDlgItem(hWnd, IDC_LIST2), LB_ADDSTRING, 0, (LPARAM)Name[i]);
			}
		}
		// 확대 옵션 읽음
		iData = SHRegReadInt(SHCU,KEY"\\Setting", "Stretch", 20);
		if (iData == 0) SendMessage(GetDlgItem(hWnd, IDC_RASCALE1),BM_SETCHECK,BST_CHECKED,0);
		if (iData == 15) SendMessage(GetDlgItem(hWnd, IDC_RASCALE2),BM_SETCHECK,BST_CHECKED,0);
		if (iData == 20) SendMessage(GetDlgItem(hWnd, IDC_RASCALE3),BM_SETCHECK,BST_CHECKED,0);
		if (iData == 30) SendMessage(GetDlgItem(hWnd, IDC_RASCALE4),BM_SETCHECK,BST_CHECKED,0);
		// 화면 리셋 옵션 읽음
		iData = SHRegReadInt(SHCU,KEY"\\Setting", "Reset", 0);
		if (iData == 0) SendMessage(GetDlgItem(hWnd, IDC_RARESET1),BM_SETCHECK,BST_CHECKED,0);
		if (iData == 30) SendMessage(GetDlgItem(hWnd, IDC_RARESET2),BM_SETCHECK,BST_CHECKED,0);
		if (iData == 100) SendMessage(GetDlgItem(hWnd, IDC_RARESET3),BM_SETCHECK,BST_CHECKED,0);
		if (iData == 500) SendMessage(GetDlgItem(hWnd, IDC_RARESET4),BM_SETCHECK,BST_CHECKED,0);
		// 속도 옵션 읽음
		iData = SHRegReadInt(SHCU,KEY"\\Setting", "Speed", 3);
		if (iData == 1) SendMessage(GetDlgItem(hWnd, IDC_RASPEED1),BM_SETCHECK,BST_CHECKED,0);
		if (iData == 2) SendMessage(GetDlgItem(hWnd, IDC_RASPEED2),BM_SETCHECK,BST_CHECKED,0);
		if (iData == 3) SendMessage(GetDlgItem(hWnd, IDC_RASPEED3),BM_SETCHECK,BST_CHECKED,0);
		if (iData == 4) SendMessage(GetDlgItem(hWnd, IDC_RASPEED4),BM_SETCHECK,BST_CHECKED,0);
		// 사운드 옵션 읽음
		iData = SHRegReadInt(SHCU,KEY"\\Setting", "Sound", 0);
		if (iData == 1) SendMessage(GetDlgItem(hWnd, IDC_RASOUND1),BM_SETCHECK,BST_CHECKED,0);
		if (iData == 5) SendMessage(GetDlgItem(hWnd, IDC_RASOUND2),BM_SETCHECK,BST_CHECKED,0);
		if (iData == 10) SendMessage(GetDlgItem(hWnd, IDC_RASOUND3),BM_SETCHECK,BST_CHECKED,0);
		if (iData == 0) SendMessage(GetDlgItem(hWnd, IDC_RASOUND4),BM_SETCHECK,BST_CHECKED,0);
		// 사람 이름 출력 옵션 읽음
		o_PrintName = SHRegReadInt(SHCU,KEY"\\Setting", "PrintName", 0);
		if (o_PrintName == 1) {
			SendMessage(GetDlgItem(hWnd, IDC_CHECKPRINTNAME),BM_SETCHECK,BST_CHECKED,0);
		}
		// 마우스 이동 무시 옵션 읽음
		o_IgnoreMove = SHRegReadInt(SHCU,KEY"\\Setting", "IgnoreMove", 0);
		if (o_IgnoreMove == 1) {
			SendMessage(GetDlgItem(hWnd, IDC_CHECKIGNOREMOUSEMOVE),BM_SETCHECK,BST_CHECKED,0);
		}
		// 마우스 클릭 무시 옵션 읽음
		o_IgnoreClick = SHRegReadInt(SHCU,KEY"\\Setting", "IgnoreClick", 0);
		if (o_IgnoreClick == 1) {
			SendMessage(GetDlgItem(hWnd, IDC_CHECKIGNOREMOUSECLICK),BM_SETCHECK,BST_CHECKED,0);
		}
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		// 하나만 선택
		case IDC_BTNSELONE:
			idx = SendMessage(GetDlgItem(hWnd, IDC_LIST1),LB_GETCURSEL,0,0);
			if (idx != -1) {
				SendMessage(GetDlgItem(hWnd, IDC_LIST1),LB_GETTEXT,idx,(LPARAM)Value);
				SendMessage(GetDlgItem(hWnd, IDC_LIST1),LB_DELETESTRING,idx,0);
				SendMessage(GetDlgItem(hWnd, IDC_LIST2), LB_ADDSTRING, 0, (LPARAM)Value);
			}
			return TRUE;
		// 하나만 선택 해제
		case IDC_BTNDESELONE:
			idx = SendMessage(GetDlgItem(hWnd, IDC_LIST2),LB_GETCURSEL,0,0);
			if (idx != -1) {
				SendMessage(GetDlgItem(hWnd, IDC_LIST2),LB_GETTEXT,idx,(LPARAM)Value);
				SendMessage(GetDlgItem(hWnd, IDC_LIST2),LB_DELETESTRING,idx,0);
				SendMessage(GetDlgItem(hWnd, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM)Value);
			}
			return TRUE;
		// 몽땅 선택
		case IDC_BTNSELALL:
			count = SendMessage(GetDlgItem(hWnd, IDC_LIST1), LB_GETCOUNT,0,0);
			for (i=0;i<count;i++) {
				SendMessage(GetDlgItem(hWnd, IDC_LIST1),LB_GETTEXT,i,(LPARAM)Value);
				SendMessage(GetDlgItem(hWnd, IDC_LIST2), LB_ADDSTRING, 0, (LPARAM)Value);
			}
			SendMessage(GetDlgItem(hWnd, IDC_LIST1),LB_RESETCONTENT,0,0);
			return TRUE;
		// 몽땅 선택해제
		case IDC_BTNDESELALL:
			count = SendMessage(GetDlgItem(hWnd, IDC_LIST2), LB_GETCOUNT,0,0);
			for (i=0;i<count;i++) {
				SendMessage(GetDlgItem(hWnd, IDC_LIST2),LB_GETTEXT,i,(LPARAM)Value);
				SendMessage(GetDlgItem(hWnd, IDC_LIST1), LB_ADDSTRING, 0, (LPARAM)Value);
			}
			SendMessage(GetDlgItem(hWnd, IDC_LIST2),LB_RESETCONTENT,0,0);
			return TRUE;
		// 프로그램 소개
		case IDC_BTNABOUT:
			MessageBox(hWnd, "다울 스크린 세이버 Version 1.0\r"
				"다울 소프트를 이끌어가는 자랑스럽고도 예쁜 사람들의 얼굴을 소재로 만든 스크린 세이버입니다.\r" 
				"재밋게 보아주십시요", "프로그램 소개", MB_OK);
			return TRUE;
		// 디폴트 옵션으로 돌아가기 - 나머지 라디오 버튼의 체크를 모두 끄기 귀찮으므로 BM_CLICK 메시지를 보냈다.
		case IDC_BTNDEFAULT:
			SendMessage(GetDlgItem(hWnd, IDC_RASCALE3),BM_CLICK,0,0);
			SendMessage(GetDlgItem(hWnd, IDC_RARESET1),BM_CLICK,0,0);
			SendMessage(GetDlgItem(hWnd, IDC_RASPEED3),BM_CLICK,0,0);
			SendMessage(GetDlgItem(hWnd, IDC_RASOUND4),BM_CLICK,0,0);
			SendMessage(GetDlgItem(hWnd, IDC_CHECKPRINTNAME),BM_SETCHECK,BST_UNCHECKED,0);
			SendMessage(GetDlgItem(hWnd, IDC_CHECKIGNOREMOUSEMOVE),BM_SETCHECK,BST_UNCHECKED,0);
			SendMessage(GetDlgItem(hWnd, IDC_CHECKIGNOREMOUSECLICK),BM_SETCHECK,BST_UNCHECKED,0);
			SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(IDC_BTNSELALL,0), 0);
			return TRUE;
		// 옵션 설정 상태를 저장한다.
		case IDOK:
			// 그림 선택 상태를 저장한다.
			if (SendMessage(GetDlgItem(hWnd, IDC_LIST2), LB_GETCOUNT,0,0) == 0) {
				MessageBox(hWnd, "최소한 하나의 그림을 선택해야 합니다","이러시면 아니되옵니다",MB_OK);
				return TRUE;
			}
			for (i=0;i<TOTALBIT;i++) {
				wsprintf(Value, "%dSelect", i);
				if (SendMessage(GetDlgItem(hWnd, IDC_LIST2),LB_FINDSTRING, (WPARAM)-1,(LPARAM)Name[i]) == LB_ERR) {
					SHRegWriteInt(SHCU,KEY"\\Picture",Value,0);
				} else {
					SHRegWriteInt(SHCU,KEY"\\Picture",Value,1);
				}
			}
			// 확대 옵션을 저장한다.
			if (SendMessage(GetDlgItem(hWnd, IDC_RASCALE1), BM_GETCHECK,0,0) == BST_CHECKED) iData = 0;
			if (SendMessage(GetDlgItem(hWnd, IDC_RASCALE2), BM_GETCHECK,0,0) == BST_CHECKED) iData = 15;
			if (SendMessage(GetDlgItem(hWnd, IDC_RASCALE3), BM_GETCHECK,0,0) == BST_CHECKED) iData = 20;
			if (SendMessage(GetDlgItem(hWnd, IDC_RASCALE4), BM_GETCHECK,0,0) == BST_CHECKED) iData = 30;
			SHRegWriteInt(SHCU,KEY"\\Setting", "Stretch",iData);
			// 화면 리셋 옵션을 저장한다.
			if (SendMessage(GetDlgItem(hWnd, IDC_RARESET1), BM_GETCHECK,0,0) == BST_CHECKED) iData = 0;
			if (SendMessage(GetDlgItem(hWnd, IDC_RARESET2), BM_GETCHECK,0,0) == BST_CHECKED) iData = 30;
			if (SendMessage(GetDlgItem(hWnd, IDC_RARESET3), BM_GETCHECK,0,0) == BST_CHECKED) iData = 100;
			if (SendMessage(GetDlgItem(hWnd, IDC_RARESET4), BM_GETCHECK,0,0) == BST_CHECKED) iData = 500;
			SHRegWriteInt(SHCU,KEY"\\Setting", "Reset",iData);
			// 속도 옵션을 저장한다.
			if (SendMessage(GetDlgItem(hWnd, IDC_RASPEED1), BM_GETCHECK,0,0) == BST_CHECKED) iData = 1;
			if (SendMessage(GetDlgItem(hWnd, IDC_RASPEED2), BM_GETCHECK,0,0) == BST_CHECKED) iData = 2;
			if (SendMessage(GetDlgItem(hWnd, IDC_RASPEED3), BM_GETCHECK,0,0) == BST_CHECKED) iData = 3;
			if (SendMessage(GetDlgItem(hWnd, IDC_RASPEED4), BM_GETCHECK,0,0) == BST_CHECKED) iData = 4;
			SHRegWriteInt(SHCU,KEY"\\Setting", "Speed",iData);
			// 사운드 옵션을 저장한다.
			if (SendMessage(GetDlgItem(hWnd, IDC_RASOUND1), BM_GETCHECK,0,0) == BST_CHECKED) iData = 1;
			if (SendMessage(GetDlgItem(hWnd, IDC_RASOUND2), BM_GETCHECK,0,0) == BST_CHECKED) iData = 5;
			if (SendMessage(GetDlgItem(hWnd, IDC_RASOUND3), BM_GETCHECK,0,0) == BST_CHECKED) iData = 10;
			if (SendMessage(GetDlgItem(hWnd, IDC_RASOUND4), BM_GETCHECK,0,0) == BST_CHECKED) iData = 0;
			SHRegWriteInt(SHCU,KEY"\\Setting", "Sound",iData);
			// 사람 이름 출력 옵션을 저장한다.
			if (SendMessage(GetDlgItem(hWnd, IDC_CHECKPRINTNAME), BM_GETCHECK,0,0) == BST_CHECKED) iData = 1; 
			else iData = 0;
			SHRegWriteInt(SHCU,KEY"\\Setting", "PrintName",iData);
			// 마우스 이동 무시 옵션을 저장한다.
			if (SendMessage(GetDlgItem(hWnd, IDC_CHECKIGNOREMOUSEMOVE), BM_GETCHECK,0,0) == BST_CHECKED) iData = 1; 
			else iData = 0;
			SHRegWriteInt(SHCU,KEY"\\Setting", "IgnoreMove",iData);
			// 마우스 클릭 무시 옵션을 저장한다.
			if (SendMessage(GetDlgItem(hWnd, IDC_CHECKIGNOREMOUSECLICK), BM_GETCHECK,0,0) == BST_CHECKED) iData = 1; 
			else iData = 0;
			SHRegWriteInt(SHCU,KEY"\\Setting", "IgnoreClick",iData);

			EndDialog(hWnd, 0);
			return TRUE;
		// 옵션 설정 상태를 저장하지 않고 종료
		case IDCANCEL:
			EndDialog(hWnd, 0);
			return TRUE;
		}
	}
	return FALSE;
}

// 커스텀 클래스를 등록하는 함수. 스크린 세이버에서 필수적으로 요구하는 함수이나 이 프로그램
// 은 DLG_SCRNSAVECONFIGURE 대화상자를 사용하므로 TRUE를 리턴해 주기만 한다.
BOOL WINAPI RegisterDialogClasses(HANDLE hInst)
{
	return TRUE;
}

// 메인 윈도우 초기화
void OnCreate(HWND hWnd)
{
	int i;
	TCHAR Value[128];
	int Interval;

	// 화면 해상도를 구하고 난수 발생기를 초기화한다.
	SX = GetSystemMetrics(SM_CXSCREEN);
	SY = GetSystemMetrics(SM_CYSCREEN);
	srand(time(NULL));

	// 비트맵을 읽어들이고 레지스트리에서 출력 허가 여부를 읽어 놓는다.
	for (i=0;i<TOTALBIT;i++) {
		hBit[i] = (HBITMAP)LoadBitmap(hMainInstance, MAKEINTRESOURCE(IDB_BITMAP1+i));
		wsprintf(Value, "%dSelect", i);
		Sel[i] = SHRegReadInt(SHCU,KEY"\\Picture", Value, 1);
	}
	for (i=0;i<5;i++)
		hBanner[i] = (HBITMAP)LoadBitmap(hMainInstance, MAKEINTRESOURCE(IDB_BITDAUL1+i));

	// 옵션 설정 상태를 읽어와 전역 변수에 대입한다. 
	o_Stretch = SHRegReadInt(SHCU,KEY"\\Setting", "Stretch", 20);
	o_Reset = SHRegReadInt(SHCU,KEY"\\Setting", "Reset", 0);
	o_Speed = SHRegReadInt(SHCU,KEY"\\Setting", "Speed", 3);
	o_Sound = SHRegReadInt(SHCU,KEY"\\Setting", "Sound", 0);
	o_PrintName = SHRegReadInt(SHCU,KEY"\\Setting", "PrintName", 0);
	o_IgnoreMove = SHRegReadInt(SHCU,KEY"\\Setting", "IgnoreMove", 0);
	o_IgnoreClick = SHRegReadInt(SHCU,KEY"\\Setting", "IgnoreClick", 0);

	// 속도값에 따라 타이머와 Sleep 시간을 결정한다.
	switch (o_Speed) {
	case 1:		// 느림
		Interval = 1500;
		nSleep = 60;
		break;
	case 2:		// 보통
		Interval = 800;
		nSleep = 30;
		break;
	case 3:		// 빠름
		Interval = 300;
		nSleep = 20;
		break;
	case 4:		// 엄청 빠름
		Interval = 100;
		nSleep = 10;
		break;
	}
	SetTimer(hWnd, 0, Interval, NULL);
	SetTimer(hWnd, 1, 200, (TIMERPROC)BannerTimerProc);
}

// 배너를 보여주는 타이머 프로시저. 배너 출력 시간 간격을 좀 더 정밀하게 제어하기 위해
// 타이머 프로시저로 만들었다. 그러나 사진 출력함수에서 Sleep로 CPU를 잡고 있기 때문에
// 별다른 효과가 없었다. 
VOID CALLBACK BannerTimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	HDC hdc = GetDC(hMainWindow);
	g_Banner++;
	if (g_Banner == 1) DrawBitmap(hdc, SX-150,0,150,30,0,0,150,30,hBanner[0]);
	if (g_Banner == 2) DrawBitmap(hdc, SX-150,0,150,30,0,0,150,30,hBanner[1]);
	if (g_Banner == 3) DrawBitmap(hdc, SX-150,0,150,30,0,0,150,30,hBanner[2]);
	if (g_Banner == 4) DrawBitmap(hdc, SX-150,0,150,30,0,0,150,30,hBanner[3]);
	if (g_Banner >= 5) DrawBitmap(hdc, SX-150,0,150,30,0,0,150,30,hBanner[4]);
	if (g_Banner == 12) g_Banner = 0;
	ReleaseDC(hMainWindow, hdc);
}
