#include <windows.h>

void main()
{
	HANDLE hOut;
	TCHAR str[50];
	CONSOLE_SCREEN_BUFFER_INFO BufInfo;
	COORD Cur;
	DWORD dw;

	hOut=GetStdHandle(STD_OUTPUT_HANDLE);

	// 커서의 위치 조사
	GetConsoleScreenBufferInfo(hOut,&BufInfo);
	wsprintf(str,"X=%d,y=%d",BufInfo.dwCursorPosition.X, BufInfo.dwCursorPosition.Y);

	// 임의 위치에 문자열 출력
	Cur.X=50;
	Cur.Y=10;
	SetConsoleCursorPosition(hOut,Cur);
	WriteFile(hOut,str,lstrlen(str),&dw,NULL);
}
