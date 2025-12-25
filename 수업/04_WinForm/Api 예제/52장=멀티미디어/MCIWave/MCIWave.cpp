#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("MCIWave");

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

#include "mmsystem.h"
UINT wDeviceID=0;
DWORD PlayWaveFile(HWND hWndNotify, LPSTR lpszWave)
{
	DWORD Result;
	MCI_OPEN_PARMS mciOpenParms;
	MCI_PLAY_PARMS mciPlayParms;

	// 장치를 Open하고 ID를 발급받는다.
	mciOpenParms.lpstrDeviceType = "WaveAudio";
	mciOpenParms.lpstrElementName = lpszWave;
	Result=mciSendCommand(0,MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, 
		(DWORD)(LPVOID) &mciOpenParms);
	if (Result) {
		return Result;
	}
	wDeviceID=mciOpenParms.wDeviceID;

	mciPlayParms.dwCallback=(DWORD)hWndNotify;
	Result = mciSendCommand(wDeviceID, MCI_PLAY, MCI_NOTIFY, 
		(DWORD)(LPVOID)&mciPlayParms);
	if (Result) {
		mciSendCommand(wDeviceID, MCI_CLOSE, 0, (DWORD)NULL);
		return Result;
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	DWORD Result;
	TCHAR str[256];
	TCHAR *Mes="왼쪽 버튼을 누르면 사운드를 연주합니다.";

	switch (iMessage) {
	case WM_LBUTTONDOWN:
		Result=PlayWaveFile(hWnd, "Bugle.wav");
		if (Result) {
			mciGetErrorString(Result, str, 256);
			MessageBox(hWnd, str,"에러 발생",MB_OK);
		}
		return 0;
	case MM_MCINOTIFY:
		switch (wParam) {
		case MCI_NOTIFY_SUCCESSFUL:
		case MCI_NOTIFY_FAILURE:
		case MCI_NOTIFY_SUPERSEDED:
		case MCI_NOTIFY_ABORTED:
			mciSendCommand(LOWORD(lParam), MCI_CLOSE, 0, (DWORD)NULL);
			break;
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd,&ps);
		TextOut(hdc,10,10,Mes,lstrlen(Mes));
		EndPaint(hWnd,&ps);
		return 0;
	case WM_DESTROY:
		if (wDeviceID)
			mciSendCommand(wDeviceID, MCI_CLOSE, 0, (DWORD)NULL);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

