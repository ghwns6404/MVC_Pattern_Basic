#include <windows.h>

void main()
{
	HANDLE hIn,hOut,hOut2,hNow;
	TCHAR *str="Console Test";
	TCHAR ch='a';
	DWORD dw;

	hIn=GetStdHandle(STD_INPUT_HANDLE);
	hOut=GetStdHandle(STD_OUTPUT_HANDLE);

	hNow=hOut;
	SetConsoleMode(hIn,ENABLE_PROCESSED_INPUT);

	hOut2=CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE,0,NULL,
		CONSOLE_TEXTMODE_BUFFER,0);

	while (ch!='0') {
		ReadFile(hIn,&ch,1,&dw,NULL);
		WriteFile(hNow,&ch,1,&dw,NULL);
		if (ch=='1') {
			hNow=hOut;
		}
		if (ch=='2') {
			hNow=hOut2;
		}
		SetConsoleActiveScreenBuffer(hNow);
	}

	CloseHandle(hOut2);
}
