#include <windows.h>
#include <stdio.h>

void main(int argc, char* argv[])
{
	HANDLE hOut;
	int Size;
	CONSOLE_CURSOR_INFO CurInfo;

	if (argc == 1) {
		printf("This Program Require Argument");
		return;
	}

	Size=atoi(argv[1]);
	if (Size==0)
		Size=10;

	hOut=GetStdHandle(STD_OUTPUT_HANDLE);

	// 커서 모양 변경
	if (argv[1][0]=='-') {
		CurInfo.dwSize=1;
		CurInfo.bVisible=FALSE;
	} else {
		CurInfo.dwSize=Size*10;
		CurInfo.bVisible=TRUE;
	}
	SetConsoleCursorInfo(hOut,&CurInfo);
}
