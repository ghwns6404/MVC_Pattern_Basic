#include <windows.h>
#include <iostream.h>

void main()
{
	HANDLE hIn,hOut;
	TCHAR *Mes="input your name:";
	TCHAR name[21];
	DWORD dwWritten, dwRead;
	TCHAR str2[50];

	hIn=GetStdHandle(STD_INPUT_HANDLE);
	hOut=GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleMode(hIn,ENABLE_PROCESSED_INPUT | ENABLE_LINE_INPUT 
		| ENABLE_ECHO_INPUT | ENABLE_MOUSE_INPUT);
	SetConsoleMode(hOut,ENABLE_PROCESSED_OUTPUT 
		| ENABLE_WRAP_AT_EOL_OUTPUT);

	WriteFile(hOut,Mes,lstrlen(Mes),&dwWritten,NULL);
	memset(name,0,sizeof(name));
	ReadFile(hIn,name,20,&dwRead,NULL);
	wsprintf(str2,"your name is %s",name);
	WriteFile(hOut,str2,lstrlen(str2),&dwWritten,NULL);
}
