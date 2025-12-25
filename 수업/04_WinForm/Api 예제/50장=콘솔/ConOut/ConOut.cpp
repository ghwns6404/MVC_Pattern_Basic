#include <windows.h>
#include <iostream.h>
#include <stdio.h>

void main()
{
	HANDLE hOut;
	TCHAR *str="Console Test\n";
	DWORD dwWritten;

	// 첫 번째 방법
	printf("%s",str);

	// 두 번째 방법
	cout << str;

	// 세 번째 방법
	hOut=GetStdHandle(STD_OUTPUT_HANDLE);
	WriteFile(hOut,str,lstrlen(str),&dwWritten,NULL);

	// 네 번째 방법
	WriteConsole(hOut,str,lstrlen(str),&dwWritten,NULL);
}
